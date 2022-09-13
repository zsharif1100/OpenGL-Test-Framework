#pragma once

#include "Test.h"

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
	inline glm::vec2 GetPos() { return m_Pos; };
};

class TestSnake: public Test
{
private:
	float m_Time;
	bool m_Started;
	bool m_ShouldRestart;
	bool m_JustGrew;
	std::list<Segment> m_Segments;
	std::list<Segment> m_Apples;

	std::set<std::pair<float, float>> m_EmptySpace;
	direction m_Direction;
	const difficulty m_Difficulty = difficulty::HARD;
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


	const glm::mat4 m_Proj = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
	const glm::mat4 m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
public:
	TestSnake();

	void OnRender(Renderer& renderer) override;
	void OnUpdate(GLFWwindow* window, float dT) override;
	//void OnImGuiRender() override;

	void ChangeDir(const direction newDir);
	bool Move();
	void Grow();
	void SpawnApple();
	void UpdateApple();

	void Restart();
	void updateMoveBuffer(direction dir);

	inline Segment& GetHead() { return m_Segments.front(); }
};

