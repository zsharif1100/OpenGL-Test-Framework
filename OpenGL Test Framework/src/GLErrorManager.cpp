#include "GLErrorManager.h"
#include <GL/glew.h>
#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogError(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError()) {
		std::string message;
		switch (error) {
		case GL_INVALID_ENUM:
			message = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			message = "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			message = "GL_INVALID_OPERATION";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			message = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		case GL_OUT_OF_MEMORY:
			message = "GL_OUT_OF_MEMORY";
			break;
		case GL_STACK_UNDERFLOW:
			message = "GL_STACK_UNDERFLOW";
			break;
		case GL_STACK_OVERFLOW:
			message = "GL_STACK_OVERFLOW";
			break;
		default:
			message = "GL_NO_ERROR";
			break;
		}

		std::cout << "[OpenGL Error]: (" << message << ")\n\tFunc: " << function << "\n\tFile: " << file << "\n\tLine: " << line << std::endl;
		return false;
	}
	return true;
}