#pragma once

#include "ConstantParameters.h"
#include "IterativeClosestPoint.h"

namespace rgbd
{
	class PyramidricalICP
	{
	private:
		std::vector<rgbd::PointToPlaneICP::Ptr> icp;

	public:
		PyramidricalICP(
			int width,
			int height,
			const glm::mat3 &K,
			const std::map<std::string, const gl::Shader::Ptr> &progs
		);
		~PyramidricalICP();

		void calc(
			const rgbd::Frame &prevFrame,
			const rgbd::Frame &currFrame,
			glm::mat4 &T
		);

		typedef std::shared_ptr<rgbd::PyramidricalICP> Ptr;
	};
}