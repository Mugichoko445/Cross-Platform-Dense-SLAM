#include "Shader.h"

namespace gl
{
	void Shader::load(const std::string &fileName, std::stringstream &codes)
	{
		std::ifstream ifs(fileName);
		
		if (!ifs.is_open())
		{
			throw "  There is no such file: " + fileName;
		}
		else
		{
			std::cout << "  Loaded \"" << fileName << "\"." << std::endl;
		}

		std::string buffer;
		codes = std::stringstream("");
		while (ifs && getline(ifs, buffer))
		{
			codes << buffer << std::endl;
		}
	}
	
	GLuint Shader::createShader(GLenum shaderType, const std::string &shaderFile)
	{
		GLuint shader = glCreateShader(shaderType);

		const char *shaderFileData = shaderFile.c_str();
		glShaderSource(shader, 1, &shaderFileData, NULL);

		glCompileShader(shader);

		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint infoLogLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

			GLchar *infoLog = new GLchar[infoLogLength + 1];
			glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);

			std::string strShaderType;
			switch (shaderType)
			{
			case GL_VERTEX_SHADER:
				strShaderType = "vertex";
				break;
			case GL_GEOMETRY_SHADER:
				strShaderType = "geometry";
				break;
			case GL_FRAGMENT_SHADER:
				strShaderType = "fragment";
				break;
			case GL_COMPUTE_SHADER:
				strShaderType = "compute";
				break;
			}

			std::string errMsg("Failed to compile \"" + strShaderType + "\" shader: \n" + infoLog);
			delete[] infoLog;

			throw errMsg;
		}
		else
		{
			std::string strShaderType;
			switch (shaderType)
			{
			case GL_VERTEX_SHADER:
				strShaderType = "vertex";
				break;
			case GL_GEOMETRY_SHADER:
				strShaderType = "geometry";
				break;
			case GL_FRAGMENT_SHADER:
				strShaderType = "fragment";
				break;
			case GL_COMPUTE_SHADER:
				strShaderType = "compute";
				break;
			}

			std::cout << "  Successfully compiled \"" << strShaderType << "\" shader." << std::endl;
		}

		return shader;
	}
	
	GLuint Shader::createProgram(const std::vector<GLuint> &shaderList)
	{
		GLuint program = glCreateProgram();

		for (int idx = 0; idx < shaderList.size(); ++idx)
		{
			glAttachShader(program, shaderList[idx]);
		}

		glLinkProgram(program);

		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint infoLogLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

			GLchar *infoLog = new GLchar[infoLogLength + 1];
			glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
			std::string errMsg("Linker failure: " + std::string(infoLog));
			delete[] infoLog;

			throw errMsg;
		}
		else
		{
			std::cout << "  Linked shader program to the program successfully." << std::endl;
		}

		for (int idx = 0; idx < shaderList.size(); ++idx)
		{
			glDetachShader(program, shaderList[idx]);
		}

		return program;
	}

	void Shader::loadUniforms()
	{
		GLint numUniforms;
		glGetProgramiv(prog, GL_ACTIVE_UNIFORMS, &numUniforms);

		GLchar buffer[128];
		GLsizei length;
		GLint size;
		GLenum type;

		for (GLint i = 0; i < numUniforms; ++i)
		{
			glGetActiveUniform(prog, i, 128, &length, &size, &type, buffer);

			std::string name(buffer);
			uniforms[name] = glGetUniformLocation(prog, name.c_str());

			//std::cout << name << std::endl;
		}
	}

	GLint Shader::getUniformLoc(const std::string &name)
	{
		if (uniforms.find(name) != uniforms.end())
		{
			return uniforms[name];
		}

		return -1;
	}

	Shader::Shader(const std::string &vsFileName, const std::string &fsFileName, const std::string &gsFileName)
	{
		init(vsFileName, fsFileName, gsFileName);
	}

	Shader::Shader(const std::string &csFileName)
	{
		init(csFileName);
	}

	Shader::~Shader() {}

	void Shader::init(const std::string &vsFileName, const std::string &fsFileName, const std::string &gsFileName)
	{
		std::cout << "Creating a shader program..." << std::endl;
		std::vector<GLuint> shaderList;
		std::stringstream ssVertexShader, ssFragmentShader, ssGeometryShader;

		try
		{
			load(vsFileName, ssVertexShader);
			load(fsFileName, ssFragmentShader);
			if (!gsFileName.empty())
			{
				load(gsFileName, ssGeometryShader);
			}

			shaderList.push_back(createShader(GL_VERTEX_SHADER, ssVertexShader.str()));
			shaderList.push_back(createShader(GL_FRAGMENT_SHADER, ssFragmentShader.str()));
			if (!gsFileName.empty())
			{
				shaderList.push_back(createShader(GL_GEOMETRY_SHADER, ssGeometryShader.str()));
			}

			prog = createProgram(shaderList);
		}
		catch (std::string e)
		{
			std::cerr << "  Error: " << e << std::endl;
			exit(EXIT_FAILURE);
		}

		std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

		loadUniforms();

		std::cout << "done!" << std::endl;
	}

	void Shader::init(const std::string &csFileName)
	{
		std::cout << "Creating a shader program..." << std::endl;
		std::vector<GLuint> shaderList;

		try
		{
			std::stringstream ssComputeShader;
			load(csFileName, ssComputeShader);
			shaderList.push_back(createShader(GL_COMPUTE_SHADER, ssComputeShader.str()));

			prog = createProgram(shaderList);
		}
		catch (std::string e)
		{
			std::cerr << "  Error: " << e << std::endl;
			exit(EXIT_FAILURE);
		}

		std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);

		loadUniforms();

		std::cout << "done!" << std::endl;
	}

	void Shader::use()
	{
		glUseProgram(prog);
	}

	void Shader::disuse()
	{
		glUseProgram(0);
	}

	template<> void Shader::setUniform<GLfloat>(const std::string &name, const GLfloat &value)
	{
		glProgramUniform1f(prog, getUniformLoc(name), value);
	}
	template<> void Shader::setUniform<GLint>(const std::string &name, const GLint &value)
	{
		glProgramUniform1i(prog, getUniformLoc(name), value);
	}
	template<> void Shader::setUniform<GLuint>(const std::string &name, const GLuint &value)
	{
		glProgramUniform1ui(prog, getUniformLoc(name), value);
	}
	template<> void Shader::setUniform<glm::mat3>(const std::string &name, const glm::mat3 &value)
	{
		glProgramUniformMatrix3fv(prog, getUniformLoc(name), 1, GL_FALSE, glm::value_ptr(value));
	}
	template<> void Shader::setUniform<glm::mat4>(const std::string &name, const glm::mat4 &value)
	{
		glProgramUniformMatrix4fv(prog, getUniformLoc(name), 1, GL_FALSE, glm::value_ptr(value));
	}
}