#include "TestMenu.h"

#include "imgui/imgui.h"

TestMenu::TestMenu(Test*& currentTest)
	:m_CurrentTest(currentTest) {}

void TestMenu::OnImGuiRender()
{
	ImGui::Begin("Test");

	for (auto& test : m_Tests) 
	{
		if (ImGui::Button(test.first.c_str())) {
			m_CurrentTest = test.second();
		}
	}

	ImGui::End();
}
