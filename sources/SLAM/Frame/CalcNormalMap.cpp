#include "CalcNormalMap.h"

namespace rgbd
{
	CalcNormalMap::CalcNormalMap(
		const gl::Shader::Ptr prog
	) : prog(prog)
	{
	}

	void CalcNormalMap::execute(
		gl::Texture::Ptr srcVertexMap,
		gl::Texture::Ptr dstNormalMap
	)
	{
		prog->use();
		srcVertexMap->bindImage(0, GL_READ_ONLY);
		dstNormalMap->bindImage(1, GL_WRITE_ONLY);
		glDispatchCompute(dstNormalMap->getWidth() / 20, dstNormalMap->getHeight() / 20, 1);
		prog->disuse();
	}
}