#include "DownSampling.h"

namespace rgbd
{
	DownSampling::DownSampling(
		const gl::Shader::Ptr colorShader,
		const gl::Shader::Ptr depthShader,
		const gl::Shader::Ptr vertexShader,
		const gl::Shader::Ptr normalShader
	) : progs{ std::make_pair(MAP_TYPE::COLOR_BGR, colorShader),
		std::make_pair(MAP_TYPE::DEPTH, depthShader),
		std::make_pair(MAP_TYPE::VERTEX, vertexShader),
		std::make_pair(MAP_TYPE::NORMAL, normalShader) }
	{
	}

	void DownSampling::execute(
		gl::Texture::Ptr src,
		gl::Texture::Ptr dst,
		rgbd::MAP_TYPE type
	)
	{
		progs[type]->use();

		src->bindImage(0, GL_READ_ONLY);
		dst->bindImage(1, GL_WRITE_ONLY);
		glDispatchCompute(dst->getWidth() / 20, dst->getHeight() / 20, 1);

		progs[type]->disuse();
	}
}