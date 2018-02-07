#pragma once

#include <array>

namespace rgbd
{
	struct ICPConstParam
	{
		static const float MIN_DEPTH;	// 0.1 meter
		static const float MAX_DEPTH;	// 4.0 meter

		static const float DST_THRESH;
		static const float ANG_THRESH;

		static const int MAX_LEVEL;		// lv. 1 is the original and lv. 3 is the coarsest (Ref: KinectFusion)
		static const int MAX_ITR_NUM[];	// lv. 1: 4, lv. 2: 5, and lv. 3: 10
	};
}