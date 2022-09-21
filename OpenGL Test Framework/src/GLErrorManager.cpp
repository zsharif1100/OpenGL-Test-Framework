#include "GLErrorManager.h"
#include <GL/glew.h>
#include <iostream>

#include <map>

static const std::map<GLenum, std::string> codeToMessage = {
	{GL_INVALID_ENUM, "GL_INVALID_ENUM"},
	{GL_INVALID_VALUE, "GL_INVALID_VALUE"},
	{GL_INVALID_OPERATION, "GL_INVALID_OPERATION"},
	{GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION"},
	{GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY"},
	{GL_STACK_UNDERFLOW, "GL_STACK_UNDERFLOW"},
	{GL_STACK_OVERFLOW, "GL_STACK_OVERFLOW"}
};

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogError(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError()) {
		std::string message = codeToMessage.at(error);

		std::cout << "[OpenGL Error]: (" << message << ")\n\tFunc: " << function << "\n\tFile: " << file << "\n\tLine: " << line << std::endl;
		return false;
	}
	return true;
}