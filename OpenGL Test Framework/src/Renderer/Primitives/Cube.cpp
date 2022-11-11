#include "Cube.h"

#include "glm/gtc/matrix_transform.hpp"

Cube::Cube(glm::vec3 pos, float width, float height, float length, glm::vec4 color, const std::string& texPath)
	:m_Pos(pos), m_Color(color),
	m_Width(width), m_Height(height), m_Length(length), m_Texture(texPath)
{
	this->UpdateModelPosVert();
	this->UpdatePosVert();
}

Cube::Cube(glm::vec3 pos, float width, float height, float length, const std::string& texPath)
	:m_Pos(pos), m_Color(1.0f, 1.0f, 1.0f, 1.0f),
	m_Width(width), m_Height(height), m_Length(length), m_Texture(texPath)
{

	this->UpdateModelPosVert();
	this->UpdatePosVert();
}

void Cube::UpdateModelPosVert()
{
	m_ModelPosVert[0] = glm::vec4(-m_Width / 2, -m_Height / 2, +m_Length / 2, 1.0f);
	m_ModelPosVert[1] = glm::vec4(+m_Width / 2, -m_Height / 2, +m_Length / 2, 1.0f);
	m_ModelPosVert[2] = glm::vec4(+m_Width / 2, +m_Height / 2, +m_Length / 2, 1.0f);
	m_ModelPosVert[3] = glm::vec4(-m_Width / 2, +m_Height / 2, +m_Length / 2, 1.0f);

	m_ModelPosVert[4] = glm::vec4(-m_Width / 2, +m_Height / 2, -m_Length / 2, 1.0f);
	m_ModelPosVert[5] = glm::vec4(+m_Width / 2, +m_Height / 2, -m_Length / 2, 1.0f);
	m_ModelPosVert[6] = glm::vec4(+m_Width / 2, -m_Height / 2, -m_Length / 2, 1.0f);
	m_ModelPosVert[7] = glm::vec4(-m_Width / 2, -m_Height / 2, -m_Length / 2, 1.0f);
}

void Cube::UpdatePosVert()
{
	m_Transform = glm::translate(glm::mat4(1.0f), m_Pos);
	//m_Transform = glm::lookAt(m_Pos, m_Pos + m_Orientation, m_Up) * m_Transform;
	//m_Transform = glm::scale(m_Transform, m_Scale);

	for (unsigned int i = 0; i < std::size(m_PosVert); i++)
	{
		m_PosVert[i] = m_Transform * m_ModelPosVert[i];
	}
}

void Cube::UpdateNormals()
{
	for (int i = 0; i < 6; i++) {
		const glm::vec3& p0 = m_PosVert[vertexOrder[i][0]];
		const glm::vec3& p1 = m_PosVert[vertexOrder[i][1]];
		const glm::vec3& p2 = m_PosVert[vertexOrder[i][3]];

		m_Normals[i] = glm::normalize(glm::cross(p1 - p0, p2 - p0));
	}
}

void Cube::Translate(glm::vec3 moveVec) 
{
	m_Pos += moveVec;
	this->UpdatePosVert();
	this->UpdateNormals();
}

void Cube::Rotate(glm::quat rotation)
{
	m_Orientation = m_Orientation * rotation;
	this->UpdatePosVert();
	this->UpdateNormals();
}

void Cube::Scale(glm::vec3 scaleFactor)
{
	m_Scale = scaleFactor;
	this->UpdatePosVert();
	this->UpdateNormals();
}