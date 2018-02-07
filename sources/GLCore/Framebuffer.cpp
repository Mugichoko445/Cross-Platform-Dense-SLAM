#include "Framebuffer.h"

namespace gl
{
	Framebuffer::Framebuffer()
	{
		glGenFramebuffers(1, &id);
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &id);
	}

	void Framebuffer::create(int width, int height)
	{
		this->width = width;
		this->height = height;

		zbuffer = std::make_shared<gl::Texture>();
		zbuffer->create(0, width, height, 1, gl::TextureType::DEPTH);
		zbuffer->setFiltering(gl::TextureFilter::NEAREST);
		zbuffer->setWarp(gl::TextureWarp::CLAMP_TO_EDGE);

		bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, zbuffer->getID(), 0);
		unbind();
	}

	void Framebuffer::attach(gl::Texture::Ptr texture, int idx)
	{
		attachments[idx] = texture;

		bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, GL_TEXTURE_2D, texture->getID(), 0);
		unbind();
	}

	void Framebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}

	void Framebuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	int Framebuffer::getWidth() const
	{
		return width;
	}

	int Framebuffer::getHeight() const
	{
		return height;
	}

	gl::Texture::Ptr Framebuffer::getColorAttachment(unsigned int idx) const
	{
		if (attachments.find(idx) != attachments.end())
		{
			return attachments.at(idx);
		}

		return nullptr;
	}

	gl::Texture::Ptr Framebuffer::getDepthAttachment() const
	{
		return zbuffer;
	}

	std::vector<GLenum> Framebuffer::getDrawBuffers()
	{
		std::vector<GLenum> drawBuffers(attachments.size());

		std::vector<GLenum>::iterator itDrawBuffers = drawBuffers.begin();
		std::map<unsigned int, gl::Texture::Ptr>::iterator itAttachments = attachments.begin();
		while (itAttachments != attachments.end())
		{
			*itDrawBuffers = GL_COLOR_ATTACHMENT0 + itAttachments->first;

			++itDrawBuffers;
			++itAttachments;
		}

		return drawBuffers;
	}
}