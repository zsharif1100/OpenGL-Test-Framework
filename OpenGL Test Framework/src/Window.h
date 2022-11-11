#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <iostream>
#include <string>

#include "glm/glm.hpp"

struct WindowData {
	std::string m_Title;
	int m_Width;
	int m_Height;

	int m_VPWidth;
	int m_VPHeight;
	int m_VPX;
	int m_VPY;

	float m_Aspect;

	double m_Scroll = 0.0;

	WindowData(const char* title = "Default Window", int width = 1280, int height = 720) {
		m_Title = title;
		m_Width = m_VPWidth = width;
		m_Height = m_VPHeight = height;
		m_Aspect = (float)m_VPWidth / m_VPHeight;
		m_VPX = m_VPY = 0;

	}

	void setData(int width, int height, int vpX, int vpY, int vpWidth, int vpHeight) {
		m_Width = width; m_Height = height;
		m_VPWidth = vpWidth; m_VPHeight = vpHeight;
		m_Aspect = (float)m_VPWidth / m_VPHeight;

		m_VPX = vpX; m_VPY = vpY;
	}

	void addScroll(double yoffset) {
		m_Scroll += yoffset;
		std::cout << "Scroll detected" << std::endl;
	}
};

class Window
{
public:
	//Window();
	Window(int width, int height, const char* title, GLFWmonitor* monitor = NULL, GLFWwindow* share = NULL);
	~Window();

	void MakeContextCurrent() const;
	void SetViewPort(int x, int y, int width, int height);
	bool ShouldClose() const;
	void SwapBuffers() const;
	void PollEvents() const;


	inline void ShowCursor() const { glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }
	inline void HideCursor() const { glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); }
	inline void DisableCursor() const { glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }
	
	inline glm::vec2 GetMousePos() const {
		double x;
		double y;
		glfwGetCursorPos(m_Window, &x, &y); 
		return { x, y };
	}

	inline void SetMousePos(glm::vec2 newPos) { glfwSetCursorPos(m_Window, newPos.x, newPos.y); }
	
	inline int GetKey(const int& key) const { return glfwGetKey(m_Window, key); }
	int GetMouseButton(const int& button) const { return glfwGetMouseButton(m_Window, button); }

	inline WindowData* GetWindowData() { return &m_Data; }

	//void AddApplication() const;

	//inline const WindowData& GetWindowData() const { return m_Data; }

	//void OnResize(GLFWwindow* window, int width, int height);

	inline GLFWwindow* GetWindow() const { return m_Window; };
private:
	GLFWwindow* m_Window;
	WindowData m_Data;
};