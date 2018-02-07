#pragma once

#include <memory>
#include <GL/glew.h>

namespace gl
{
	enum class TextureType
	{
		COLOR,
		SRGB,
		DEPTH,
		DEPTH_STENCIL,
		STENCIL,
		FLOAT16,
		FLOAT32
	};

	enum class TextureFilter
	{
		NEAREST,
		LINEAR
	};

	enum class TextureWarp
	{
		REPEAT,
		MIRRORED_REPEAT,
		CLAMP_TO_EDGE,
		CLAM_TO_BORDER
	};

	class Texture
	{
	private:
		int width;
		int height;

		GLuint id;

		GLint internalFormat;
		GLenum format;
		GLenum dataType;

		GLint getInternalFormat(TextureType type, int channels);
		GLenum getFormat(TextureType type, int channels, bool invertChannels);

	public:
		Texture();
		~Texture();

		void create(const void* data, int width, int height, int channels, TextureType type, bool invertChannels = false);
		void update(const void* data);

		void setFiltering(TextureFilter filter);
		void setWarp(TextureWarp warp);

		void read(const void* data);
		void read(const void* data, GLenum format, GLenum dataType);

		void bind();
		void unbind();
		void bindImage(int idx, GLenum access);
		void bindImage(int idx, GLenum access, GLenum internalFormat);

		void use(int idx = 0);
		
		int getWidth() const;
		int getHeight() const;
		int getID() const;

		typedef std::shared_ptr<Texture> Ptr;
	};
}