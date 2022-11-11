#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GLErrorManager.h"

#include <iostream>

#include "Renderer/Renderer.h"
#include "Window.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestMenu/TestMenu.h"

int main() {

	Renderer::Init();

	Window window(1280, 720, "OpenGL Test Framework");

	Renderer::InitData();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplGlfwGL3_Init(window.GetWindow(), true);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::StyleColorsDark();

	//Test implementation

	Test* currentTest = nullptr;
	TestMenu* tMenu = new TestMenu(&window, currentTest);
	currentTest = tMenu;

	while (!window.ShouldClose()) {
		Renderer::Clear();

		ImGui_ImplGlfwGL3_NewFrame();

		currentTest->OnUpdate(0.05f);
		currentTest->OnRender();

		currentTest->OnImGuiRender();
		if (currentTest != tMenu)
		{
			if (window.GetKey(GLFW_KEY_ESCAPE))
			{
				delete currentTest;
				currentTest = tMenu;
			}
		}

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		window.SwapBuffers();
		window.PollEvents();
	}

	delete tMenu;

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

	Renderer::ResetData();

	Renderer::Shutdown();
}