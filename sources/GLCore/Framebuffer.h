#pragma once

#include <vector>
#include <map>
#include <GL/glew.h>
#include "Texture.h"

namespace gl
{
	class Framebuffer
	{
	private:
		int width;
		int height;

		GLuint id;

		std::map<unsigned int, gl::Texture::Ptr> attachments;
		gl::Texture::Ptr zbuffer;

	public:
		Framebuffer();
		~Framebuffer();

		void create(int width, int height);
		void attach(gl::Texture::Ptr texture, int idx);

		void bind();
		void unbind();

		int getWidth() const;
		int getHeight() const;

		gl::Texture::Ptr getColorAttachment(unsigned int idx = 0) const;
		gl::Texture::Ptr getDepthAttachment() const;

		std::vector<GLenum> getDrawBuffers();

		typedef std::shared_ptr<gl::Framebuffer> Ptr;
	};
}