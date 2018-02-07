#pragma once

#include "ComputeShader.h"

namespace rgbd
{
	class CalcNormalMap : public ComputeShader
	{
	private:
		const gl::Shader::Ptr prog;

	public:
		CalcNormalMap(
			const gl::Shader::Ptr prog
		);

		void execute(
			gl::Texture::Ptr srcVertexMap,
			gl::Texture::Ptr dstNormalMap
		);
	};
}