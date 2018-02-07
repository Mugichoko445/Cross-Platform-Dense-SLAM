#pragma once

#include <array>
#include <map>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "GLCore/Framebuffer.h"
#include "GLCore/Buffer.h"
#include "GLCore/Shader.h"
#include "GLCore/Texture.h"
#include "SLAM/Frame/Frame.h"
#include "SLAM/ICP/ConstantParameters.h"
#include "SLAM/Utilities/ClibratedProjectionMatrix.h"

namespace rgbd
{
	struct GlobalMapConstParam
	{
		static const unsigned int MAX_MAP_SIZE;
		static const float CSTABLE;
	};

	struct GlobalMapData
	{
		glm::vec4 data;
		glm::vec4 vertex;
		glm::vec4 normal;
		glm::vec4 color;

		GlobalMapData()
		{
			data = glm::vec4(-1.0f);	// Confidence, radius, timestamp, and empty data
			vertex = glm::vec4(0.0f);	// Note: glm::vec3 didn't work
			normal = glm::vec4(0.0f);	// Note: glm::vec3 didn't work
			color = glm::vec4(0.0f);	// Note: glm::vec3 didn't work
		}
	};

	class GlobalMap
	{
	private:
		const int width;
		const int height;

		GLuint mapSize;
		int buffSwitch;

		std::map<std::string, const gl::Shader::Ptr> progs;

		gl::Framebuffer indexMapFBO;
		gl::Framebuffer virtualFrameFBO;

		std::array<gl::AtomicCounterBuffer, 2> atomic;
		std::array<gl::ShaderStorageBuffer<GlobalMapData>, 2> ssbo;

	public:
		GlobalMap(
			int width,
			int height,
			const glm::mat3 &K,
			const std::map<std::string, const gl::Shader::Ptr> &progs
		);
		~GlobalMap();

		void genIndexMap(
			const glm::mat4 &invT
		);
		void updateGlobalMap(
			const rgbd::Frame &srcFrame,
			const glm::mat4 &T,
			const int timestamp
		);
		void removeUnnecessaryPoints(
			int timestamp
		);
		void genVirtualFrame(
			const rgbd::Frame &dstFrame,
			const glm::mat4 &invT
		);

		GLuint getMapSize();

		typedef std::shared_ptr<rgbd::GlobalMap> Ptr;
	};
}