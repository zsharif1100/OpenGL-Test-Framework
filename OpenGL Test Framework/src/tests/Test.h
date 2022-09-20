#pragma once

#include <vector>
#include <string>

#include "Window.h"
#include "Renderer.h"

class Test {
public:
	Test() {}
	virtual ~Test() {}

	virtual void OnUpdate(Window& window, float dT) {}
	virtual void OnRender(Window& Window) {}
	virtual void OnImGuiRender(Window& window) {}
};
