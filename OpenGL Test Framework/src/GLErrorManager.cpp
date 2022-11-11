#include "GLErrorManager.h"
#include <GL/glew.h>
#include <iostream>

#include <map>
#include <random>

static const std::map<GLenum, std::string> codeToMessage = {
	{GL_NO_ERROR, "GL_NO_ERROR"},
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
	GLenum error = glGetError();
	while (error != GL_NO_ERROR) {
		std::cout << codeToMessage.at(error);
		error = glGetError();
	}
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

int getRandomInt(int x, int y)
{
	std::random_device randomSeed;

	std::mt19937 randomGenerator(randomSeed());
	std::uniform_int_distribution<int> distribution(x, y);

	return distribution(randomGenerator);
}
