#pragma once

#include "ComputeShader.h"
#include "Frame.h"

namespace rgbd
{
	enum MAP_TYPE
	{
		COLOR_BGR,
		COLOR_RGB,
		DEPTH,
		VERTEX,
		NORMAL,
	};

	class DownSampling : public ComputeShader
	{
	private:
		std::map<rgbd::MAP_TYPE, const gl::Shader::Ptr> progs;

	public:
		DownSampling(
			const gl::Shader::Ptr colorShader,
			const gl::Shader::Ptr depthShader,
			const gl::Shader::Ptr vertexShader,
			const gl::Shader::Ptr normalShader
		);

		void execute(
			gl::Texture::Ptr src,
			gl::Texture::Ptr dst,
			rgbd::MAP_TYPE type
		);
	};
}