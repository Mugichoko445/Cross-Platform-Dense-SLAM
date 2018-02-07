#pragma once

#include <stdarg.h>
#include <vector>
#include "GLCore/Shader.h"
#include "GLCore/Texture.h"

namespace rgbd
{
	struct ComputeShader
	{
	public:
		ComputeShader() {}
		~ComputeShader() {}
		
		virtual void execute() {}

		typedef std::shared_ptr<ComputeShader> Ptr;
	};
}