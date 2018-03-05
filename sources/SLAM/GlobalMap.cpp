#include "GlobalMap.h"

namespace rgbd
{
	const unsigned int GlobalMapConstParam::MAX_MAP_SIZE = 2500000;
	const float GlobalMapConstParam::CSTABLE = 10.0f;

	GlobalMap::GlobalMap(
		int width,
		int height,
		const glm::mat3 &K,
		const std::map<std::string, const gl::Shader::Ptr> &progs
	) : width(width), height(height),
		progs{ { "IndexMapGeneration", progs.at("IndexMapGeneration") },
		{ "GlobalMapUpdate", progs.at("GlobalMapUpdate") },
		{ "UnnecessaryPointRemoval", progs.at("UnnecessaryPointRemoval") },
		{ "SurfaceSplatting", progs.at("SurfaceSplatting") } }
	{
		gl::Texture::Ptr indexMap = std::make_shared<gl::Texture>();
		indexMap->create(0, width * 4, height * 4, 1, gl::TextureType::FLOAT32);
		indexMap->setFiltering(gl::TextureFilter::NEAREST);
		indexMap->setWarp(gl::TextureWarp::CLAMP_TO_EDGE);

		virtualFrameFBO.create(width, height);
		indexMapFBO.create(indexMap->getWidth(), indexMap->getHeight());
		indexMapFBO.attach(indexMap, 0);

		indexMapFBO.bind();
		glClearColor(-1.0f, -1.0f, -1.0f, -1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		indexMapFBO.unbind();

		buffSwitch = 0;
		mapSize = 0;

		GLuint valueZero(0);
		for (int i = 0; i < atomic.size(); ++i)
		{
			atomic[i].bind();
			atomic[i].create(&valueZero, 1, GL_DYNAMIC_DRAW);
			atomic[i].bindBase(i);
			atomic[i].unbind();
		}
		std::vector<GlobalMapData> tmpMapData(GlobalMapConstParam::MAX_MAP_SIZE);
		for (int i = 0; i < ssbo.size(); ++i)
		{
			ssbo[i].bind();
			ssbo[i].create(tmpMapData.data(), GlobalMapConstParam::MAX_MAP_SIZE, GL_DYNAMIC_DRAW);
			ssbo[i].bindBase(i);
			ssbo[i].unbind();
		}

		glm::mat4 P = rgbd::calibratedPerspective(
			ICPConstParam::MIN_DEPTH, ICPConstParam::MAX_DEPTH,
			width, height, K[2][0], K[2][1], K[0][0], K[1][1], 0.0f
		);
		this->progs["IndexMapGeneration"]->setUniform("P", P);

		this->progs["GlobalMapUpdate"]->setUniform("timestamp", 0);
		this->progs["GlobalMapUpdate"]->setUniform("sigma", 0.6f);
		this->progs["GlobalMapUpdate"]->setUniform("c_stable", GlobalMapConstParam::CSTABLE);
		this->progs["GlobalMapUpdate"]->setUniform("K", K);
		this->progs["GlobalMapUpdate"]->setUniform("maxMapSize", GlobalMapConstParam::MAX_MAP_SIZE);

		this->progs["SurfaceSplatting"]->setUniform("P", P);
		this->progs["SurfaceSplatting"]->setUniform("c_stable", GlobalMapConstParam::CSTABLE);
		this->progs["SurfaceSplatting"]->setUniform("scale", 1.5f);

		this->progs["UnnecessaryPointRemoval"]->setUniform("c_stable", GlobalMapConstParam::CSTABLE);
	}

	GlobalMap::~GlobalMap()
	{
	}

	GLuint GlobalMap::getMapSize()
	{
		return mapSize;
	}

	void GlobalMap::genIndexMap(const glm::mat4 &invT)
	{
		glEnable(GL_DEPTH_TEST);

		indexMapFBO.bind();
		glClearColor(-1.0f, -1.0f, -1.0f, -1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, indexMapFBO.getWidth(), indexMapFBO.getHeight());

		progs["IndexMapGeneration"]->use();
		progs["IndexMapGeneration"]->setUniform("invT", invT);
		
		ssbo[buffSwitch].bindBase(0);
		
		glDrawArrays(GL_POINTS, 0, getMapSize());

		progs["IndexMapGeneration"]->disuse();
		indexMapFBO.unbind();
	}

	void GlobalMap::updateGlobalMap(
		const rgbd::Frame &srcFrame,
		const glm::mat4 &T,
		const int timestamp
	)
	{
		progs["GlobalMapUpdate"]->use();
		progs["GlobalMapUpdate"]->setUniform("T", T);
		progs["GlobalMapUpdate"]->setUniform("invT", glm::inverse(T));
		progs["GlobalMapUpdate"]->setUniform("timestamp", timestamp);

		indexMapFBO.getColorAttachment(0)->bindImage(0, GL_READ_ONLY);
		srcFrame.getVertexMap()->bindImage(1, GL_READ_ONLY);
		srcFrame.getNormalMap()->bindImage(2, GL_READ_ONLY);
		srcFrame.getColorMap()->bindImage(3, GL_READ_ONLY);	// <-- debugging: color integration
		
		atomic[buffSwitch].update(&mapSize, 0, 1);
		atomic[buffSwitch].bindBase(0);
		ssbo[buffSwitch].bindBase(0);
		
		glDispatchCompute(width / 20, height / 20, 1);
		atomic[buffSwitch].read(&mapSize, 0, 1);
		progs["GlobalMapUpdate"]->disuse();
	}

	void GlobalMap::removeUnnecessaryPoints(int timestamp)
	{
		std::array<int, 2> _buffSwitch = { buffSwitch, (buffSwitch + 1) % 2 };
		
		progs["UnnecessaryPointRemoval"]->use();
		progs["UnnecessaryPointRemoval"]->setUniform("timestamp", timestamp);
		progs["UnnecessaryPointRemoval"]->setUniform("c_stable", GlobalMapConstParam::CSTABLE);
		
		GLuint valueZero(0);
		atomic[_buffSwitch.back()].bindBase(1);
		atomic[_buffSwitch.back()].update(&valueZero, 0, 1);
		ssbo[_buffSwitch.front()].bindBase(0);
		ssbo[_buffSwitch.back()].bindBase(1);
		
		glDispatchCompute(mapSize / 400, 1, 1);
		
		atomic[_buffSwitch.back()].read(&mapSize, 0, 1);
		progs["UnnecessaryPointRemoval"]->disuse();

		buffSwitch = _buffSwitch.back();
	}

	void GlobalMap::genVirtualFrame(
		const rgbd::Frame &dstFrame,
		const glm::mat4 &invT
	)
	{
		virtualFrameFBO.attach(dstFrame.getVertexMap(), 0);
		virtualFrameFBO.attach(dstFrame.getNormalMap(), 1);
		virtualFrameFBO.attach(dstFrame.getDepthMap(), 2);
		virtualFrameFBO.attach(dstFrame.getColorMap(), 3);

		std::vector<GLenum> drawBuffs = virtualFrameFBO.getDrawBuffers();

		virtualFrameFBO.bind();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, virtualFrameFBO.getWidth(), virtualFrameFBO.getHeight());
		
		progs["SurfaceSplatting"]->use();
		progs["SurfaceSplatting"]->setUniform("invT", invT);
		
		ssbo[buffSwitch].bindBase(0);
		
		glDrawBuffers((GLsizei)drawBuffs.size(), drawBuffs.data());
		glDrawArrays(GL_POINTS, 0, getMapSize());
		
		glBindTexture(GL_TEXTURE_2D, 0);
		progs["SurfaceSplatting"]->disuse();
		virtualFrameFBO.unbind();
	}
}