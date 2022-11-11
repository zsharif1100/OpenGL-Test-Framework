#pragma once

#include "tests/Test.h"

class TestClearColor : public Test
{
public:
	TestClearColor(Window* window);

	void OnRender() override;
	void OnImGuiRender() override;

private:
	Window* m_Window;
	float m_ClearColor[4];
};

