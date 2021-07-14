#include "Shader.h"
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader()
{
}

Shader::~Shader()
{
	glDeleteProgram(shaderID);
}

bool Shader::InitShader(const char * vertexPath, const char * fragmentPath)
{
	if (!GetVertexCode(vertexPath))
		return false;
	if (!GetFragmentCode(fragmentPath))
		return false;
	if (!compileShader())
		return false;

	return true;
}

void Shader::UseProgram()
{
	glUseProgram(shaderID);
}

void Shader::SetFloat(const std::string & name, float value) const
{
	glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
}

bool Shader::GetVertexCode(const char * vertexPath)
{
	std::ifstream vShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		std::stringstream vShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		vShaderFile.close();

		vertexCode = vShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << vertexPath << std::endl;
		return false;
	}
	return true;
}

bool Shader::GetFragmentCode(const char * fragmentPath)
{
	std::ifstream fShaderFile;
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		fShaderFile.open(fragmentPath);
		std::stringstream fShaderStream;
		fShaderStream << fShaderFile.rdbuf();
		fShaderFile.close();

		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << fragmentPath << std::endl;
		return false;
	}
	return true;
}

bool Shader::compileShader()
{
	// vertex shader
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	const char* vCode = vertexCode.c_str();
	glShaderSource(vertex, 1, &vCode, 0);
	glCompileShader(vertex);
	if (!checkCompileError(vertex, "SHADER"))
		return false;
	// fragment shader
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fCode = fragmentCode.c_str();
	glShaderSource(fragment, 1, &fCode, 0);
	glCompileShader(fragment);
	if (!checkCompileError(fragment, "SHADER"))
		return false;
	// shader program
	shaderID = glCreateProgram();
	glAttachShader(shaderID, vertex);
	glAttachShader(shaderID, fragment);
	glLinkProgram(shaderID);
	if (!checkCompileError(shaderID, "PROGRAM"))
		return false;
	// delete shader
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	return true;
}

bool Shader::checkCompileError(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	return !!success;
}
