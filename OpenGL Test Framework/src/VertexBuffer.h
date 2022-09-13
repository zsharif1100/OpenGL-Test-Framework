#pragma once
class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(const float data[], const unsigned int size);
	~VertexBuffer();

	void Bind() const;

	void Unbind() const;

};

