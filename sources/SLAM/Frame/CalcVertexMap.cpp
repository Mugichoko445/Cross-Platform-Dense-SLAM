#include "CalcVertexMap.h"

namespace rgbd
{
	CalcVertexMap::CalcVertexMap(
		float minDepth,
		float maxDepth,
		const glm::mat3 &K,
		const gl::Shader::Ptr prog
	) : prog(prog), invK(glm::inverse(K))
	{
		invK = glm::inverse(K);
		prog->setUniform("invK", invK);
		prog->setUniform("minDepth", minDepth);
		prog->setUniform("maxDepth", maxDepth);
	}

	void CalcVertexMap::execute(
		gl::Texture::Ptr srcDepthMap,
		gl::Texture::Ptr dstVertexMap
	)
	{
		prog->setUniform("invK", invK);

		prog->use();
		srcDepthMap->bindImage(0, GL_READ_ONLY);
		dstVertexMap->bindImage(1, GL_WRITE_ONLY);
		glDispatchCompute(dstVertexMap->getWidth() / 20, dstVertexMap->getHeight() / 20, 1);
		prog->disuse();
	}
}