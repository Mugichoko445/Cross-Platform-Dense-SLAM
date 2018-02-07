#include "Window.h"

namespace gl
{
	Window::Window(
		int width,
		int height,
		const std::string &name
	) : name(name)
	{
		this->width = width;
		this->height = height;

		try
		{
			if (!glfwInit())
			{
				throw "Failed to initialize GLFW 3...";
			}
			window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
			if (!window)
			{
				glfwTerminate();
				throw "Failed to create a window...";
			}
		}
		catch (std::string e)
		{
			std::cerr << "Error: " << e.c_str() << std::endl;
			exit(EXIT_FAILURE);
		}

		glfwMakeContextCurrent(window);
		setCallbacks();

		try
		{
			if (glewInit() != GLEW_OK)
			{
				glfwTerminate();
				throw "Failed to initialize GLEW...";
			}
		}
		catch (std::string e)
		{
			std::cerr << "Error: " << e.c_str() << std::endl;
			exit(EXIT_FAILURE);
		}

		glEnable(GL_DEPTH_TEST);

		std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		std::cout << std::endl;
	}

	Window::~Window()
	{
		if (window)
		{
			glfwTerminate();
		}
	}

	void Window::clearBuffers(
		float r, float g, float b, float a
	)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::swapBuffers()
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}