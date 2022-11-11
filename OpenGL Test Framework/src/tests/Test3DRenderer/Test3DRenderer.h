#pragma once

#include "tests/Test.h"

class Test3DRenderer : public Test
{
public:
	Test3DRenderer(Window* window);

	void OnUpdate(float dT) override;
	void OnRender() override;
	void OnImGuiRender() override;
private:
	Window* m_Window;
	WindowData* m_WindowData;

	std::vector<Cube> m_Cubes;
	std::vector<Cube> m_Lights;
	float m_Time = 0;


	CameraPerspective m_Camera;
};