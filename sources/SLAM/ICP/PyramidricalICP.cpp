#include "PyramidricalICP.h"

namespace rgbd
{
	PyramidricalICP::PyramidricalICP(
		int width,
		int height,
		const glm::mat3 &K,
		const std::map<std::string, const gl::Shader::Ptr> &progs
	)
	{
		icp.resize(ICPConstParam::MAX_LEVEL);

		for (int lv = 0; lv < icp.size(); ++lv)
		{
			int bias(int(pow(2, lv)));

			glm::mat3 _K;
			_K[0][0] = K[0][0] / bias; _K[1][1] = K[1][1] / bias;
			_K[2][0] = K[2][0] / bias; _K[2][1] = K[2][1] / bias;

			icp[lv] = std::make_shared<rgbd::PointToPlaneICP>(width / bias, height / bias, _K, progs);
		}
	}

	PyramidricalICP::~PyramidricalICP()
	{
	}

	void PyramidricalICP::calc(
		const rgbd::Frame &prevFrame,
		const rgbd::Frame &currFrame,
		glm::mat4 &T
	)
	{
		for (int lv = ICPConstParam::MAX_LEVEL - 1; lv >= 0; --lv)
		{
			icp[lv]->calc(lv, prevFrame, currFrame, T);
		}
	}
}