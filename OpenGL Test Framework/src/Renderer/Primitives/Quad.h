#pragma once

#include <vector>
#include <memory>

#include "Renderer/Texture.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>

class Quad
{
public:
	Quad() = delete;
	Quad(glm::vec3 pos, float width, float height, glm::vec4 color, const std::string& texPath = "");
	/*Quad(glm::vec3 pos, float width, float height, float rotation, glm::vec4 color);*/
	Quad(glm::vec3 pos, float width, float height, const std::string& texPath);

	inline void SetPos(glm::vec3 pos) { m_Pos = pos; UpdatePosVert(); }
	void Translate(glm::vec3 moveVec);
	void Rotate(float degrees);
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

	inline const int* GetOrder(int index) const { return vertexOrder; }

	static constexpr int vertexOrder[4] = { 0, 1, 2, 3 };

private:

	glm::vec3 m_Pos;						// Translate
	float m_Rotation = 0;					// Rotate
	glm::vec3 m_Scale = glm::vec3(1.0f);	// Scale
	glm::mat4 m_Transform;

	float m_Width, m_Height;
	glm::vec4 m_Color;
	Texture m_Texture;

	glm::vec4 m_ModelPosVert[4];
	glm::vec4 m_PosVert[4];
	glm::vec2 m_TexVert[4] = { {0.0f, 0.0f}, {1.0f, 0.0f }, {1.0f,1.0f}, {0.0f, 1.0f} };

	void UpdateModelPosVert();
	void UpdatePosVert();
}; 