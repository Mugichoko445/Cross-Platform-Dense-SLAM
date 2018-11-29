#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "GLCore/Shader.h"

#include "SLAM/Frame/Frame.h"
#include "SLAM/Utilities/IniFileReader.h"
#include "SLAM/Utilities/TUMRGBDUtilities.h"
#include "SLAM/ICP/PyramidricalICP.h"
#include "SLAM/GlobalMap.h"

namespace rgbd
{
	class SLAM
	{
	private:
		rgbd::PyramidricalICP::Ptr icp;
		rgbd::GlobalMap::Ptr gMap;
		std::vector<glm::mat4> vT;

	public:
		SLAM();
		~SLAM();

		void loadShaders(
			std::map<std::string, const gl::Shader::Ptr> &progs,
			const std::string &folderPath = "../../shaders/"
		);

		void init(
			const rgbd::Frame &currentFrame,
			const rgbd::Frame &virtualFrame,
			const glm::mat3 &K,
			const std::map<std::string, const gl::Shader::Ptr> &progs
		);

		glm::mat4 calcDevicePose(
			const rgbd::Frame &currentFrame,
			const rgbd::Frame &virtualFrame
		);

		void updateGlobalMap(
			const rgbd::Frame &currentFrame,
			const rgbd::Frame &virtualFrame
		);
	};
}