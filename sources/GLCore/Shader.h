#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gl
{
	class Shader
	{
	private:
		GLuint prog;
		std::map<std::string, GLint> uniforms;

		Shader(const Shader&) = delete;

		void load(const std::string &fileName, std::stringstream &codes);
		GLuint createShader(GLenum shaderType, const std::string &shaderFile);
		GLuint createProgram(const std::vector<GLuint> &shaderList);

		void loadUniforms();
		GLint getUniformLoc(const std::string &name);

	public:
		Shader(const std::string &vsFileName, const std::string &fsFileName, const std::string &gsFileName = "");
		Shader(const std::string &csFileName);
		~Shader();

		void init(const std::string &vsFileName, const std::string &fsFileName, const std::string &gsFileName = "");
		void init(const std::string &csFileName);

		void use();
		void disuse();

		template<typename T>
		void setUniform(const std::string &name, const T& value);

		typedef std::shared_ptr<Shader> Ptr;
	};
}