#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

struct WindowData {
	std::string m_Title;
	int m_Width;
	int m_Height;

	int m_VPWidth;
	int m_VPHeight;
	int m_VPX;
	int m_VPY;

	WindowData(const char* title = "Default Window", int width = 1280, int height = 720) {
		m_Title = title;
		m_Width = m_VPWidth = width;
		m_Height = m_VPHeight = height;
		m_VPX = m_VPY = 0;
	}

	void  setData(int width, int height, int vpX, int vpY, int vpWidth, int vpHeight) {
		m_Width = width; m_Height = height;
		m_VPWidth = vpWidth; m_VPHeight = vpHeight;
		m_VPX = vpX; m_VPY = vpY;
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
	int GetKey(const int& key) const;
	//void AddApplication() const;

	//inline const WindowData& GetWindowData() const { return m_Data; }

	//void OnResize(GLFWwindow* window, int width, int height);

	inline GLFWwindow* GetWindow() const { return m_Window; };
private:
	GLFWwindow* m_Window;
	WindowData m_Data;
};

