#pragma once
#include "Engine.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Engine/Renderer/EditorCamera.h"
#include "Panels/ContentBrowserPanel.h"

namespace eg {
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual~EditorLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveAs();
		void Save();

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);

		void OnScenePlay();
		void OnSceneStop();

		void OnDuplicateEntity();

		//UI Panels
		void UI_Toolbar();
	private:
		OrthographicCameraController m_Camera;
		//Temp
		Ref<Shader> m_Shader;
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		Ref<VertexArray> m_VA;
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_IconPlay, m_IconStop;
		Ref<FrameBuffer> m_FrameBuffer;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;
		std::filesystem::path m_ActiveScenePath;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCameraEntity;

		Entity m_HoveredEntity;

		bool m_PrimaryCamera = true;

		EditorCamera m_EditorCamera;

		struct ProfileResult
		{
			const char* Name;
			float Time;
		};

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		std::vector<ProfileResult> m_ProfileResults;

		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;

		int m_GizmoType = -1;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		enum class SceneState
		{
			Edit = 0, Play = 1
		};

		SceneState m_SceneState = SceneState::Edit;
	};
}