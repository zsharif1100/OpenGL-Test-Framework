#pragma once

#include "tests/Test.h"

#include <vector>
#include <functional>

class TestMenu : public Test
{
public:
	TestMenu() = delete;
	TestMenu(Window* window, Test*& currentTest);
	~TestMenu();

	void OnImGuiRender() override;

	inline Test*& CurrentTest() { return m_CurrentTest; }
	inline Window* GetWindow() { return m_Window; }

	//template<typename T>
	//void RegisterTest(Window* window, std::string name) 
	//{
	//	m_Tests.push_back(std::make_pair(name.c_str(), []() {return new T; }));
	//}
private:
	Test*& m_CurrentTest;
	Window* m_Window;
	WindowData* m_WindowData;

	std::vector<std::pair<std::string, std::function<Test*()> >> m_Tests;
};

