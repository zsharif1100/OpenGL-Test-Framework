#pragma once

#include <vector>
#include <GL/glew.h>

struct VertexBufferElement 
{
	unsigned int type;
	unsigned int count;
	unsigned int offset;
	unsigned char normalised;
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
	unsigned int m_Count;
public:
	VertexBufferLayout()
		:m_Stride(0), m_Count(0) {}
	//~VertexBufferLayout();

	template<typename T>
	void Push(unsigned int count) = delete;

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, m_Stride, GL_FALSE });
		m_Count += count;
		m_Stride += count * sizeof(GLuint);
	}

	template<>
	void Push<float>(unsigned int count) 
	{
		m_Elements.push_back({ GL_FLOAT, count, m_Stride, GL_FALSE });
		m_Count += count;
		m_Stride += count * sizeof(GLfloat);
	}

	//template<>
	//void Push<unsigned char>(unsigned int count)
	//{
	//	m_Elements.push_back({ GL_UNSIGNED_BYTE, count, m_Stride, GL_FALSE });
	// m_Count += count;
	// m_Stride += count * sizeof(GLbyte);
	//}

	inline const unsigned int& GetStride() const { return m_Stride; };
	inline const unsigned int& GetCount() const { return m_Count; };
	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; };
};