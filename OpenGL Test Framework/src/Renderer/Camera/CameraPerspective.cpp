#include "CameraPerspective.h"


#include "glm/gtc/matrix_transform.hpp"

#include "glm/ext/quaternion_common.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"

CameraPerspective::CameraPerspective(Window* window, glm::vec3 startPos)
	:m_Window(window),
	m_WindowData(m_Window->GetWindowData()),
	m_Pos(startPos)
{
	this->UpdateProjection();
	this->UpdateView();
}

void CameraPerspective::UpdateCamera()
{
	if (m_Window->GetKey(GLFW_KEY_A) == GLFW_PRESS) {
		m_Pos -= m_Speed * m_Right;
		this->UpdateView();
	}

	if (m_Window->GetKey(GLFW_KEY_D) == GLFW_PRESS) {
		m_Pos += m_Speed * m_Right;
		this->UpdateView();
	}

	if (m_Window->GetKey(GLFW_KEY_W) == GLFW_PRESS) {
		m_Pos += m_Speed * m_Orientation;
		this->UpdateView();
	}

	if (m_Window->GetKey(GLFW_KEY_S) == GLFW_PRESS) {
		m_Pos -= m_Speed * m_Orientation;
		this->UpdateView();
	}

	if (m_Window->GetKey(GLFW_KEY_SPACE) == GLFW_PRESS) {
		m_Pos += m_Speed * m_WorldUp;
		this->UpdateView();
	}

	if (m_Window->GetKey(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		m_Pos -= m_Speed * m_WorldUp;
		this->UpdateView();
	}

	if (m_Window->GetMouseButton(GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		m_Window->DisableCursor();

		if (m_FirstClick) {
			m_LastMousePos = m_Window->GetMousePos();
			m_FirstClick = false;
		}
		
		glm::vec2 deltaPos = m_LastMousePos - m_Window->GetMousePos();
		m_Window->SetMousePos(m_LastMousePos);

		float deltaPitch = m_Sensitivity * deltaPos.y;
		float deltaYaw = m_Sensitivity * deltaPos.x;

		m_Pitch += deltaPitch;
		if (m_Pitch > m_PitchMax) {
			m_Pitch = m_PitchMax;
			deltaPitch = 0;
		}
		else if (m_Pitch < -m_PitchMax) {
			m_Pitch = -m_PitchMax;
			deltaPitch = 0;
		}

		m_Yaw = glm::mod(m_Yaw + deltaYaw, 360.0f);

		m_Rotation = 
			glm::angleAxis(glm::radians(-deltaYaw), m_WorldUp)
			* 
			glm::angleAxis(glm::radians(-deltaPitch), m_Right);

		m_Orientation = m_Orientation * glm::mat3_cast(m_Rotation);
		//printf("Vec3: %f|%f|%f\tPitch: %f, Yaw: %f\n", m_Orientation.x, m_Orientation.y, m_Orientation.z, m_Pitch, m_Yaw);

		this->UpdateView();
	}

	if (m_Window->GetMouseButton(GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE) {
		m_Window->ShowCursor();
		m_FirstClick = true;
	}
}

void CameraPerspective::UpdateProjection()
{
	m_Projection = glm::perspective(glm::radians(m_FOV), m_WindowData->m_Aspect, 0.1f, 1000.0f);

	m_ViewProjection = m_Projection * m_View;
}

void CameraPerspective::UpdateView()
{
	//m_Direction = glm::normalize(m_Pos - m_Target); //points in opposite direction so target to pos
	//m_Right = glm::normalize(glm::cross(m_WorldUp, m_Direction));
	
	
	//m_Up = glm::cross(m_Orientation, m_Right);
	m_Right = glm::normalize(glm::cross(m_Orientation, m_Up ));

	m_View = glm::lookAt(m_Pos, m_Pos + m_Orientation, m_Up);

	//m_View = glm::translate(glm::mat4(1.0f), m_Pos)
	//	* glm::mat4_cast(m_Rotation);

	//m_View = glm::translate(glm::mat4(1.0f), m_Pos);
	//m_View = glm::rotate(m_View, glm::radians(m_Rotation), { 0.0f, 0.0f, 1.0f });

	//m_View = glm::inverse(m_View);

	m_ViewProjection = m_Projection * m_View;
}