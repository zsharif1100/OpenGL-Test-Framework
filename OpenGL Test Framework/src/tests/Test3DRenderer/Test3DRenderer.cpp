#include "Test3DRenderer.h"

#include "imgui/imgui.h"

Test3DRenderer::Test3DRenderer(Window* window)
	:m_Window(window),
	m_WindowData(m_Window->GetWindowData()),
	m_Camera(window)
{
	m_Cubes.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f),
		10.0f, 10.0f, 10.0f, glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));

	m_Cubes.emplace_back(glm::vec3(0.0f, -10.0f, 0.0f),
		100.0f, 10.0f, 100.0f, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

	m_Lights.emplace_back(glm::vec3(0.0f, 10.0f, 0.0f),
		5.0f, 5.0f, 5.0f, glm::vec4(1.0f, 0.5f, 0.5f, 1.0f));
}

void Test3DRenderer::OnUpdate(float dT)
{
	m_Camera.UpdateCamera();
	float currentTime = (float)glfwGetTime();
	float elapsed = currentTime - m_Time;

	if (elapsed > dT * 0.1) {
		m_Time = currentTime;

		for (auto& cubeLight : m_Lights) {
			cubeLight.SetPos(glm::vec3(sin(m_Time) * 20, 10.0f, cos(m_Time) * 20));
		}
	}
}

void Test3DRenderer::OnRender()
{
	Renderer::BeginScene(m_Camera);

	for (auto& cubeLight : m_Lights) {
		Renderer::DrawCubeLight(cubeLight);
	}

	for (auto& cube : m_Cubes) {
		Renderer::DrawCube(cube);
	}

	Renderer::EndScene();
}

void Test3DRenderer::OnImGuiRender()
{
	ImGui::SetNextWindowSize(ImVec2((float)m_WindowData->m_Width - m_WindowData->m_VPWidth, (float)m_WindowData->m_Height), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2((float)m_WindowData->m_VPWidth, 0.0f), ImGuiCond_Always);

	ImGui::Begin("Test", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::Text("Framerate: %.3f ms/frame (%.1f FPS)", 
		1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}
