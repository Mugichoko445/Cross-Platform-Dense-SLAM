#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "SLAM/Frame/Frame.h"
#include "GLCore/Buffer.h"
#include "GLCore/Shader.h"
#include "GLCore/Texture.h"
#include "ConstantParameters.h"

namespace rgbd
{
	struct ICPProjectiveData
	{
		float A[6];
		float b;
	};

	class ProjectiveDataAssoc
	{
	private:
		const int width;
		const int height;

		std::map<std::string, const gl::Shader::Ptr> progs;

		gl::Texture assocDataTex;
		gl::AtomicCounterBuffer atomic;
		gl::ShaderStorageBuffer<ICPProjectiveData> ssbo;

	public:
		ProjectiveDataAssoc(
			int width,
			int height,
			const std::map<std::string, const gl::Shader::Ptr> progs
		);
		~ProjectiveDataAssoc();

		GLuint execute(
			int level,
			const rgbd::Frame &prevFrame,
			gl::Texture::Ptr virtualVertexMap,
			gl::Texture::Ptr virtualNormalMap,
			float *assocData
		);

		int getWidth();
		int getHeight();
	};
}