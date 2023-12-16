#include "egpch.h"

#include "Engine/Core/Core.h"
#include "Engine/Renderer/OrthographicCameraController.h"
#include "glad/glad.h"
#include "Engine/Core/KeyCodes.h"

namespace eg {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), 
		m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top), m_Rotation(rotation)
	{
	}

	

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		EG_PROFILE_FUNCTION();
		if (Input::IsKeyPressed(KeyCode::A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraSpeed * (float)ts * m_ZoomLevel;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraSpeed * (float)ts * m_ZoomLevel;
		}
		if (eg::Input::IsKeyPressed(KeyCode::D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraSpeed * (float)ts * m_ZoomLevel;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraSpeed * (float)ts * m_ZoomLevel;
		}
		if (eg::Input::IsKeyPressed(KeyCode::W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraSpeed * (float)ts * m_ZoomLevel;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraSpeed * (float)ts * m_ZoomLevel;
		}
		if (eg::Input::IsKeyPressed(KeyCode::S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraSpeed * (float)ts * m_ZoomLevel;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraSpeed * (float)ts * m_ZoomLevel;
		}
		
		if (m_Rotation) {
			if (eg::Input::IsKeyPressed(KeyCode::Q))
				m_CameraRotation += m_CameraRotationSpeed * (float)ts;
			if (eg::Input::IsKeyPressed(KeyCode::E))
				m_CameraRotation -= m_CameraRotationSpeed * (float)ts;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
		
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EG_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		RecalculateView();
	}

	void OrthographicCameraController::RecalculateView()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		EG_PROFILE_FUNCTION();
		if (e.GetYOffset() > 0)
			m_ZoomLevel /= m_ZoomChange;
		else
			m_ZoomLevel *= m_ZoomChange;

		
		RecalculateView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		EG_PROFILE_FUNCTION();
		
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

	
}