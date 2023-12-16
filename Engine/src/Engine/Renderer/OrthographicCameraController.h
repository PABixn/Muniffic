#pragma once
#include "Engine/Renderer/OrthographicCamera.h"
#include "Engine/Core/Timestep.h"
#include "Engine/Core/Input.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/ApplicationEvent.h"

namespace eg {

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }

	};

	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);
		OrthographicCamera& GetCamera()  { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		void OnResize(float width, float height);

		void SetZoomLevel(float level) { m_ZoomLevel = level; RecalculateView(); }
		float  GetZoomLevel() const { return m_ZoomLevel; }

		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }
	private:
		void RecalculateView();
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		bool m_Rotation;
		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;
		float m_CameraSpeed = 5.0f;
		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 180.0f;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_ZoomChange = 1.2f;
	};
}