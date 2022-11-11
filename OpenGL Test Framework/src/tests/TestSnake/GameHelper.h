#pragma once

#include <glm/glm.hpp>

enum class direction {
	LEFT, DOWN, RIGHT, UP
};

enum class difficulty {
	EASY = 150, MEDIUM = 100, HARD = 75, EXTREME = 50
};

static const glm::vec3 dirVectors[4] = {
	{-1.0f, 0.0f, 0.0f },
	{ 0.0f,-1.0f, 0.0f },
	{ 1.0f, 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f },
};