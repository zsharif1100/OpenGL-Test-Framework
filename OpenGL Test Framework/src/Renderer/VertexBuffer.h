#pragma once

#include <GL/glew.h>

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const float data[], const unsigned int size, GLenum usage = GL_STATIC_DRAW);
	~VertexBuffer();


	void updateBuffer(const float data[], unsigned int size, unsigned int offset);

	void Bind() const;
	void Unbind() const;

	inline const unsigned int GetID() const { return m_RendererID; }

};

