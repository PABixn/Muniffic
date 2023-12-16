#include "egpch.h"
#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace eg {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		EG_PROFILE_FUNCTION();
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		EG_PROFILE_FUNCTION();
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		//RecalculateViewMatrix();
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		EG_PROFILE_FUNCTION();
		glm::mat4 empty(1.0f);
		glm::mat4 transform = glm::translate(empty, m_Position) *
		glm::rotate(empty, glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}