#pragma once

#include "Window.h"
#include "Renderer/Renderer.h"
#include "glm/glm.hpp"

#include "Renderer/Camera/CameraOrthographic.h"
#include "Renderer/Camera/CameraPerspective.h"


class Test {
public:
	Test() {}
	virtual ~Test() {}

	virtual void OnUpdate(float dT) {}
	virtual void OnRender() {}
	virtual void OnImGuiRender() {}
};
