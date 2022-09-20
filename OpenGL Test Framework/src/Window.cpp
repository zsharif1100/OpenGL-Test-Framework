#include "Window.h"

#include "GLErrorManager.h"
#include "Renderer.h"

#include <iostream>


void OnResizeInternal(GLFWwindow* window, int width, int height)
{
	int ImGui_Width = 214;
	width = width - ImGui_Width;

	float desiredRatio = 4.0f / 3.0f;
	float aspectRatio = (float)width / height;

	int excessWidth, excessHeight;

	if (width > (height * desiredRatio) ) { //too much width for 4/3
		excessWidth = width - height * desiredRatio;
		excessHeight = 0;
	}
	else { //too much height for 4/3
		excessHeight = height - width / desiredRatio;
		excessWidth = 0;
	}

	int newWidth = width - excessWidth;
	int newHeight = height - excessHeight;
	int newX = 0;
	int newY = 0;

	//printf("New Width: %i ExcessWidth: %i\nNewHeight: %i ExcessHeight: %i\n\n", newWidth, excessWidth, newHeight, excessHeight);

	GLCall(glViewport(newX, newY, newWidth, newHeight));

	WindowData* wData = (WindowData * )glfwGetWindowUserPointer(window);

	//wData->vp_X = newX;
	//wData->vp_Y = newY;

	//wData->vp_Width = newWidth;
	//wData->vp_Width	= newWidth;
	//m_ViewportData.Update(newX, newY, newWidth, newHeight);
	//std::cout << "Window has been resized!" << std::endl;
}

Window::Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
	:m_Data(title, width, height)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_Window = glfwCreateWindow(width, height, title, monitor, share);
	glfwSetWindowSizeLimits(m_Window, 854, 480, GLFW_DONT_CARE, GLFW_DONT_CARE);


	this->MakeContextCurrent();
	glfwSwapInterval(0);
	
	//glfwSwapInterval(1);

	//glfwSetWindowSizeCallback(m_Window, OnResizeInternal);
	OnResizeInternal(m_Window, m_Data.m_Width, m_Data.m_Height);
	glfwSetFramebufferSizeCallback(m_Window, OnResizeInternal);

	glfwSetWindowUserPointer(m_Window, &m_Data);

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
