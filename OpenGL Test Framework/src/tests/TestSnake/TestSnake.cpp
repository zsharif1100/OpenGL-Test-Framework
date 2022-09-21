#include "TestSnake.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GLErrorManager.h"

#include "imgui/imgui.h"

#include <random>
#include <iostream>

int getRandomInt(int x, int y)
{
	std::random_device randomSeed;

	std::mt19937 randomGenerator(randomSeed());
	std::uniform_int_distribution<int> distribution(x, y);

	return distribution(randomGenerator);
}

Segment::Segment(float x, float y, float width, float height)
	:m_Pos(x, y),
	m_Width(width),
	m_Height(height) {}

std::array<float, 16> Segment::GetVB()
{
	std::array<float, 16> VB = {
		//x				   y				texture
		  0 - m_Width / 2.0f, -m_Height / 2.0f,	0.0, 0.0,
		  0 + m_Width / 2.0f, -m_Height / 2.0f,	1.0, 0.0,
		  0 + m_Width / 2.0f, +m_Height / 2.0f,	1.0, 1.0,
		  0 - m_Width / 2.0f, +m_Height / 2.0f,	0.0, 1.0,
	};

	return VB;
}

bool Segment::SetPos(glm::vec2& newPos)
{
	//if (newPos.x < 0.0f || newPos.x > m_SWidth || newPos.y < 0.0f || newPos.y > m_SHeight) {
	//	return false;
	//}
	if (newPos.x < 0.0f) {
		newPos.x = 1280.0f - Segment::m_Width / 2;
	}
	else if (newPos.x > 1280.0f) {
		newPos.x = 0.0f + Segment::m_Width / 2;
	}
	if (newPos.y < 0.0f) {
		newPos.y = 980.0f - Segment::m_Height / 2;
	}
	else if (newPos.y > 980.0f) {
		newPos.y = 0.0f + Segment::m_Height / 2;
	}

	m_Pos = newPos;
	return true;
}

TestSnake::TestSnake()
	:m_Time(0),
	m_Title(1280.0f / 2.0f, 960.0f / 2.0f, 1280.0f / 1.5f, 960.0f / 2.0f)
{
	//GLCall(glViewport(SCONST::RSCREEN_WIDTH_BEGIN, SCONST::RSCREEN_HEIGHT_BEGIN, m_SWidth, m_SHeight))

	for (float i = 0; i < 2; i++) {
		for (float x = 0.0f + m_Size / 2; x <= m_SWidth - m_Size / 2; x += m_Size) {
			m_Border.emplace_back(x, 0.0f + m_Size / 2 + i * (m_SHeight - m_Size),m_Size, m_Size);
		}

		for (float y = 0.0f + m_Size / 2; y <= m_SHeight - m_Size / 2; y += m_Size) {
			m_Border.emplace_back(0.0f + m_Size / 2 + i * (m_SWidth - m_Size), y, m_Size, m_Size);
		}

	}

	this->Restart();

	float* title_VBO = m_Title.GetVB().data();
	m_TitleVBO = std::make_unique<VertexBuffer>(title_VBO, Segment::m_VBOSize);

	float* segment_VBO = m_Segments.front().GetVB().data();

	m_VBO = std::make_unique<VertexBuffer>(segment_VBO, Segment::m_VBOSize);

	m_Layout = std::make_unique<VertexBufferLayout>();

	m_Layout->Push<float>(2);
	m_Layout->Push<float>(2);


	m_VAO = std::make_unique<VertexArray>();
	m_VAO->AddBuffer(*m_TitleVBO, *m_Layout);

	m_IBO = std::make_unique<IndexBuffer>(Segment::m_IB, Segment::m_Count);
	m_IBO->Bind();

	m_Shader = std::make_unique<Shader>("../../../OpenGL Test Framework/res/shaders/Shader1.shader");

	m_TexSnake = std::make_unique<Texture>("../../../OpenGL Test Framework/res/textures/segment.png");
	m_TexTitle = std::make_unique<Texture>("../../../OpenGL Test Framework/res/textures/title.png");
	m_TexApple = std::make_unique<Texture>("../../../OpenGL Test Framework/res/textures/apple.png");
	m_TexBorder = std::make_unique<Texture>("../../../OpenGL Test Framework/res/textures/border.png");

	m_Shader->SetUniform1i("u_Texture", 0);

}

void TestSnake::RenderSnake()
{
	m_TexSnake->Bind();
	for (auto& segment : m_Segments) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(segment.GetPos(), 0.0f));
		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp); //MVP = model view projection
		Renderer::Draw(*m_Shader, *m_VAO, *m_IBO);
	}
}

void TestSnake::RenderApple()
{
	m_TexApple->Bind();
	for (auto& apple : m_Apples) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(apple.GetPos(), 0.0f));
		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp); //MVP = model view projection
		Renderer::Draw(*m_Shader, *m_VAO, *m_IBO);
	}
}

void TestSnake::RenderBorder()
{
	m_TexBorder->Bind();
	for (auto& segment : m_Border) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(segment.GetPos(), 0.0f));
		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp); //MVP = model view projection
		Renderer::Draw(*m_Shader, *m_VAO, *m_IBO);
	}
}

void TestSnake::RenderTitle()
{
	m_TexTitle->Bind();
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(m_Title.GetPos(), 0.0f));
	glm::mat4 mvp = m_Proj * m_View * model;
	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_MVP", mvp);
	Renderer::Draw(*m_Shader, *m_VAO, *m_IBO);
}

void TestSnake::OnRender(Window&)
{
	if (!m_Started) {
		this->RenderTitle();
		return;
	}
	this->RenderBorder();

	this->RenderSnake();

	this->RenderApple();
}

void TestSnake::OnUpdate(Window& window, float dT)
{
	if (!m_Started) {
		if (window.GetKey(GLFW_KEY_SPACE)) {
			m_Started = true;
			m_TexSnake->Bind();
			m_VAO->AddBuffer(*m_VBO, *m_Layout);
		}
		else
			return;
	}

	//keyboard press logic
	if (window.GetKey(GLFW_KEY_LEFT)) {
		this->updateMoveBuffer(direction::LEFT);
	}
	else if (window.GetKey(GLFW_KEY_RIGHT)) {
		this->updateMoveBuffer(direction::RIGHT);
	}
	else if (window.GetKey(GLFW_KEY_UP)) {
		this->updateMoveBuffer(direction::UP);
	}
	else if (window.GetKey(GLFW_KEY_DOWN)) {
		this->updateMoveBuffer(direction::DOWN);
	}

	float currentTime = (float)glfwGetTime();
	float elapsed = currentTime - m_Time;
	float multiplier = (float)m_Difficulty / 100.0f;
	//per Move logic
	if (elapsed > dT * multiplier ) {
		if (!this->Move() || this->HitsBorder()) {
			this->Restart();
			m_VAO->AddBuffer(*m_TitleVBO, *m_Layout);
		}

		//apple logic
		this->UpdateApple();

		m_Time = currentTime;
	}

}

void TestSnake::OnImGuiRender(Window& window)
{
	WindowData * wData = (WindowData*)glfwGetWindowUserPointer(window.GetWindow());

	ImGui::SetNextWindowSize(ImVec2(wData->m_Width - wData->m_VPWidth, wData->m_Height), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(wData->m_VPWidth, 0.0f), ImGuiCond_Always);
	ImGui::Begin("Test", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::Text("Framerate: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void TestSnake::updateMoveBuffer(direction dir)
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

void TestSnake::ChangeDir(const direction newDir)
{
	if (abs((int)m_Direction - (int)newDir) != 2) {
		m_Direction = newDir;
	}
}

bool TestSnake::Move()
{
	Segment& head = m_Segments.front();

	glm::vec2 nextPos = head.GetPos();

	//change dir
	if (!m_MoveBuffer.empty()) {
		this->ChangeDir(m_MoveBuffer.front());
		m_MoveBuffer.pop();
	}

	glm::vec2 newHeadPos = head.GetPos() + dirVectors[(int)m_Direction] * glm::vec2(head.m_Width, head.m_Height);

	//self-collision check
	for (auto segment = std::next(m_Segments.begin()); segment != m_Segments.end(); segment++) {
		if (newHeadPos == segment->GetPos())
			return false;
	};

	//wall collision check + Move head segment
	if (head.SetPos(newHeadPos) == false)
		return false;

	//
	m_EmptySpace.erase(std::make_pair(newHeadPos.x, newHeadPos.y));

	//Move rest of the segments
	glm::vec2 currPos = std::next(m_Segments.begin())->GetPos();

	for (auto segment = std::next(m_Segments.begin()); segment != m_Segments.end(); segment++) {

		currPos = segment->GetPos();

		//movement
		segment->SetPos(nextPos);
		nextPos = currPos;

	}


	if (m_JustGrew) {
		m_JustGrew = false;
	}
	else {
		Segment& back = m_Segments.back();
		m_EmptySpace.insert(std::make_pair(back.GetPos().x, back.GetPos().y));
	}

	return true;
}

bool TestSnake::HitsBorder()
{
	Segment& head = m_Segments.front();
	for (auto& border : m_Border) {
		if (border.GetPos() == head.GetPos())
			return true;
	}
	return false;
}

void TestSnake::Grow()
{
	m_Segments.emplace_back(m_Segments.back());
	m_JustGrew = true;
}


void TestSnake::SpawnApple()
{
	int max = m_EmptySpace.size() - 1;

	if (max == -1) {
		return;
	}

	int randomNum = getRandomInt(0, max);
	const std::pair<float, float>& randomPos = *std::next(m_EmptySpace.begin(), randomNum);

	m_Apples.emplace_back(Segment(randomPos.first, randomPos.second, m_Size, m_Size));
	m_EmptySpace.erase(randomPos);
}

void TestSnake::UpdateApple()
{
	glm::vec2 headPos = this->GetHead().GetPos();

	for (auto appleIt = m_Apples.begin(); appleIt != m_Apples.end(); appleIt++) {
		if (appleIt->GetPos() == headPos) {
			m_Apples.erase(appleIt);
			this->Grow();
			this->SpawnApple();
			break;
		}
	}
}

void TestSnake::Restart()
{
	m_Started = false;
	m_ShouldRestart = false;

	m_EmptySpace.clear();

	for (float i = 0.0f + m_Size / 2.0f; i <= m_SWidth - m_Size / 2.0f; i += m_Size) {
		for (float j = 0.0f + m_Size / 2.0f; j <= m_SHeight - m_Size / 2.0f; j += m_Size) {
			m_EmptySpace.insert(std::make_pair(i, j));
		}
	}

	for (auto& segment : m_Border) {
		glm::vec2 pos = segment.GetPos();
		m_EmptySpace.erase(std::make_pair(pos.x, pos.y));
	}

	//while (!m_MoveBuffer.empty()) {
	//	m_MoveBuffer.pop();
	//} 
	m_MoveBuffer = {};
	m_Apples.clear();
	m_Segments.clear();
	m_Direction = direction::LEFT;

	m_Segments.emplace_back((0.0f + m_Size / 2.0f) + m_SWidth / 2.0f, (0.0f + m_Size / 2.0f) + m_SHeight / 2.0f, m_Size, m_Size);

	//m_Segments.emplace_back(Segment(0.0f + m_Size / 2.0f, SCONST::RSCREEN_MIDDLE.y, m_Size, m_Size));
	for (int i = 0; i < StartSegmentCount - 1; i++) {
		this->Grow();
		this->Move();
	};

	for (int i = 0; i < StarAppleCount; i++) {
		this->SpawnApple();
	};
}