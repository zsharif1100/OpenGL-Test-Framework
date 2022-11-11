#include "CameraOrthographic.h"

#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

CameraOrthographic::CameraOrthographic(Window* window, float verticalSize, glm::vec3 startPos)
	:m_Window(window),
	m_WindowData(m_Window->GetWindowData()),
	m_OrthographicSize(verticalSize),
	m_Frustum(-m_OrthographicSize * m_WindowData->m_Aspect, 
		m_OrthographicSize * m_WindowData->m_Aspect,
		-m_OrthographicSize, m_OrthographicSize, -1.0, 1.0f),
	m_Pos(startPos)
{

	this->UpdateProjection();
	this->UpdateView();

}

void CameraOrthographic::UpdateCamera()
{
	if (m_Window->GetKey(GLFW_KEY_A)) {
		m_Pos.x -= m_MoveSpeed;
		this->UpdateView();
	}
	if (m_Window->GetKey(GLFW_KEY_D)) {
		m_Pos.x += m_MoveSpeed;
		this->UpdateView();
	}
	if (m_Window->GetKey(GLFW_KEY_W)) {
		m_Pos.y += m_MoveSpeed;
		this->UpdateView();
	}
	if (m_Window->GetKey(GLFW_KEY_S)) {
		m_Pos.y -= m_MoveSpeed;
		this->UpdateView();
	}

	if (m_Window->GetKey(GLFW_KEY_Q)) {
		m_Orientation = glm::mod(m_Orientation - m_RotSpeed, 360.0f);
		this->UpdateView();
	}
	if (m_Window->GetKey(GLFW_KEY_E)) {
		m_Orientation = glm::mod(m_Orientation + m_RotSpeed, 360.0f);
		this->UpdateView();
	}
	//if (m_Window->GetKey(GLFW_KEY_R)) {
	//	if (m_Scale < 10.0f)
	//		m_Scale += m_ScaleSpeed;

	//	this->UpdateView();
	//}
	//if (m_Window->GetKey(GLFW_KEY_F)) {
	//	if (m_Scale > 0.2f)
	//		m_Scale -= m_ScaleSpeed;

	//	this->UpdateView();
	//}
	m_Scale +=  (float)m_WindowData->m_Scroll * m_ScaleSpeed;

}

void CameraOrthographic::UpdateProjection()
{
	m_Projection = glm::ortho(m_Frustum.left, m_Frustum.right,
		m_Frustum.bottom, m_Frustum.top, m_Frustum.near, m_Frustum.far);
	m_ViewProjection = m_Projection * m_View;
}

void CameraOrthographic::UpdateView()
{
	m_View = glm::scale(glm::mat4(1.0f), { m_Scale, m_Scale, 1.0f });
	m_View = glm::rotate(m_View, glm::radians(m_Orientation), {0.0f, 0.0f, 1.0f});
	m_View = glm::translate(m_View, m_Pos);

	m_View = glm::inverse(m_View);

	m_ViewProjection = m_Projection * m_View;
}