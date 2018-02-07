#include "VirtualFrameRenderer.h"

namespace rgbd
{
	VirtualFrameRenderer::VirtualFrameRenderer(
		int width,
		int height,
		const glm::mat3 &K,
		const gl::Shader::Ptr prog
	) : prog(prog)
	{
		vertexMap = std::make_shared<gl::Texture>();
		vertexMap->create(0, width, height, 4, gl::TextureType::FLOAT32);
		vertexMap->setFiltering(gl::TextureFilter::NEAREST);
		vertexMap->setWarp(gl::TextureWarp::CLAMP_TO_EDGE);

		normalMap = std::make_shared<gl::Texture>();
		normalMap->create(0, width, height, 4, gl::TextureType::FLOAT32);
		normalMap->setFiltering(gl::TextureFilter::NEAREST);
		normalMap->setWarp(gl::TextureWarp::CLAMP_TO_EDGE);

		virtualFrameFBO.create(width, height);
		virtualFrameFBO.attach(vertexMap, 0);
		virtualFrameFBO.attach(normalMap, 1);

		glm::mat4 P = rgbd::calibratedPerspective(
			ICPConstParam::MIN_DEPTH, ICPConstParam::MAX_DEPTH,
			width, height, K[2][0], K[2][1], K[0][0], K[1][1], 0.0f);
		prog->setUniform("P", P);
	}
	VirtualFrameRenderer::~VirtualFrameRenderer()
	{
	}


	gl::Texture::Ptr VirtualFrameRenderer::getVirtualVertexMap()
	{
		return vertexMap;
	}

	gl::Texture::Ptr VirtualFrameRenderer::getVirtualNormalMap()
	{
		return normalMap;
	}

	void VirtualFrameRenderer::execute(
		int level,
		const rgbd::Frame &srcFrame,
		const glm::mat4 &T
	)
	{
		virtualFrameFBO.bind();
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, virtualFrameFBO.getWidth(), virtualFrameFBO.getHeight());

		prog->use();
		prog->setUniform("T", T);

		srcFrame.getVertexMap(level)->use(0);
		srcFrame.getNormalMap(level)->use(1);

		std::vector<GLenum> drawBuffs = virtualFrameFBO.getDrawBuffers();
		glDrawBuffers((GLsizei)drawBuffs.size(), drawBuffs.data());
		glDrawArrays(GL_POINTS, 0, virtualFrameFBO.getWidth() * virtualFrameFBO.getHeight());

		glBindTexture(GL_TEXTURE_2D, 0);
		prog->disuse();

		virtualFrameFBO.unbind();
	}
}