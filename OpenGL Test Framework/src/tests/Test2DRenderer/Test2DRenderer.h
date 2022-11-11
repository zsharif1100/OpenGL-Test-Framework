#pragma once

#include "tests/Test.h"
#include "glm/glm.hpp"

#include <vector>

class Test2DRenderer : public Test
{
public:
	Test2DRenderer(Window* window);

	void OnUpdate(float dT) override;
	void OnRender() override;
	void OnImGuiRender() override;
private:
	Window* m_Window;
	WindowData* m_WindowData;

	std::vector<Quad> m_Quads;
	Quad m_Quad;
	float m_Time = 0;

	
	CameraOrthographic m_Camera;
};
