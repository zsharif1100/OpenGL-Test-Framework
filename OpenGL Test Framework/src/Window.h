#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

struct WindowData {
	std::string m_Title;
	int m_Width;
	int m_Height;

	int vp_X;
	int vp_Y;
	int vp_Width;
	int vp_Height;

	WindowData(const char* title = "Default Window", int width = 1280, int height = 720) {
		m_Title = title;
		m_Width = vp_Width = width;
		m_Height = vp_Height = height;
		vp_X = vp_Y = 0;
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

