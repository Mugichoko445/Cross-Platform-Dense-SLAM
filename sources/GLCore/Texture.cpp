#include "Texture.h"

namespace gl
{
	GLint Texture::getInternalFormat(TextureType type, int channels)
	{
		GLint internalFormat = GL_RGB8;
		switch (type)
		{
		case TextureType::COLOR:
			switch (channels)
			{
			case 1:	internalFormat = GL_R8; break;
			case 2:	internalFormat = GL_RG8; break;
			case 3:	internalFormat = GL_RGB8; break;
			case 4:	internalFormat = GL_RGBA8; break;
			}
			break;
		case TextureType::SRGB:
			switch (channels)
			{
			case 3:	internalFormat = GL_SRGB8; break;
			case 4:	internalFormat = GL_SRGB8_ALPHA8; break;
			default: internalFormat = GL_SRGB; break;
			}
			break;
		case TextureType::DEPTH:
			switch (channels)
			{
			case 2:	internalFormat = GL_DEPTH_COMPONENT16; break;
			case 3:	internalFormat = GL_DEPTH_COMPONENT24; break;
			case 4:	internalFormat = GL_DEPTH_COMPONENT32; break;
			default: internalFormat = GL_DEPTH_COMPONENT; break;
			}
			break;
		case TextureType::DEPTH_STENCIL:
			switch (channels)
			{
			case 3:	internalFormat = GL_DEPTH24_STENCIL8; break;
			case 4:	internalFormat = GL_DEPTH32F_STENCIL8; break;
			default: internalFormat = GL_DEPTH_STENCIL; break;
			}
			break;
		case TextureType::STENCIL:
			switch (channels)
			{
			case 1:	internalFormat = GL_STENCIL_INDEX8; break;
			case 2:	internalFormat = GL_STENCIL_INDEX16; break;
			default: internalFormat = GL_STENCIL_INDEX; break;
			}
			break;
		case TextureType::FLOAT16:
			switch (channels)
			{
			case 1: internalFormat = GL_R16F; break;
			case 2:	internalFormat = GL_RG16F; break;
			case 3:	internalFormat = GL_RGB16F; break;
			case 4:	internalFormat = GL_RGBA16F; break;
			}
			break;
		case TextureType::FLOAT32:
			switch (channels)
			{
			case 1: internalFormat = GL_R32F; break;
			case 2:	internalFormat = GL_RG32F; break;
			case 3:	internalFormat = GL_RGB32F; break;
			case 4:	internalFormat = GL_RGBA32F; break;
			}
			break;
		}

		return internalFormat;
	}

	GLenum Texture::getFormat(TextureType type, int channels, bool invertChannels)
	{
		GLenum format = GL_RGB;

		switch (type)
		{
		case TextureType::DEPTH:
			format = GL_DEPTH_COMPONENT;
			break;
		case TextureType::DEPTH_STENCIL:
			format = GL_DEPTH_STENCIL;
			break;
		case TextureType::STENCIL:
			format = GL_STENCIL_INDEX;
			break;
		default:
			if (invertChannels)
			{
				switch (channels)
				{
				case 1:	format = GL_RED; break;
				case 2:	format = GL_RG; break;
				case 3:	format = GL_BGR; break;
				case 4:	format = GL_BGRA; break;
				}
			}
			else
			{
				switch (channels)
				{
				case 1:	format = GL_RED; break;
				case 2:	format = GL_RG; break;
				case 3:	format = GL_RGB; break;
				case 4:	format = GL_RGBA; break;
				}
			}
			break;
		}

		return format;
	}


	Texture::Texture()
	{
		glGenTextures(1, &id);
	}


	Texture::~Texture()
	{
		glDeleteTextures(1, &id);
	}

	void Texture::create(const void* data, int width, int height, int channels, TextureType type, bool invertChannels)
	{
		this->width = width;
		this->height = height;

		internalFormat = getInternalFormat(type, channels);
		format = getFormat(type, channels, invertChannels);
		if (type == TextureType::DEPTH || type == TextureType::FLOAT16 || type == TextureType::FLOAT32)
		{
			dataType = GL_FLOAT;
		}
		else
		{
			dataType = GL_UNSIGNED_BYTE;
		}

		bind();
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, dataType, data);
		unbind();
	}

	void Texture::update(const void* data)
	{
		glBindTexture(GL_TEXTURE_2D, id);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, dataType, data);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::setFiltering(TextureFilter filter)
	{
		GLint minFilter = GL_NEAREST;
		GLint maxFilter = GL_NEAREST;

		switch (filter)
		{
		case TextureFilter::NEAREST:
			minFilter = GL_NEAREST;
			maxFilter = GL_NEAREST;
			break;
		case TextureFilter::LINEAR:
			minFilter = GL_LINEAR;
			maxFilter = GL_LINEAR;
			break;
		}

		bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);
		unbind();
	}

	void Texture::setWarp(TextureWarp warp)
	{
		GLint warpMode;

		switch (warp)
		{
		case TextureWarp::REPEAT:
			warpMode = GL_REPEAT;
			break;
		case TextureWarp::MIRRORED_REPEAT:
			warpMode = GL_MIRRORED_REPEAT;
			break;
		case TextureWarp::CLAMP_TO_EDGE:
			warpMode = GL_CLAMP_TO_EDGE;
			break;
		case TextureWarp::CLAM_TO_BORDER:
			warpMode = GL_CLAMP_TO_BORDER;
			break;
		}

		bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warpMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warpMode);
		unbind();
	}
	
	void Texture::read(const void* data)
	{
		bind();
		glGetTexImage(GL_TEXTURE_2D, 0, format, dataType, (GLvoid*)data);
		unbind();
	}

	void Texture::read(const void* data, GLenum format, GLenum dataType)
	{
		bind();
		glGetTexImage(GL_TEXTURE_2D, 0, format, dataType, (GLvoid*)data);
		unbind();
	}

	void Texture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void Texture::unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::bindImage(int idx, GLenum access)
	{
		glBindImageTexture(idx, id, 0, GL_FALSE, 0, access, internalFormat);
	}

	void Texture::bindImage(int idx, GLenum access, GLenum internalFormat)
	{
		glBindImageTexture(idx, id, 0, GL_FALSE, 0, access, internalFormat);
	}

	void Texture::use(int idx)
	{
		glActiveTexture(GL_TEXTURE0 + idx);
		bind();
	}

	int Texture::getWidth() const
	{
		return width;
	}

	int Texture::getHeight() const
	{
		return height;
	}
	
	int Texture::getID() const
	{
		return id;
	}

}