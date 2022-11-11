#include "Quad.h"

#include "glm/gtc/matrix_transform.hpp"

Quad::Quad(glm::vec3 pos, float width, float height, glm::vec4 color, const std::string& texPath)
	:m_Pos(pos), m_Color(color), 
	m_Width(width), m_Height(height), m_Texture(texPath)
{
	this->UpdateModelPosVert();
	this->UpdatePosVert();
}

Quad::Quad(glm::vec3 pos, float width, float height, const std::string& texPath)
	:m_Pos(pos), m_Color(1.0f, 1.0f, 1.0f, 1.0f), 
	m_Width(width), m_Height(height), m_Texture(texPath)
{

	this->UpdateModelPosVert();
	this->UpdatePosVert();
}

void Quad::UpdateModelPosVert() 
{
	m_ModelPosVert[0] = glm::vec4(-m_Width / 2, -m_Height / 2, 0.0, 1.0f);
	m_ModelPosVert[1] = glm::vec4(+m_Width / 2, -m_Height / 2, 0.0, 1.0f);
	m_ModelPosVert[2] = glm::vec4(+m_Width / 2, +m_Height / 2, 0.0, 1.0f);
	m_ModelPosVert[3] = glm::vec4(-m_Width / 2, +m_Height / 2, 0.0, 1.0f);
}

void Quad::UpdatePosVert()
{
	m_Transform = glm::translate(glm::mat4(1.0f), m_Pos);
	m_Transform = glm::rotate(m_Transform, glm::radians(m_Rotation), { 0.0f, 0.0f, 1.0f });
	m_Transform = glm::scale(m_Transform, m_Scale);

	for (unsigned int i = 0; i < std::size(m_PosVert); i++) 
	{
		m_PosVert[i] = m_Transform * m_ModelPosVert[i];
	}
}

void Quad::Translate(glm::vec3 moveVec) 
{
	//glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	m_Pos += moveVec;
	this->UpdatePosVert();
}

void Quad::Rotate(float degrees)
{
	m_Rotation += degrees;
	this->UpdatePosVert();
}

void Quad::Scale(glm::vec3 scaleFactor)
{
	m_Scale = scaleFactor;
	this->UpdatePosVert();
}