#include "Renderer.h"

#include <GL/glew.h>
#include "GLErrorManager.h"
#include <iostream>



void Renderer::Clear() const
{
	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const Shader& shader, const VertexArray& vao, const IndexBuffer& ibo) const
{
	shader.Bind();
	vao.Bind();
	ibo.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr))
}
