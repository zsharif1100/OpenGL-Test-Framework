#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GLErrorManager.h"

#include <array>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

#include "Renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestMenu.h"
#include "tests/TestClearColor.h"
#include "tests/TestSnake.h"


void error_callback(int error, const char* description) {
	fprintf(stderr, "error: %s\n", description);
}

int main() {

	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		std::cout << "GLFW Initialization failed!" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "OpenGL", NULL, NULL);
	if (!window) {
		std::cout << "Window creation failed!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);


	if (GLEW_OK != glewInit()) {
		std::cout << "GLEW Initialization creation failed!" << std::endl;
		return -1;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	{

		Renderer renderer;

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //src RGBA is set to src alpha and dest RGBA is set to 1 - src alpha.

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplGlfwGL3_Init(window, true);

		ImGui::StyleColorsDark();
		  
		//Test implementation

		Test* currentTest = new TestSnake();
		TestMenu* tMenu = new TestMenu(currentTest);
		//currentTest = tMenu;

		tMenu->RegisterTest<TestClearColor>("Clear color");
		tMenu->RegisterTest<TestSnake>("Snake Game");

		while (!glfwWindowShouldClose(window)) {
			renderer.Clear();
			ImGui_ImplGlfwGL3_NewFrame();
			

			currentTest->OnRender(renderer);
			currentTest->OnUpdate(window, 0.05f);

			currentTest->OnImGuiRender();

			if (currentTest != tMenu)
			{
				if (glfwGetKey(window, GLFW_KEY_ESCAPE))
				{
					delete currentTest;
					currentTest = tMenu;
				}
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			glfwPollEvents();
		}


		//Something here to delete everything?

	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
}