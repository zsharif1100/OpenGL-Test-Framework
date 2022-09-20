#pragma once

#include "Test.h"

class TestClearColor : public Test
{
public:
	TestClearColor();

	void OnRender(Window& window) override;
	void OnImGuiRender(Window& window) override;

private:
	float m_ClearColor[4];
};

