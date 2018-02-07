#include "ClibratedProjectionMatrix.h"

namespace rgbd
{
	glm::mat4 calibratedPerspective(
		float nearPlane, float farPlane,
		int w, int h,
		float cx, float cy,
		float fx, float fy,
		float skew,
		bool yDown
	)
	{
		glm::mat4 projMat;

		projMat[0][0] = float(2.0 * fx / w);
		projMat[1][0] = float(-2.0 * skew / w);
		projMat[2][0] = float(w - 2.0 * cx) / w;
		projMat[3][0] = 0.0f;

		projMat[0][1] = 0.0f;
		if (yDown)
		{
			projMat[1][1] = float(2.0 * fy / h);
			projMat[2][1] = float(-h + 2.0 * cy) / h;
		}
		else
		{
			projMat[1][1] = float(-2.0 * fy / h);
			projMat[2][1] = float(h - 2.0 * cy) / h;
		}
		projMat[3][1] = 0.0f;

		projMat[0][2] = 0.0f;
		projMat[1][2] = 0.0f;
		projMat[2][2] = (-farPlane - nearPlane) / (farPlane - nearPlane);
		projMat[3][2] = float(-2.0 * farPlane * nearPlane / (farPlane - nearPlane));

		projMat[0][3] = 0.0f;
		projMat[1][3] = 0.0f;
		projMat[2][3] = -1.0f;
		projMat[3][3] = 0.0f;

		return projMat;
	}
}