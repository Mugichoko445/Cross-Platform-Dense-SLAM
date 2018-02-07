#pragma once

#include <opencv2/opencv.hpp>
#include <GLCore/Shader.h>
#include "ConstantParameters.h"
#include "VirtualFrameRenderer.h"
#include "ProjectiveDataAssoc.h"

// Ref: "Linear Least - squares Optimization for Point-to-plane ICP Surface Registration"
namespace rgbd
{
	class PointToPlaneICP
	{
	private:
		VirtualFrameRenderer virtualFrameRenderer;
		ProjectiveDataAssoc dataAssoc;

	public:
		PointToPlaneICP(
			int width,
			int height,
			const glm::mat3 &K,
			const std::map<std::string, const gl::Shader::Ptr> &progs
		);
		~PointToPlaneICP();
		
		void paramToMat(
			const cv::Mat &params,	// parameters (6 x 1 matrix)
			glm::mat4 &T			// 4x4 matrix in glm::mat4
		);
		
		void calc(
			const int level,
			const rgbd::Frame &prevFrame,
			const rgbd::Frame &currFrame,
			glm::mat4 &T,
			const float finThresh = 1.0e-4F
		);

		typedef std::shared_ptr<rgbd::PointToPlaneICP> Ptr;
	};
}
