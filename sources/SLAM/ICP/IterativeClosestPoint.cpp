#include "IterativeClosestPoint.h"

namespace rgbd
{
	PointToPlaneICP::PointToPlaneICP(
		int width,
		int height,
		const glm::mat3 &K,
		const std::map<std::string, const gl::Shader::Ptr> &progs
	) : virtualFrameRenderer(width, height, K, progs.at("VirtualMapGeneration")),
		dataAssoc(width, height, progs)
	{
	}

	PointToPlaneICP::~PointToPlaneICP()
	{
	}

	void PointToPlaneICP::paramToMat(
		const cv::Mat &params,
		glm::mat4 &T
	)
	{
		const float a(params.ptr<float>(0)[0]);	// alpha (yaw)
		const float b(params.ptr<float>(1)[0]);	// beta (pitch)
		const float g(params.ptr<float>(2)[0]);	// gamma (roll)
		const float cos_a(cos(a));
		const float cos_b(cos(b));
		const float cos_g(cos(g));
		const float sin_a(sin(a));
		const float sin_b(sin(b));
		const float sin_g(sin(g));

		T[0][0] = cos_g * cos_b;
		T[1][0] = -sin_g * cos_a + cos_g * sin_b * sin_a;
		T[2][0] = sin_g * sin_a + cos_g * sin_b * cos_a;
		T[3][0] = params.ptr<float>(3)[0];
		T[0][1] = sin_g * cos_b;
		T[1][1] = cos_g * cos_a + sin_g * sin_b * sin_a;
		T[2][1] = -cos_g * sin_a + sin_g * sin_b * cos_a;
		T[3][1] = params.ptr<float>(4)[0];
		T[0][2] = -sin_b;
		T[1][2] = cos_b * sin_a;
		T[2][2] = cos_b * cos_a;
		T[3][2] = params.ptr<float>(5)[0];

		// <-- An alternative
		//const float a(params.ptr<float>(0)[0]);	// alpha (yaw)
		//const float b(params.ptr<float>(1)[0]);	// beta (pitch)
		//const float g(params.ptr<float>(2)[0]);	// gamma (roll)
		//glm::mat4 Rx, Ry, Rz;
		//Rx = glm::rotate(a, glm::vec3(1.0f, 0.0f, 0.0f));
		//Ry = glm::rotate(b, glm::vec3(0.0f, 1.0f, 0.0f));
		//Rz = glm::rotate(g, glm::vec3(0.0f, 0.0f, 1.0f));
		//T = Rz * Ry * Rx;
		//T[3][0] = params.ptr<float>(3)[0];
		//T[3][1] = params.ptr<float>(4)[0];
		//T[3][2] = params.ptr<float>(5)[0];
	}

	void PointToPlaneICP::calc(
		const int level,
		const rgbd::Frame &prevFrame,
		const rgbd::Frame &currFrame,
		glm::mat4 &T,
		const float finThresh
	)
	{
		glm::mat4 dT;
		for (int loop = 0; loop < ICPConstParam::MAX_ITR_NUM[level]; ++loop)
		{
			virtualFrameRenderer.execute(level, currFrame, T);

			cv::Mat assocData(7, 6, CV_32F);
			int elemNum = dataAssoc.execute(
				level, prevFrame,
				virtualFrameRenderer.getVirtualVertexMap(),
				virtualFrameRenderer.getVirtualNormalMap(),
				(float *)assocData.data
			);

			if (elemNum >= 6)
			{
				cv::Mat A(assocData, cv::Rect(0, 0, 6, 6));
				cv::Mat b(assocData, cv::Rect(0, 6, 6, 1));
				cv::Mat x;
				bool bSolved = cv::solve(A, b.t(), x, cv::DECOMP_QR);

				paramToMat(x, dT);
				T = dT * T;

				float diff = (float)cv::norm(x, cv::NORM_L2);
				if (diff < finThresh) break;
			}
			else
			{
				break;
			}
		}
	}
}