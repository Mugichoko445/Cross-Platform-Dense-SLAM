#pragma once

#include <glm/glm.hpp>

namespace rgbd
{
	glm::mat4 calibratedPerspective(
		float nearPlane, float farPlane,
		int w, int h,
		float cx, float cy,
		float fx, float fy,
		float skew,
		bool yDown = true
	);
}