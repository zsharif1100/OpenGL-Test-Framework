#include "TestSnake.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GLErrorManager.h"

#include "imgui/imgui.h"

#include <random>

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
		  0 - m_Width / 2, -m_Height / 2,	0.0, 0.0,
		  0 + m_Width / 2, -m_Height / 2,	1.0, 0.0,
		  0 + m_Width / 2, +m_Height / 2,	1.0, 1.0,
		  0 - m_Width / 2, +m_Height / 2,	0.0, 1.0,
	};

	return VB;
}

bool Segment::SetPos(glm::vec2& newPos)
{
	if (newPos.x < 0 || newPos.x > 1280 || newPos.y < 0 || newPos.y > 720) {
		return false;
	}
	//if (newPos.x < 0) {
	//	newPos.x = 1280 - Segment::m_Width / 2;
	//}
	//else if (newPos.x > 1280) {
	//	newPos.x = 0 + Segment::m_Width / 2;
	//}
	//if (newPos.y < 0) {
	//	newPos.y = 720 - Segment::m_Height / 2;
	//}
	//else if (newPos.y > 720) {
	//	newPos.y = 0 + Segment::m_Height / 2;
	//}

	m_Pos = newPos;
	return true;
}

TestSnake::TestSnake()
	:m_Time(0)
{
	this->Restart();

	Segment title(640.0f, 360.0f, 1280.0f / 2, 720.0f / 2);
	float* title_VBO = title.GetVB().data();
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

	m_Shader->SetUniform1i("u_Texture", 0);

}

void TestSnake::OnRender(Renderer& renderer)
{

	if (!m_Started) {
		m_TexTitle->Bind();
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(640.0f, 380.0f, 0.0f));
		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp); //MVP = model view projection
		renderer.Draw(*m_Shader, *m_VAO, *m_IBO);
		return;
	}

	m_TexSnake->Bind();
	for (auto& segment : m_Segments) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(segment.GetPos(), 0.0f));
		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp); //MVP = model view projection
		renderer.Draw(*m_Shader, *m_VAO, *m_IBO);
	}

	m_TexApple->Bind();
	for (auto& apple : m_Apples) {
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(apple.GetPos(), 0.0f));
		glm::mat4 mvp = m_Proj * m_View * model;
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp); //MVP = model view projection
		renderer.Draw(*m_Shader, *m_VAO, *m_IBO);
	}
}

void TestSnake::OnUpdate(GLFWwindow* window, float dT)
{
	if (!m_Started) {
		if (glfwGetKey(window, GLFW_KEY_SPACE)) {
			m_Started = true;
			m_TexSnake->Bind();
			m_VAO->AddBuffer(*m_VBO, *m_Layout);
		}
		else
			return;
	}

	//keyboard press logic
	if (glfwGetKey(window, GLFW_KEY_LEFT)) {
		this->updateMoveBuffer(direction::LEFT);
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
		this->updateMoveBuffer(direction::RIGHT);
	}
	else if (glfwGetKey(window, GLFW_KEY_UP)) {
		this->updateMoveBuffer(direction::UP);
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN)) {
		this->updateMoveBuffer(direction::DOWN);
	}

	float currentTime = (float)glfwGetTime();
	float elapsed = currentTime - m_Time;
	float multiplier = (float)m_Difficulty / 100.0f;
	//per Move logic
	if (elapsed > dT * multiplier ) {
		if (!this->Move()) {
			this->Restart();
			m_VAO->AddBuffer(*m_TitleVBO, *m_Layout);
		}

		//apple logic
		this->UpdateApple();

		m_Time = currentTime;
	}

}

//void TestSnake::OnImGuiRender()
//{
//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//}

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

	m_Apples.emplace_back(Segment(randomPos.first, randomPos.second, 20.0f, 20.0f));
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

	for (float i = 0 + 10; i <= 1280 - 10; i += 20) {
		for (float j = 0 + 10; j <= 720 - 10; j += 20) {
			m_EmptySpace.insert(std::make_pair(i, j));
		}
	}

	//while (!m_MoveBuffer.empty()) {
	//	m_MoveBuffer.pop();
	//}
	m_MoveBuffer = {};
	m_Apples.clear();
	m_Segments.clear();
	m_Direction = direction::LEFT;

	m_Segments.emplace_back(Segment(640.0f - 10.0f, 360.0f- 10.0f, 20.0f, 20.0f));
	for (int i = 0; i < 3; i++) {
		this->Grow();
		this->Move();
	};

	for (int i = 0; i < 1; i++) {
		this->SpawnApple();
	};
}