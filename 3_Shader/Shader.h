#pragma once
#include "glad/glad.h"
#include <iostream>

class Shader
{
public:
	Shader();
	~Shader();
	bool InitShader(const char* vertexPath, const char* fragmentPath);
	void UseProgram();

	// uniform
	void SetFloat(const std::string &name, float value) const;
private:
	bool GetVertexCode(const char* vertexPath);
	bool GetFragmentCode(const char* fragmentPath);
	bool compileShader();
	bool checkCompileError(unsigned int shader, std::string type);
private:
	unsigned int shaderID = 0;
	std::string vertexCode;
	std::string fragmentCode;
};
