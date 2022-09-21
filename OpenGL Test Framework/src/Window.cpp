#include "Window.h"

#include "GLErrorManager.h"
#include "Renderer/Renderer.h"

#include <iostream>

static bool glew_Initialised = false;

void OnResizeInternal(GLFWwindow* window, int width, int height)
{
	int ImGui_Width = 214;
	int reducedWidth = width - ImGui_Width;


	float desiredRatio = 4.0f / 3.0f;
	float aspectRatio = (float)reducedWidth / height;

	int excessWidth, excessHeight;

	if (reducedWidth > (height * desiredRatio) ) {
		//too much width for 4/3
		excessWidth = reducedWidth - (int)(height * desiredRatio);
		excessHeight = 0;
	}
	else { 
		//too much height for 4/3
		excessHeight = height - (int)(reducedWidth / desiredRatio);
		excessWidth = 0;
	}

	int vpWidth = reducedWidth - excessWidth;
	int vpHeight = height - excessHeight;
	int vpX = 0;
	int vpY = excessHeight;

	GLCall(glViewport(vpX, vpY, vpWidth, vpHeight));

	WindowData* wData = (WindowData * )glfwGetWindowUserPointer(window);

	wData->setData(width, height, vpX, vpY, vpWidth, vpHeight);
}

Window::Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
	:m_Data(title, width, height)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(width, height, title, monitor, share);
	glfwSetWindowSizeLimits(m_Window, 1280, 720, GLFW_DONT_CARE, GLFW_DONT_CARE);


	this->MakeContextCurrent();
	glfwSwapInterval(0);

	//glfwSetWindowSizeCallback(m_Window, OnResizeInternal);

	glfwSetWindowUserPointer(m_Window, &m_Data);

	OnResizeInternal(m_Window, m_Data.m_Width, m_Data.m_Height);
	glfwSetFramebufferSizeCallback(m_Window, OnResizeInternal);

	if (!glew_Initialised) {
		if (GLEW_OK != glewInit()) {
			std::cout << "GLEW Initialization failed!" << std::endl;
		}
		else {
			std::cout << glGetString(GL_VERSION) << std::endl;
			glew_Initialised = true;
		}
	}

}

Window::~Window()
{
	glfwDestroyWindow(m_Window);
}

void Window::MakeContextCurrent() const
{
	glfwMakeContextCurrent(m_Window);
}

void Window::SetViewPort(int x, int y, int width, int height)
{
	GLCall(glViewport(x, y, width, height));
}

bool Window::ShouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}

void Window::SwapBuffers() const
{
	glfwSwapBuffers(m_Window);
}

void Window::PollEvents() const
{
	glfwPollEvents();
}

int Window::GetKey(const int& key) const
{
	return glfwGetKey(m_Window, key);
}

//void Window::OnResize()
//{
//
//}
