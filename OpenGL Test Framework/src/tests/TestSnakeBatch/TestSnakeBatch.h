#pragma once

#include "tests/Test.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/hash.hpp"

#include <array>
#include <list>
#include <queue>
#include <unordered_set>
#include <memory>

#include "tests/TestSnake/GameHelper.h"

class TestSnakeBatch: public Test
{
public:
	TestSnakeBatch() = delete;
	TestSnakeBatch(Window* window);
	static constexpr int StartSegmentCount = 3;
	static constexpr int StartAppleCount = 3;

	void OnRender() override;
	void OnUpdate(float dT) override;
	void OnImGuiRender() override;

	void ChangeDir(const direction newDir);
	bool Move();
	bool HitsBorder();
	void Grow();
	void SpawnApple();
	void UpdateApple();

	void Restart();
	void updateMoveBuffer(direction dir);

	inline Quad& GetHead() { return m_Snake.front(); }
private:
	Window* m_Window;
	WindowData* m_WindowData;

	float m_Time = 0;
	bool m_Started;
	bool m_ShouldRestart;
	bool m_JustGrew;

	Quad m_Title;
	std::list<Quad> m_Snake;
	std::list<Quad> m_Border;
	std::list<Quad> m_Apples;

	std::unordered_set<glm::vec3> m_DefaultEmptySpace;
	std::unordered_set<glm::vec3> m_EmptySpace;

	
	glm::vec4 m_SnakeColor = {34.0f / 255, 177.0f / 255, 76.0f / 255, 1.0f};
	glm::vec4 m_BorderColor = {127.0f / 255, 127.0f / 255, 127.0f / 255, 1.0f};
	glm::vec4 m_AppleColor = {237.0f / 255, 28.0f / 255, 36.0f / 255, 1.0f};

	int m_Score = 0;
	int m_Highscore = 0;

	direction m_Direction;
	const difficulty m_Difficulty = difficulty::MEDIUM;
	std::queue<direction> m_MoveBuffer;

	static constexpr float m_SHeight = 960.0f;
	static constexpr float m_SWidth = 1280.0f;

	CameraOrthographic m_Camera;
	//const float m_SizeFactor = 15.0f;
	const float m_Size = 40.0f; //SCONST::RSCREEN_RATIO * m_SizeFactor;

};

