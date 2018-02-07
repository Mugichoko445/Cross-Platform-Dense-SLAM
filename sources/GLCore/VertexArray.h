#pragma once

#include "GL/glew.h"

namespace gl
{
	class VertexArray
	{
	private:
		GLuint id;

		VertexArray(const VertexArray&);
		VertexArray& operator =(const VertexArray&);

	public:
		VertexArray()
		{
			glGenVertexArrays(1, &id);
		}

		~VertexArray()
		{
			glDeleteVertexArrays(1, &id);
		}

		void addVertexAttrib(int index, int size, int bufferID, int stride, const void* pointer, GLenum type = GL_FLOAT)
		{
			glBindVertexArray(id);
			glEnableVertexAttribArray(index);
			glBindBuffer(GL_ARRAY_BUFFER, bufferID);
			glVertexAttribPointer(index, size, type, GL_FALSE, stride, pointer);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		void removeVertexAttrib(int index)
		{
			glBindVertexArray(id);
			glDisableVertexAttribArray(index);
			glBindVertexArray(0);
		}

		void bind()
		{
			glBindVertexArray(id);
		}

		void unbind()
		{
			glBindVertexArray(0);
		}
	};
}
