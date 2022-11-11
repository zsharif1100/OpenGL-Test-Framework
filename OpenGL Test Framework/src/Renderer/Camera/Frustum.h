#pragma once

struct Frustum {
	float left;		float right;
	float bottom;	float top;
	float near;		float far;

	Frustum()
		:left(-1.0f), right(1.0f), bottom(-1.0f), top(1.0f), near(-1.0f), far(1.0f) {}

	Frustum(float l, float r, float b, float t, float n, float f)
		:left(l), right(r), bottom(b), top(t), near(n), far(f) {}
};