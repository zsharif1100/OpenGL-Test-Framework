#pragma once

#include <vector>
#include <string>

#include "Renderer.h"

class Test {
public:
	Test() {}
	virtual ~Test() {}

	virtual void OnUpdate(GLFWwindow* window, float dT) {}
	virtual void OnRender(Renderer& renderer) {}
	virtual void OnImGuiRender() {}
};
