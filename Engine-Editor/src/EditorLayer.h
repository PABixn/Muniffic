#pragma once
#include "Engine.h"
#include "Panels/UnsavedChangesPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Engine/Renderer/EditorCamera.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/AddResourcePanel.h"
#include "Panels/ConsolePanel.h"


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

		SceneHierarchyPanel* GetSceneHierarchyPanel() { return &m_SceneHierarchyPanel; }
		UUID GetCurrentDirectoryUUID() { m_ContentBrowserPanel->GetCurrentDirectoryUUID(); }

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		void OnOverlayRender();

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveAs();
		void Save();

		void NewProject();
		bool OpenProject();
		void OpenProject(const std::filesystem::path& path);
		void SaveProjectAs();
		void SaveProject();

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);

		void OnScenePlay();
		void OnSceneStop();
		void OnSceneSimulate();
		void OnScenePause();

		void OnDuplicateEntity();

		void CloseAddResourcePanel();

		//UI Panels
		void UI_Toolbar();
	private:
		friend class UnsavedChangesPanel;
		friend class ConsolePanel;
		OrthographicCameraController m_Camera;
		//Temp
		Ref<Shader> m_Shader;
		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		Ref<VertexArray> m_VA;
		Ref<Texture2D> m_Texture;
		Ref<Texture2D> m_IconPlay, m_IconStop, m_IconPause, m_IconSimulate, m_IconStep;
		Ref<Texture2D> m_LogoIcon;
		Ref<FrameBuffer> m_FrameBuffer;

		ImFont* m_PoppinsRegularFont;
		ImFont* m_PoppinsMediumFont;
		ImFont* m_PoppinsSemiBoldFont;
		ImFont* m_PoppinsExtraBoldFont;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_RuntimeScene;
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

		Ref<ResourceSystemState> resourceSystemState;

		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;

		int m_GizmoType = -1;

		bool m_ShowPhysicsColliders = false;
		bool m_ShowAxis = true;
		bool m_ShowGrid = true;

		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		Scope<ContentBrowserPanel> m_ContentBrowserPanel;
		Scope<AddResourcePanel> m_AddResourcePanel;
		/*Scope<DeleteFilePanel> m_DeleteFilePanel;
		RenameFolderPanel* m_RenameFolderPanel;
		DeleteDirectoryPanel* m_DeleteDirectoryPanel;
		RenameResourcePanel* m_RenameResourcePanel;*/
		Scope<ConsolePanel> m_ConsolePanel;

		enum class SceneState
		{
			Edit = 0, Play = 1, Simulate = 2
		};

		SceneState m_SceneState = SceneState::Edit;

		friend class AddResourcePanel;
	public:
		UnsavedChangesPanel* m_UnsavedChangesPanel;
		UnsavedChangesPanel* GetUnsavedChangesPanel() { return m_UnsavedChangesPanel; };
		
	//styles
	public:
		ImVec4 m_LightTextShade = ImVec4(0.7765f, 0.7333f, 0.8863f, 1.0f);
		ImVec4 m_DarkShade = ImVec4(0.125f, 0.102f, 0.188f, 1.0f);
		ImVec4 m_LightShade = ImVec4(0.251f, 0.212f, 0.349f, 1.0f);
		ImVec4 m_NormalShade = ImVec4(0.192f,0.157f,0.267f,1.0f);
	};

}