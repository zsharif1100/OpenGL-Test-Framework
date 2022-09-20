#include "TestMenu.h"

#include "imgui/imgui.h"

TestMenu::TestMenu(Test*& currentTest)
	:m_CurrentTest(currentTest) {}

void TestMenu::OnImGuiRender(Window& window)
{
	//ImGui::SetNextWindowPos(ImVec2(0.0f, 1.0f), ImGuiCond_Always);
	ImGui::ShowDemoWindow();

	ImGui::SetNextWindowSize(ImVec2(SCONST::IMGUI_WIDTH, SCONST::IMGUI_HEIGHT), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);

	ImGui::Begin("Test", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::Text("Press Esc to return to this menu");
	for (auto& test : m_Tests) 
	{
		if (ImGui::Button(test.first.c_str())) {
			m_CurrentTest = test.second();
		}
	}
	ImGui::End();
}
