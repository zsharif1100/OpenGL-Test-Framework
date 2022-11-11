#pragma once

#include "Camera.h"

#include "glm/gtc/quaternion.hpp"

#include "Window.h"
#include "Frustum.h"


class CameraPerspective : public Camera
{
public:
	CameraPerspective() = delete;
	CameraPerspective(Window* window, glm::vec3 startPos = glm::vec3(0.0f, 0.0f, -100.0f));
	//CameraPerspective(Window& window, float left, float right,
	//	float bottom, float top, float zNear, float zFar);

	inline const glm::vec3 GetPos() const override { return m_Pos; };

	void UpdateCamera();
	inline const glm::mat4 GetViewProj() const override { return m_ViewProjection; }
	inline void SetCameraPos(glm::vec3 pos) { m_Pos = pos; this->UpdateView(); };

private:
	Window* m_Window;
	WindowData* m_WindowData;
	//Frustum m_Frustum;

	float m_FOV = 45.0f;

	float m_Speed = 0.05f;
	glm::vec3 m_Pos;
	glm::vec3 m_Target = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);;
	glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_Right;

	glm::vec3 m_Orientation = glm::vec3(0.0f, 0.0f, 1.0f);
	const float m_PitchMax = 85.0f;

	float m_Pitch = 0;
	float m_Yaw = 0;
	glm::quat m_Rotation;

	bool m_FirstClick = true;
	glm::vec2 m_LastMousePos;
	float m_Sensitivity = 0.1f;

	glm::mat4 m_Projection;
	glm::mat4 m_View;
	glm::mat4 m_ViewProjection;

	void UpdateProjection();
	void UpdateView();
};