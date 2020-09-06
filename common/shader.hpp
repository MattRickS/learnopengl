#include <string>
#include <glad/glad.h>

std::string LoadFile(const char* filename);
GLuint CompileShader(const char* source, GLenum shaderType);
GLuint CompileProgram(GLuint vertexShader, GLuint fragmentShader);
GLuint LoadShadersProgram(const char* vertexShaderFile, const char* fragmetnShaderFile);
