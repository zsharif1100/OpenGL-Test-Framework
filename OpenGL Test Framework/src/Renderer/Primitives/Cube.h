#pragma once

#include <array>
#include <memory>

#include "Renderer/Texture.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

class Cube
{
public:
	Cube() = delete;
	Cube(glm::vec3 pos, float width, float height, float length, glm::vec4 color, 
		const std::string& texPath = "");
	/*Cube(glm::vec3 pos, float width, float height, float rotation, glm::vec4 color);*/
	Cube(glm::vec3 pos, float width, float height, float length, const std::string& texPath);

	inline void SetPos(glm::vec3 pos) { m_Pos = pos; UpdatePosVert(); }
	void Translate(glm::vec3 moveVec);
	void Rotate(glm::quat rotation);
	void Scale(glm::vec3 scaleFactor);

	inline const glm::vec3& GetPos() const { return m_Pos; };
	inline const glm::vec4& GetPosVert(int index) const { return m_PosVert[index]; };
	inline  glm::vec3* GetPosPtr() { return &m_Pos; };

	//void SetColor(glm::vec4 color);
	inline const glm::vec4& GetColor() const { return m_Color; };

	inline const Texture& GetTexture() const { return m_Texture; };

	inline const int GetTexID() const { return m_Texture.GetID(); };
	inline void BindTexSlot(unsigned int slot) const { m_Texture.Bind(slot); };
	inline const glm::vec2& GetTexVert(int index) const { return m_TexVert[index]; };

	inline const int* GetOrder(int index) const { return vertexOrder[index]; }
	inline const glm::vec3 GetNormal(int index) { return m_Normals[index]; }

	// {front/facing, back/non-facing, top, left, right, bottom}
	static constexpr int vertexOrder[6][4] = { {0, 1, 2, 3}, {6, 7, 4, 5}, {3, 2, 5, 4}, 
		{7, 0, 3, 4}, {1, 6, 5, 2}, {7, 6, 1, 0} };
private:

	glm::vec3 m_Pos;										// Translate
	glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, 1.0f);	// Rotate
	glm::vec3 m_Scale = glm::vec3(1.0f);					// Scale
	glm::mat4 m_Transform;

	glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

	float m_Width, m_Height, m_Length;
	glm::vec4 m_Color;
	Texture m_Texture;

	glm::vec4 m_ModelPosVert[8];
	glm::vec4 m_PosVert[8];
	glm::vec2 m_TexVert[4] = { {0.0f, 0.0f}, {1.0f, 0.0f }, {1.0f,1.0f}, {0.0f, 1.0f} };
	
	std::array<glm::vec3, 6> m_Normals;

	void UpdateModelPosVert();
	void UpdatePosVert();

	void UpdateNormals();
};