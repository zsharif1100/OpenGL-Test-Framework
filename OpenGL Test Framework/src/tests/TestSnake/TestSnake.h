#pragma once

#include "tests/Test.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <array>
#include <list>
#include <queue>
#include <set>
#include <memory>

enum class direction {
	LEFT, DOWN, RIGHT, UP
};

enum class difficulty {
	EASY = 150, MEDIUM = 100, HARD = 75, EXTREME = 50
};

static const glm::vec2 dirVectors[4] = {
	{-1.0f, 0.0f },
	{ 0.0f,-1.0f },
	{ 1.0f, 0.0f },
	{ 0.0f, 1.0f },
};

class Segment
{
private:
	glm::vec2 m_Pos;
public:
	const float m_Width;
	const float m_Height;
	static constexpr unsigned int m_IB[6] = {
		0, 1, 2,
		2, 3, 0,
	};
	static constexpr unsigned int m_VBOSize = 16 * sizeof(float);
	static constexpr unsigned int m_Count = 6;

	Segment() = delete;
	Segment(float x, float y, float width, float height);

	std::array<float, 16> GetVB();

	bool SetPos(glm::vec2& newPos);
	inline glm::vec2 GetPos() { return m_Pos; }
	inline float GetWidth() { return m_Width; }
	inline float GetHeight() { return m_Height; }
};

class TestSnake: public Test
{
public:
	TestSnake();
	static constexpr int StartSegmentCount = 3;
	static constexpr int StarAppleCount = 3;

	void RenderSnake();
	void RenderApple();
	void RenderBorder();
	void RenderTitle();

	void OnRender(Window& window) override;
	void OnUpdate(Window& window, float dT) override;
	void OnImGuiRender(Window& window) override;
	//void OnImGuiRender() override;

	void ChangeDir(const direction newDir);
	bool Move();
	bool HitsBorder();
	void Grow();
	void SpawnApple();
	void UpdateApple();

	void Restart();
	void updateMoveBuffer(direction dir);

	inline Segment& GetHead() { return m_Segments.front(); }
private:


	float m_Time;
	bool m_Started;
	bool m_ShouldRestart;
	bool m_JustGrew;
	Segment m_Title;
	std::list<Segment> m_Segments;
	std::list<Segment> m_Border;
	std::list<Segment> m_Apples;

	std::set<std::pair<float, float>> m_EmptySpace;
	direction m_Direction;
	const difficulty m_Difficulty = difficulty::MEDIUM;
	std::queue<direction> m_MoveBuffer;

	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<VertexBuffer> m_TitleVBO;
	std::unique_ptr<VertexBufferLayout> m_Layout;


	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<IndexBuffer> m_IBO;
	std::unique_ptr<Shader> m_Shader;

	std::unique_ptr<Texture> m_TexSnake;
	std::unique_ptr<Texture> m_TexTitle;
	std::unique_ptr<Texture> m_TexApple;
	std::unique_ptr<Texture> m_TexBorder;

	const float m_SWidth = 1280.0f;
	const float m_SHeight = 960.0f;

	const glm::mat4 m_Proj = glm::ortho(0.0f, m_SWidth, 0.0f, m_SHeight, -1.0f, 1.0f);
	const glm::mat4 m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	//const float m_SizeFactor = 15.0f;
	const float m_Size = 40.0f; //SCONST::RSCREEN_RATIO * m_SizeFactor;

};

