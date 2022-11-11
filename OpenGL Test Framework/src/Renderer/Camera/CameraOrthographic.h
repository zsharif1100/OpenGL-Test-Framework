#pragma once

#include "Camera.h"

#include "Window.h"
#include "Frustum.h"

class CameraOrthographic : public Camera
{
public:
	CameraOrthographic() = delete;
	CameraOrthographic(Window* window, float verticalSize, glm::vec3 startPos = glm::vec3(0.0f));
	//CameraOrthographic(Window& window, float left, float right,
	//	float bottom, float top, float zNear, float zFar);

	inline const glm::vec3 GetPos() const override { return m_Pos; };

	void UpdateCamera();
	const glm::mat4 GetViewProj() const override{ return m_ViewProjection; }
	inline void SetCameraPos(glm::vec3 pos) { m_Pos = pos; this->UpdateView(); }

private:
	Window* m_Window;
	WindowData* m_WindowData;
	float m_OrthographicSize;
	Frustum m_Frustum;


	glm::vec3 m_Pos;
	float m_MoveSpeed = 0.2f;


	float m_Orientation = 0.0f;
	float m_RotSpeed = 0.1f;

	float m_Scale = 1.0f;
	float m_ScaleSpeed = 0.001f;


	glm::mat4 m_Projection;
	glm::mat4 m_View;
	glm::mat4 m_ViewProjection;

	void UpdateProjection();
	void UpdateView();

};

