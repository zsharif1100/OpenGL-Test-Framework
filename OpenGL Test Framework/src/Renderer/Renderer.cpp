#include "Renderer.h"

#include <GL/glew.h>
#include "GLErrorManager.h"
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct Vertex {
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 TexCoord;
	float TexID;
};

static unsigned int quad_VBO;
static unsigned int quad_VAO;
static unsigned int quad_Shader;

void error_callback(int error, const char* description) {
	fprintf(stderr, "error: %s\n", description);
}

void Renderer::Init()
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		std::cout << "GLFW Initialization failed!" << std::endl;
	}
}

void Renderer::Shutdown()
{

	glfwTerminate();
}

void Renderer::Clear()
{
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const Shader& shader, const VertexArray& vao, const IndexBuffer& ibo)
{
	shader.Bind();
	vao.Bind();
	ibo.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr))
}

