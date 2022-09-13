#pragma once

#include "Test.h"

class TestClearColor : public Test
{
public:
	TestClearColor();

	void OnRender(Renderer& renderer) override;
	void OnImGuiRender() override;

private:
	float m_ClearColor[4];
};

