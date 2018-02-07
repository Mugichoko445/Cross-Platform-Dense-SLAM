#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "GLCore/Buffer.h"
#include "GLCore/Texture.h"
#include "GLCore/VertexArray.h"

namespace gl
{
	class Quad
	{
	private:
		gl::VertexBuffer<GLfloat> vertices;
		gl::IndexBuffer<GLuint> indices;
		gl::VertexArray vao;

	public:
		Quad();
		~Quad();

		void render(gl::Texture::Ptr tex);
	};
}