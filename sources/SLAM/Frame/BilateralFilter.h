#pragma once

#include "ComputeShader.h"

namespace rgbd
{
	// Ref: https://www.shadertoy.com/view/4dfGDH
	class BilateralFilter : public ComputeShader
	{
	private:
		const gl::Shader::Ptr prog;

	public:
		BilateralFilter(
			const gl::Shader::Ptr prog
		);
		void execute(
			gl::Texture::Ptr srcDepthMap,
			gl::Texture::Ptr dstDepthMap,
			float sigma = 15.0f,
			float bSigma = 0.05f
		);
	};
}