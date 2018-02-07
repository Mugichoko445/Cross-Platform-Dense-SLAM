#include "BilateralFilter.h"

namespace rgbd
{
	BilateralFilter::BilateralFilter(
		const gl::Shader::Ptr prog
	) : prog(prog)
	{
	}

	void BilateralFilter::execute(
		gl::Texture::Ptr srcDepthMap,
		gl::Texture::Ptr dstDepthMap,
		float sigma,
		float bSigma
	)
	{
		prog->setUniform("sigma", sigma);
		prog->setUniform("bSigma", bSigma);

		prog->use();
		srcDepthMap->bindImage(0, GL_READ_ONLY);
		dstDepthMap->bindImage(1, GL_WRITE_ONLY);
		glDispatchCompute(dstDepthMap->getWidth() / 20, dstDepthMap->getHeight() / 20, 1);
		prog->disuse();
	}
}