#include "Test2DRenderer.h"

#include "imgui/imgui.h"

Test2DRenderer::Test2DRenderer(Window* window)
	:m_Window(window),
	m_WindowData(m_Window->GetWindowData()),
	m_Camera(window, 400.0f),
	m_Quad(glm::vec3(-200.0f, 0.0f, 0.0f),
		50.0f, 50.0f, "../../../OpenGL Test Framework/res/textures/segment_clear.png")
{

	m_Quads.emplace_back(glm::vec3(0.0f,200.0f, 0.0f),
		100.0f, 100.0f, glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f ));

	m_Quads.emplace_back(glm::vec3(200.0f, 0.0f, 0.0f),
		200.0f, 200.0f, "../../../OpenGL Test Framework/res/textures/blender_suzanne.jpg");

	m_Quads.emplace_back(glm::vec3(0.0f, -200.0f, 0.0f),
		200.0f, 200.0f, glm::vec4(34.0f / 255, 177.0f / 255, 76.0f / 255, 1.0f),
		"../../../OpenGL Test Framework/res/textures/segment_clear.png");
}

void Test2DRenderer::OnUpdate(float dT)
{
	m_Camera.UpdateCamera();
	float currentTime = (float)glfwGetTime();
	float elapsed = currentTime - m_Time;

	if (elapsed > dT) {

		//for (auto& quad : m_Quads) {
		//	quad.Rotate(1.5);
		//}

		//m_Quad.Rotate(1);
		//m_Quad.Scale({ 2, 1, 1 });

		m_Time = currentTime;
	}
}

void Test2DRenderer::OnRender()
{
	Renderer::BeginScene(m_Camera);
	for (auto& quad : m_Quads) {
		Renderer::DrawQuad(quad);
	}
	Renderer::DrawQuad(m_Quad);
	Renderer::EndScene();
}

void Test2DRenderer::OnImGuiRender()
{
	ImGui::SetNextWindowSize(ImVec2((float)m_WindowData->m_Width - m_WindowData->m_VPWidth, (float)m_WindowData->m_Height), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2((float)m_WindowData->m_VPWidth, 0.0f), ImGuiCond_Always);
	ImGui::Begin("Test", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGui::Text("Framerate: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	//for (int i = 0; i < m_Quads.size(); i++)
	//{
	//	glm::vec3* posPtr = m_Quads[i].GetPosPtr();
	//	float posVec[3] = { posPtr->x, posPtr->y, posPtr->z };

	//	ImGui::PushID(i);
	//	ImGui::DragFloat3("Position (xyz)", posVec, 0.01f, -1.0f, 1.0f);

	//	ImGui::PopID();
	//}
	ImGui::End();
}
