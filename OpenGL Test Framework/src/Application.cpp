#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GLErrorManager.h"

#include <iostream>

#include "Window.h"
#include "Renderer/Renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestMenu/TestMenu.h"
#include "tests/TestClearColor/TestClearColor.h"
#include "tests/TestSnake/TestSnake.h"

int main() {

	Renderer::Init();

	bool fullscreen = false;
	GLFWmonitor* primaryMonitor = fullscreen == true ? glfwGetPrimaryMonitor() : NULL;

	Window window(1280, 720, "OpenGL Test Framework");
	//Window window(1920, 1080, "OpenGL Test Framework", primaryMonitor);

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //src RGBA is set to src alpha and dest RGBA is set to 1 - src alpha.


	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfwGL3_Init(window.GetWindow(), true);
	 
	ImGui::StyleColorsDark();

	//Test implementation

	Test* currentTest = nullptr;
	TestMenu* tMenu = new TestMenu(currentTest);
	currentTest = tMenu;

	//tMenu->RegisterTest<TestClearColor>("Clear color");
	tMenu->RegisterTest<TestSnake>("Snake Game");

	while (!window.ShouldClose()) {
		Renderer::Clear();

		ImGui_ImplGlfwGL3_NewFrame();

		currentTest->OnRender(window);
		currentTest->OnUpdate(window, 0.05f);

		currentTest->OnImGuiRender(window);
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

	window.~Window();

	Renderer::Shutdown();
}