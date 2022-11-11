#pragma once
class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
public:
	IndexBuffer(const unsigned int* data, const unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline const unsigned int GetID() const { return m_RendererID; }
	inline const unsigned int GetCount() const { return m_Count; }

};