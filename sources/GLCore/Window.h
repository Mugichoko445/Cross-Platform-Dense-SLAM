#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace gl
{
	class Window
	{
	protected:
		GLFWwindow *window;
		int width;
		int height;
		const std::string name;

		int keyID;
		double xOffset;
		double yOffset;

	public:
		Window(
			int width,
			int height,
			const std::string &name
		);
		~Window();

		void clearBuffers(
			float r, float g, float b, float a
		);
		void swapBuffers();

		virtual void mainLoop() = 0;

		void setCallbacks()
		{
			glfwSetWindowUserPointer(window, this);
			glfwSetErrorCallback(errorCallback);
			glfwSetKeyCallback(window, keyCallback);
			glfwSetScrollCallback(window, scrollCallback);
		}
		
		static void errorCallback(
			int errorCode,
			const char *description
		)
		{
			std::cerr << *description << std::endl;
		}

		static void keyCallback(
			GLFWwindow* window,
			int key,
			int scancode,
			int action,
			int mods
		)
		{
			gl::Window* _this = static_cast<gl::Window*>(glfwGetWindowUserPointer(window));
			_this->keyID = key;

			if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
			{
				glfwSetWindowShouldClose(window, GL_TRUE);
			}
		}

		static void scrollCallback(
			GLFWwindow* window,
			double xOffset,
			double yOffset
		)
		{
			gl::Window* _this = static_cast<gl::Window*>(glfwGetWindowUserPointer(window));

			_this->xOffset = xOffset;
			_this->yOffset = yOffset;
		}
	};
}