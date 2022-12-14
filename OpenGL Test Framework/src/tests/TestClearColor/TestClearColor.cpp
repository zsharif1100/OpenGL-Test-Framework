#include "TestClearColor.h"

#include "GL/glew.h"
#include "GLErrorManager.h"

#include "imgui/imgui.h"

TestClearColor::TestClearColor(Window* window)
	:m_Window(window), m_ClearColor{ 1.0f, 1.0f, 1.0f, 1.0f } {}

void TestClearColor::OnRender()
{
	GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void TestClearColor::OnImGuiRender()
{
	ImGui::Begin("Test");
	ImGui::ColorEdit4("Clear color", m_ClearColor);

	ImGui::End();
}
