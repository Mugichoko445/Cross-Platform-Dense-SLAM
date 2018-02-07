#include "ProjectiveDataAssoc.h"

namespace rgbd
{
	ProjectiveDataAssoc::ProjectiveDataAssoc(
		int width,
		int height,
		const std::map<std::string, const gl::Shader::Ptr> progs
	) : width(width), height(height),
		progs{ { "ProjectiveDataAssoc", progs.at("ProjectiveDataAssoc") },
		{ "MultiplyMatrices", progs.at("MultiplyMatrices") } }
	{
		this->progs["ProjectiveDataAssoc"]->use();
		this->progs["ProjectiveDataAssoc"]->setUniform("ed", rgbd::ICPConstParam::DST_THRESH);
		this->progs["ProjectiveDataAssoc"]->setUniform("et", rgbd::ICPConstParam::ANG_THRESH);
		this->progs["ProjectiveDataAssoc"]->setUniform("minZ", rgbd::ICPConstParam::MIN_DEPTH);
		this->progs["ProjectiveDataAssoc"]->setUniform("maxZ", rgbd::ICPConstParam::MAX_DEPTH);
		this->progs["ProjectiveDataAssoc"]->disuse();

		assocDataTex.create(0, 6, 7, 1, gl::TextureType::FLOAT32);
		assocDataTex.setFiltering(gl::TextureFilter::NEAREST);

		GLuint valueZero(0);
		atomic.bind();
		atomic.create(&valueZero, 1, GL_DYNAMIC_DRAW);
		atomic.bindBase(2);
		atomic.unbind();

		std::vector<ICPProjectiveData> icpData(width * height);
		ssbo.bind();
		ssbo.create(icpData.data(), width * height, GL_DYNAMIC_DRAW);
		ssbo.bindBase(2);
		ssbo.unbind();
	}

	ProjectiveDataAssoc::~ProjectiveDataAssoc()
	{
	}

	GLuint ProjectiveDataAssoc::execute(
		int level,
		const rgbd::Frame &prevFrame,
		gl::Texture::Ptr virtualVertexMap,
		gl::Texture::Ptr virtualNormalMap,
		float *assocData
	)
	{
		progs["ProjectiveDataAssoc"]->use();
		virtualVertexMap->bindImage(0, GL_READ_ONLY);
		virtualNormalMap->bindImage(1, GL_READ_ONLY);
		prevFrame.getVertexMap(level)->bindImage(2, GL_READ_ONLY);
		prevFrame.getNormalMap(level)->bindImage(3, GL_READ_ONLY);
		
		GLuint numOfValidPix(0);
		atomic.bindBase(2);
		atomic.update(&numOfValidPix, 0, 1);
		ssbo.bindBase(2);
		glDispatchCompute(width / 20, height / 20, 1);
		atomic.read(&numOfValidPix, 0, 1);
		progs["ProjectiveDataAssoc"]->disuse();

		progs["MultiplyMatrices"]->use();
		atomic.bindBase(2);
		atomic.update(&numOfValidPix, 0, 1);
		ssbo.bindBase(2);
		assocDataTex.bindImage(0, GL_WRITE_ONLY);
		glDispatchCompute(6, 7, 1);
		progs["MultiplyMatrices"]->disuse();

		assocDataTex.read(assocData);

		return numOfValidPix;
	}

	int ProjectiveDataAssoc::getWidth()
	{
		return width;
	}
	int ProjectiveDataAssoc::getHeight()
	{
		return height;
	}
}