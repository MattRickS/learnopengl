#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <glad/glad.h>

#include "shader.hpp"

std::string LoadFile(const char* filename)
{
	std::string code;
	std::ifstream stream(filename, std::ios::in);
	if (stream.is_open())
	{
		std::stringstream sstr;
		sstr << stream.rdbuf();
		code = sstr.str();
		stream.close();
	}
	else
	{
		std::cout << "Unable to open " << filename << std::endl;
	}

	return code;
}

GLuint CompileShader(const char* source, GLenum shaderType)
{
	GLuint shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &source, NULL);
	glCompileShader(shaderID);

	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << "Error compiling shader:" << std::endl << infoLog << std::endl;
		return 0;
	}

	return shaderID;
}

GLuint CompileProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);

	GLint success;
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetProgramInfoLog(programID, 512, NULL, infoLog);
		std::cout << "Error linking program:" << std::endl << infoLog << std::endl;
		return 0;
	}

	return programID;
}

// Convenience method for loading a program with a vertex and fragment shader
GLuint LoadShadersProgram(const char* vertexShaderFile, const char* fragmetnShaderFile)
{
	// TODO: Invalid symbols in file - IDE? Windows line endings?
	std::string vertexSource = LoadFile(vertexShaderFile);
	std::string fragmentSource = LoadFile(fragmetnShaderFile);

	GLuint vertexShader = CompileShader(vertexSource.c_str(), GL_VERTEX_SHADER);
	GLuint fragmentShader = CompileShader(fragmentSource.c_str(), GL_FRAGMENT_SHADER);

	GLuint programID = CompileProgram(vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return programID;
}
