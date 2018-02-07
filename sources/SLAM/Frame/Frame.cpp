#include "Frame.h"

namespace rgbd
{
	Frame::Frame()
	{
	}

	Frame::~Frame()
	{
	}

	void Frame::create(
		int width,
		int height,
		int maxLevel,
		float minDepth,
		float maxDepth,
		const glm::mat3 &K,
		std::map<std::string, const gl::Shader::Ptr> &progs
	)
	{
		this->width = width;
		this->height = height;

		frameData.resize(maxLevel);

		// Color needs only "one" level
		// Note: Color must be 4ch since compute shader does not support rgb8 internal format.
		frameData[0].colorMap = std::make_shared<gl::Texture>();
		frameData[0].colorMap->create(0, width, height, 4, gl::TextureType::COLOR);
		frameData[0].colorMap->setFiltering(gl::TextureFilter::NEAREST);
		frameData[0].colorMap->setWarp(gl::TextureWarp::CLAMP_TO_EDGE);

		for (int lv = 0; lv < frameData.size(); ++lv)
		{
			int dev = int(pow(2, lv));
			int _w(width / dev), _h(height / dev);
			frameData[lv].depthMap = std::make_shared<gl::Texture>();
			frameData[lv].depthMap->create(0, _w, _h, 1, gl::TextureType::FLOAT32);
			frameData[lv].depthMap->setFiltering(gl::TextureFilter::NEAREST);
			frameData[lv].depthMap->setWarp(gl::TextureWarp::CLAMP_TO_EDGE);

			frameData[lv].vertexMap = std::make_shared<gl::Texture>();
			frameData[lv].vertexMap->create(0, _w, _h, 4, gl::TextureType::FLOAT32);
			frameData[lv].vertexMap->setFiltering(gl::TextureFilter::NEAREST);
			frameData[lv].vertexMap->setWarp(gl::TextureWarp::CLAMP_TO_EDGE);

			frameData[lv].normalMap = std::make_shared<gl::Texture>();
			frameData[lv].normalMap->create(0, _w, _h, 4, gl::TextureType::FLOAT32);
			frameData[lv].normalMap->setFiltering(gl::TextureFilter::NEAREST);
			frameData[lv].normalMap->setWarp(gl::TextureWarp::CLAMP_TO_EDGE);
		}
		rawDepthMap = std::make_shared<gl::Texture>();
		rawDepthMap->create(0, width, height, 1, gl::TextureType::FLOAT32);
		rawDepthMap->setFiltering(gl::TextureFilter::NEAREST);
		rawDepthMap->setWarp(gl::TextureWarp::CLAMP_TO_EDGE);

		bilateralFilter = std::make_shared<rgbd::BilateralFilter>(progs["BilateralFilter"]);
		vertexMapProc = std::make_shared<rgbd::CalcVertexMap>(minDepth, maxDepth, K, progs["CalcVertexMap"]);
		normalMapProc = std::make_shared<rgbd::CalcNormalMap>(progs["CalcNormalMap"]);
		downSampling.resize(maxLevel - 1);
		for (int lv = 0; lv < downSampling.size(); ++lv)
		{
			downSampling[lv] = std::make_shared<rgbd::DownSampling>(
				progs["DownSamplingC"], progs["DownSamplingD"],
				progs["DownSamplingV"], progs["DownSamplingN"]
				);
		}
	}

	void Frame::update(
		const void *colorData,
		const void *depthData,
		float bfSigma,
		float bfDSigma
	) const
	{
		frameData[0].colorMap->update(colorData);
		rawDepthMap->update(depthData);
		std::dynamic_pointer_cast<rgbd::BilateralFilter>(bilateralFilter)->execute(rawDepthMap, frameData[0].depthMap, bfSigma, bfDSigma);
		std::dynamic_pointer_cast<rgbd::CalcVertexMap>(vertexMapProc)->execute(frameData[0].depthMap, frameData[0].vertexMap);
		std::dynamic_pointer_cast<rgbd::CalcNormalMap>(normalMapProc)->execute(frameData[0].vertexMap, frameData[0].normalMap);

		update();
	}

	void Frame::update() const
	{
		for (int lv = 0; lv < downSampling.size(); ++lv)
		{
			std::dynamic_pointer_cast<rgbd::DownSampling>(downSampling[lv])->execute(frameData[lv].depthMap, frameData[lv + 1].depthMap, MAP_TYPE::DEPTH);
			std::dynamic_pointer_cast<rgbd::DownSampling>(downSampling[lv])->execute(frameData[lv].vertexMap, frameData[lv + 1].vertexMap, MAP_TYPE::VERTEX);
			std::dynamic_pointer_cast<rgbd::DownSampling>(downSampling[lv])->execute(frameData[lv].normalMap, frameData[lv + 1].normalMap, MAP_TYPE::NORMAL);
		}
	}

	int Frame::getWidth(int lv) const
	{
		return width / int(pow(2, lv));
	}

	int Frame::getHeight(int lv) const
	{
		return height / int(pow(2, lv));
	}

	gl::Texture::Ptr Frame::getColorMap(int lv) const
	{
		return frameData[lv].colorMap;
	}

	gl::Texture::Ptr Frame::getDepthMap(int lv) const
	{
		return frameData[lv].depthMap;
	}

	gl::Texture::Ptr Frame::getVertexMap(int lv) const
	{
		return frameData[lv].vertexMap;
	}

	gl::Texture::Ptr Frame::getNormalMap(int lv) const
	{
		return frameData[lv].normalMap;
	}
}