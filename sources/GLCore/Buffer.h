#pragma once

#include <GL/glew.h>

namespace gl
{
	template<int BufferType, typename DataType>
	class Buffer
	{
	private:
		GLuint id;

		Buffer(const Buffer&) = delete;

	public:
		Buffer()
		{
			glGenBuffers(1, &id);
		}

		~Buffer()
		{
			glDeleteBuffers(1, &id);
		}

		void create(const void* data, int size, GLenum usage = GL_STATIC_DRAW)
		{
			glBindBuffer(BufferType, id);
			glBufferData(BufferType, size * sizeof(DataType), data, usage);
			glBindBuffer(BufferType, 0);
		}

		void update(const void* data, int offset, int size)
		{
			glBindBuffer(BufferType, id);
			glBufferSubData(BufferType, offset, size * sizeof(DataType), data);
			glBindBuffer(BufferType, 0);
		}

		void read(void* data, int offset, int size)
		{
			glBindBuffer(BufferType, id);
			glGetBufferSubData(BufferType, offset, size * sizeof(DataType), data);
			glBindBuffer(BufferType, 0);
		}

		void bindRange(GLuint index, GLintptr offset = 0)
		{
			glBindBufferRange(BufferType, index, id, offset, sizeof(DataType));
		}

		void bindBase(GLuint index)
		{
			glBindBufferBase(BufferType, index, id);
		}

		void bind()
		{
			glBindBuffer(BufferType, id);
		}

		void unbind()
		{
			glBindBuffer(BufferType, 0);
		}

		operator GLuint() const
		{
			return id;
		}
	};

	template<typename DataType>
	class VertexBuffer : public Buffer<GL_ARRAY_BUFFER, DataType> {};

	template<typename DataType>
	class IndexBuffer : public Buffer<GL_ELEMENT_ARRAY_BUFFER, DataType> {};

	template<typename DataType>
	class UniformBuffer : public Buffer<GL_UNIFORM_BUFFER, DataType> {};

	template<typename DataType>
	class ShaderStorageBuffer : public Buffer<GL_SHADER_STORAGE_BUFFER, DataType> {};

	class AtomicCounterBuffer : public Buffer<GL_ATOMIC_COUNTER_BUFFER, GLuint> {};
}