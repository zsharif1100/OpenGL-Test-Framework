#include "TestSnakeBatch.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GLErrorManager.h"

#include "imgui/imgui.h"

#include <iostream>

TestSnakeBatch::TestSnakeBatch(Window* window)
	:m_Window(window),
	m_WindowData(m_Window->GetWindowData()),
	m_Camera(window, 960.0f / 2, { m_SWidth / 2, m_SHeight / 2, 0.0f}),
	m_Title(glm::vec3(m_SWidth / 2.0f, m_SHeight / 2.0f, 0.0f),
		m_SWidth / 1.5f, m_SHeight / 2.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		"../../../OpenGL Test Framework/res/textures/title.png")
{
	for (float i = 0.0f; i < 2; i++) {
		//top & bottom walls
		for (float x = 0.0f + m_Size / 2; x <= m_SWidth - m_Size / 2; x += m_Size) {
			m_Border.emplace_back(glm::vec3(x, m_Size / 2 + i * (m_SHeight - m_Size), 0.0f), 
				m_Size, m_Size, m_BorderColor,
				"../../../OpenGL Test Framework/res/textures/segment_clear.png");
		}

		//left & right walls
		for (float y = 0.0f + m_Size / 2; y <= m_SHeight - m_Size / 2; y += m_Size) {
			m_Border.emplace_back(glm::vec3(m_Size / 2 + i * (m_SWidth - m_Size), y, 0.0f), 
				m_Size, m_Size, m_BorderColor,
				"../../../OpenGL Test Framework/res/textures/segment_clear.png");
		}

	}

	for (float i = 0.0f + m_Size / 2.0f; i <= m_SWidth - m_Size / 2.0f; i += m_Size) {
		for (float j = 0.0f + m_Size / 2.0f; j <= m_SHeight - m_Size / 2.0f; j += m_Size) {
			m_DefaultEmptySpace.insert({ i, j, 0.0f });
		}
	}

	for (auto& segment : m_Border) {
		const glm::vec3& pos = segment.GetPos();
		m_DefaultEmptySpace.erase({ pos.x, pos.y, pos.z });
	}

	this->Restart();
}

void TestSnakeBatch::OnRender()
{
	Renderer::BeginScene(m_Camera);
	if (!m_Started) {
		Renderer::DrawQuad(m_Title);

		for (auto& quad : m_Border)
			Renderer::DrawQuad(quad);

		Renderer::EndScene();
		return;
	}

	for (auto& quad : m_Border)
		Renderer::DrawQuad(quad);

	for (auto& quad : m_Snake)
		Renderer::DrawQuad(quad);

	for (auto& quad : m_Apples)
		Renderer::DrawQuad(quad);
	Renderer::EndScene();
}

void TestSnakeBatch::OnUpdate(float dT)
{
	if (!m_Started) {
		if (m_Window->GetKey(GLFW_KEY_SPACE)) {
			m_Started = true;
		}
		else { return; }
	}

	//keyboard press logic
	if (m_Window->GetKey(GLFW_KEY_LEFT)) {
		this->updateMoveBuffer(direction::LEFT);
	}
	else if (m_Window->GetKey(GLFW_KEY_RIGHT)) {
		this->updateMoveBuffer(direction::RIGHT);
	}
	else if (m_Window->GetKey(GLFW_KEY_UP)) {
		this->updateMoveBuffer(direction::UP);
	}
	else if (m_Window->GetKey(GLFW_KEY_DOWN)) {
		this->updateMoveBuffer(direction::DOWN);
	}

	float currentTime = (float)glfwGetTime();
	float elapsed = currentTime - m_Time;
	float multiplier = (float)m_Difficulty / 100.0f;
	//per Move logic
	if (elapsed > dT * multiplier ) {
		if (!this->Move() || this->HitsBorder()) {
			this->Restart();
		}

		//apple logic
		this->UpdateApple();

		m_Time = currentTime;
	}

}

void TestSnakeBatch::OnImGuiRender()
{
	ImGui::SetNextWindowSize(ImVec2((float)m_WindowData->m_Width - m_WindowData->m_VPWidth, (float)m_WindowData->m_Height), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2((float)m_WindowData->m_VPWidth, 0.0f), ImGuiCond_Always);

	ImGui::Begin("Test", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::Text("Framerate: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Score: %i", m_Score);
	ImGui::Text("Highscore: %i", m_Highscore);
	ImGui::End();
}

void TestSnakeBatch::updateMoveBuffer(direction dir)
{
	if (m_MoveBuffer.size() > 3) {
		m_MoveBuffer.pop();
	}
	else {
		m_MoveBuffer.push(dir);
	}

	while (!m_MoveBuffer.empty() && m_MoveBuffer.front() == m_Direction) {
		m_MoveBuffer.pop();
	}
}

void TestSnakeBatch::ChangeDir(const direction newDir)
{
	if (abs((int)m_Direction - (int)newDir) != 2) {
		m_Direction = newDir;
	}
}

bool TestSnakeBatch::Move()
{
	Quad& head = m_Snake.front();

	glm::vec3 nextPos = head.GetPos();

	//change dir
	if (!m_MoveBuffer.empty()) {
		this->ChangeDir(m_MoveBuffer.front());
		m_MoveBuffer.pop();
	}

	glm::vec3 newHeadPos = head.GetPos() + dirVectors[(int)m_Direction] * glm::vec3(m_Size, m_Size, 0.0f);
	if (newHeadPos.x < 0.0f) {
		newHeadPos.x = 1280.0f - m_Size / 2;
	}
	else if (newHeadPos.x > 1280.0f) {
		newHeadPos.x = 0.0f + m_Size / 2;
	}
	if (newHeadPos.y < 0.0f) {
		newHeadPos.y = 980.0f - m_Size / 2;
	}
	else if (newHeadPos.y > 980.0f) {
		newHeadPos.y = 0.0f + m_Size / 2;
	}
	
	//self-collision check
	for (auto segment = std::next(m_Snake.begin()); segment != m_Snake.end(); segment++) {
		if (newHeadPos == segment->GetPos())
			return false;
	};

	//wall collision check + Move head segment
	head.SetPos(newHeadPos);

	//
	m_EmptySpace.erase(newHeadPos);

	//Move rest of the segments
	glm::vec3 currPos = std::next(m_Snake.begin())->GetPos();

	for (auto segment = std::next(m_Snake.begin()); segment != m_Snake.end(); segment++) {

		currPos = segment->GetPos();

		//movement
		segment->SetPos(nextPos);
		nextPos = currPos;

	}

	if (m_JustGrew) {
		m_JustGrew = false;
	}
	else {
		const glm::vec3& backPos = m_Snake.back().GetPos();
		m_EmptySpace.insert({ backPos.x, backPos.y, backPos.z });
	}

	return true;
}

bool TestSnakeBatch::HitsBorder()
{
	Quad& head = m_Snake.front();
	for (auto& border : m_Border) {
		if (border.GetPos() == head.GetPos())
			return true;
	}
	return false;
}

void TestSnakeBatch::Grow()
{
	m_Snake.emplace_back(m_Snake.back());
	m_JustGrew = true;
}

void TestSnakeBatch::SpawnApple()
{
	int max = m_EmptySpace.size() - 1;

	if (max == -1) {
		return;
	}

	int randomNum = getRandomInt(0, max);
	const glm::vec3& randomPos = *std::next(m_EmptySpace.begin(), randomNum);

	m_Apples.emplace_back(randomPos, m_Size, m_Size, 
		m_AppleColor, 
		"../../../OpenGL Test Framework/res/textures/segment_clear.png");

	m_EmptySpace.erase(randomPos);
}

void TestSnakeBatch::UpdateApple()
{
	glm::vec3 headPos = this->GetHead().GetPos();

	for (auto appleIt = m_Apples.begin(); appleIt != m_Apples.end(); appleIt++) {
		if (appleIt->GetPos() == headPos) {
			m_Apples.erase(appleIt);
			this->Grow();
			this->SpawnApple();

			m_Score++;
			break; 
		}
	}
}

void TestSnakeBatch::Restart()
{
	m_Started = false;
	m_ShouldRestart = false;

	m_EmptySpace.clear();

	m_EmptySpace = m_DefaultEmptySpace;

	while (!m_MoveBuffer.empty())
		m_MoveBuffer.pop();

	m_Direction = direction::LEFT;

	if (m_Score > m_Highscore)
		m_Highscore = m_Score;
	m_Score = 0;

	m_Apples.clear();
	m_Snake.clear();

	m_Snake.emplace_back(glm::vec3((m_Size / 2.0f) + m_SWidth / 2.0f, (m_Size / 2.0f) + m_SHeight / 2.0f, 0.0f), 
		m_Size, m_Size, m_SnakeColor, 
		"../../../OpenGL Test Framework/res/textures/segment_clear.png");

	for (int i = 0; i < StartSegmentCount - 1; i++) {
		this->Grow();
		this->Move();
	};

	for (int i = 0; i < StartAppleCount; i++) {
		this->SpawnApple();
	};
}