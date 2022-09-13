#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "Shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class Renderer
{
private:

public:
	void Clear() const;
	void Draw(const Shader& shader, const VertexArray& vao, const IndexBuffer& ibo) const;
};