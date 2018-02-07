#pragma once

#include <iostream>
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GLCore/Texture.h"
#include "ComputeShader.h"
#include "BilateralFilter.h"
#include "CalcVertexMap.h"
#include "CalcNormalMap.h"
#include "CalcNormalMap.h"
#include "DownSampling.h"

namespace rgbd
{
	enum FRAME
	{
		VIRTUAL, CURRENT
	};

	struct FrameData
	{
		gl::Texture::Ptr colorMap;
		gl::Texture::Ptr depthMap;
		gl::Texture::Ptr vertexMap;
		gl::Texture::Ptr normalMap;

		typedef std::shared_ptr<rgbd::FrameData> Ptr;
	};

	class Frame
	{
	private:
		int width;
		int height;

		gl::Texture::Ptr rawDepthMap;
		std::vector<rgbd::FrameData> frameData;

		rgbd::ComputeShader::Ptr bilateralFilter;
		rgbd::ComputeShader::Ptr vertexMapProc;
		rgbd::ComputeShader::Ptr normalMapProc;
		std::vector<rgbd::ComputeShader::Ptr> downSampling;

	public:
		Frame();
		~Frame();

		void create(
			int width,
			int height,
			int maxLevel,
			float minDepth,
			float maxDepth,
			const glm::mat3 &K,
			std::map<std::string, const gl::Shader::Ptr> &progs
		);

		// for the current frame
		void update(
			const void *colorData,
			const void *depthData,
			float bfSigma = 15.0f,
			float bfDSigma = 0.05f
		) const;
		// for the synthetic frame
		void update() const;

		int getWidth(int lv = 0) const;
		int getHeight(int lv = 0) const;

		gl::Texture::Ptr getColorMap(int lv = 0) const;
		gl::Texture::Ptr getDepthMap(int lv = 0) const;
		gl::Texture::Ptr getVertexMap(int lv = 0) const;
		gl::Texture::Ptr getNormalMap(int lv = 0) const;

		typedef std::shared_ptr<rgbd::Frame> Ptr;
	};
}