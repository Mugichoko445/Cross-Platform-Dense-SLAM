#include "Quad.h"

namespace gl
{
	Quad::Quad()
	{
		std::vector<GLfloat> vert =
		{
			+1.0f, +1.0f, 0.0f, /* Upper right */ -1.0f, +1.0f, 0.0f, /* Upper left */
			-1.0f, -1.0f, 0.0f, /* Lower left */  +1.0f, -1.0f, 0.0f, /* Lower right */
			1.0f, 1.0f, /* Upper right */ 0.0f, 1.0f, /* Upper left */
			0.0f, 0.0f, /* Lower left */  1.0f, 0.0f  /* Lower right */
		};
		std::vector<GLshort> index =
		{
			0, 1, 2, 3
		};

		vertices.create(vert.data(), (int)vert.size(), GL_STATIC_READ);
		vao.addVertexAttrib(0, 3, vertices, 0, 0);
		vao.addVertexAttrib(1, 2, vertices, 0, (void *)(sizeof(GLfloat) * 3 * 4));

		indices.create(index.data(), (int)index.size(), GL_STATIC_READ);
		vao.bind();
		indices.bind();
		vao.unbind();
	}

	Quad::~Quad()
	{
	}

	void Quad::render(gl::Texture::Ptr tex)
	{
		vao.bind();
		tex->use();
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, 0);
		vao.unbind();
	}
}