#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

std::string LoadFile(const char* filename);
GLuint CompileShader(const char* source, GLenum shaderType);
GLuint CompileProgram(GLuint vertexShader, GLuint fragmentShader);
GLuint LoadShadersProgram(const char* vertexShaderFile, const char* fragmetnShaderFile);

class Shader
{
public:
	GLuint ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();
	// Utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setMat3(const std::string& name, glm::mat3 matrix) const;
	void setMat4(const std::string& name, glm::mat4 matrix) const;
};