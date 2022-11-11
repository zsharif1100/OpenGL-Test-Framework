#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
public:
	virtual const glm::vec3 GetPos() const { return glm::vec3(0.0f); };
	virtual const glm::mat4 GetViewProj() const { return glm::mat4(0.0f); };
};