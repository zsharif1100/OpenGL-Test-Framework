#include "TestMenu.h"

#include "tests/Test2DRenderer/Test2DRenderer.h"
#include "tests/Test3DRenderer/Test3DRenderer.h"
#include "tests/TestSnake/TestSnake.h"
#include "tests/TestSnakeBatch/TestSnakeBatch.h"

#include "imgui/imgui.h"

TestMenu::TestMenu(Window* window, Test*& currentTest)
	:m_CurrentTest(currentTest), 
	m_Window(window),
	m_WindowData(m_Window->GetWindowData())
{

	m_Tests.push_back(std::make_pair("3D Renderer",
		[this]() {return new Test3DRenderer(this->GetWindow()); }));

	m_Tests.push_back(std::make_pair("2D Renderer", 
		[this]() {return new Test2DRenderer(this->GetWindow()); }));

	m_Tests.push_back(std::make_pair("Snake Game (Batch rendered)", 
		[this]() {return new TestSnakeBatch(this->GetWindow()); }));

	m_Tests.push_back(std::make_pair("Snake Game (No-Batch)",
		[this]() {return new TestSnake(this->GetWindow()); }));

}

void TestMenu::OnImGuiRender()
{
	ImGui::ShowDemoWindow();

	ImGui::SetNextWindowSize(ImVec2((float)m_WindowData->m_Width - m_WindowData->m_VPWidth, (float)m_WindowData->m_Height), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2((float)m_WindowData->m_VPWidth, 0.0f), ImGuiCond_Always);

	ImGui::Begin("Test", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	for (auto& test : m_Tests) 
	{
		if (ImGui::Button(test.first.c_str())) {
			m_CurrentTest = test.second();
		}
	}
	ImGui::End();
}


TestMenu::~TestMenu()
{
	if (m_CurrentTest != this)
	{
		m_CurrentTest->~Test();
	}
}