#include "TestMenu.h"

#include "imgui/imgui.h"

TestMenu::TestMenu(Test*& currentTest)
	:m_CurrentTest(currentTest) {}

void TestMenu::OnImGuiRender(Window& window)
{
	ImGui::ShowDemoWindow();

	WindowData* wData = (WindowData*)glfwGetWindowUserPointer(window.GetWindow());

	ImGui::SetNextWindowSize(ImVec2(wData->m_Width - wData->m_VPWidth, wData->m_Height), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(wData->m_VPWidth, 0.0f), ImGuiCond_Always);
	ImGui::Begin("Test", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	for (auto& test : m_Tests) 
	{
		if (ImGui::Button(test.first.c_str())) {
			m_CurrentTest = test.second();
		}
	}
	ImGui::End();
}
