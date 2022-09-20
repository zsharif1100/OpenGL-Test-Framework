#pragma once

#include "Test.h"

#include <vector>
#include <functional>

class TestMenu : public Test
{
public:
	TestMenu() = delete;
	TestMenu(Test*& currentTest);

	void OnImGuiRender(Window& window) override;

	inline Test*& CurrentTest() { return m_CurrentTest; }

	template<typename T>
	void RegisterTest(std::string name) 
	{
		m_Tests.push_back(std::make_pair(name, []() {return new T; }));
	}
private:
	Test*& m_CurrentTest;
	std::vector<std::pair<std::string, std::function<Test*()> >> m_Tests;
};

