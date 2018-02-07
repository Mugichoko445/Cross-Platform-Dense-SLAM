#pragma once

#include <array>
#include <glm/glm.hpp>
#include "SLAM/Frame/Frame.h"
#include "GLCore/Framebuffer.h"
#include "GLCore/Shader.h"
#include "GLCore/Texture.h"
#include "ConstantParameters.h"
#include "SLAM/Utilities/ClibratedProjectionMatrix.h"

namespace rgbd {
	class VirtualFrameRenderer
	{
	private:
		gl::Framebuffer virtualFrameFBO;
		gl::Texture::Ptr vertexMap;
		gl::Texture::Ptr normalMap;

		const gl::Shader::Ptr prog;

	public:
		VirtualFrameRenderer(
			int width,
			int height,
			const glm::mat3 &K,
			const gl::Shader::Ptr prog
		);
		~VirtualFrameRenderer();

		gl::Texture::Ptr getVirtualVertexMap();
		gl::Texture::Ptr getVirtualNormalMap();

		void execute(
			int level,
			const rgbd::Frame &srcFrame,
			const glm::mat4 &T
		);
	};
}