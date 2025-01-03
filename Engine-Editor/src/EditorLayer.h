#pragma once
#include "Engine.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Engine/Renderer/EditorCamera.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/AddResourcePanel.h"
#include "Panels/ProjectDirectoryPanel.h"
#include "Panels/ConsolePanel.h"
#include "Panels/ResourcesPanels/AnimationEditorPanel.h"
#include "Panels/WelcomingPanel.h"
#include "Panels/NameNewProjectPanel.h"
#include "Panels/AssistantPanel.h"

namespace eg
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event &e) override;

		const std::string CompileCustomScripts(); 
		SceneHierarchyPanel* GetSceneHierarchyPanel() { return &m_SceneHierarchyPanel; }
		Ref<ContentBrowserPanel> GetContentBrowserPanel() { return m_ContentBrowserPanel; }
		UUID GetCurrentDirectoryUUID() { m_ContentBrowserPanel->GetCurrentDirectoryUUID(); }
		void SetIsWindowTryingToClose(bool value) { IsWindowTryingToClose = value; } // Helper for unsaved changes popup
	private:
		bool OnKeyPressed(KeyPressedEvent &e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent &e);
		void OnOverlayRender();

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path &path);
		void SaveAs();
		void Save();

		bool CreateCmakelists(const std::filesystem::path path); 

		void NewProject();
		bool OpenProject();
		void OpenProject(const std::filesystem::path &path);
		void SaveProjectAs();
		void SaveProject();

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path &path);

		void OnScenePlay();
		void OnSceneStop();
		void OnSceneSimulate();
		void OnScenePause();

		void OnDuplicateEntity();

		void CloseAddResourcePanel();

		// UI Panels
		void UI_Toolbar();

		void LoadFonts();
	private:
		friend class ConsolePanel;
		friend class AddResourcePanel;
	private:
		Ref<Project> m_CurrentProject = nullptr;
		std::filesystem::path m_CustomScriptsDirectory; // absolute
		OrthographicCameraController m_Camera;
		// Temp
        Ref<Shader> m_Shader = nullptr;
		glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
        Ref<VertexArray> m_VA = nullptr;
        Ref<Texture2D> m_Texture = nullptr;
        Ref<Texture2D> m_IconPlay = nullptr, m_IconStop = nullptr, m_IconPause = nullptr, m_IconSimulate = nullptr, m_IconStep = nullptr;
        Ref<FrameBuffer> m_FrameBuffer = nullptr;

		Ref<Scene> m_ActiveScene = nullptr;
		Ref<Scene> m_RuntimeScene = nullptr;
		Ref<Scene> m_EditorScene = nullptr;
		std::filesystem::path m_ActiveScenePath;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCameraEntity;

		Entity m_HoveredEntity;

		bool m_PrimaryCamera = true;

		EditorCamera m_EditorCamera;

		struct ProfileResult
		{
			const char *Name;
			float Time;
		};

		glm::vec2 m_ViewportSize = {0.0f, 0.0f};
        glm::vec2 m_ViewportBounds[2] = {{ 0.0f, 0.0f },{0.0f, 0.0f}};

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;
		std::vector<ProfileResult> m_ProfileResults;

		Ref<ResourceSystemState> resourceSystemState = nullptr;

		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;

		int m_GizmoType = -1;

		bool m_ShowPhysicsColliders = false;
		bool m_ShowAxis = false;
		bool m_ShowGrid = false;

		enum class SceneState
		{
			Edit = 0, Play = 1, Simulate = 2
		};

		SceneState m_SceneState = SceneState::Edit;
		friend class AddResourcePanel;
	public:
		ImVec4 m_LightTextShade = ImVec4(0.7765f, 0.7333f, 0.8863f, 1.0f);
		ImVec4 m_LightShade = ImVec4(0.251f, 0.212f, 0.349f, 1.0f);
		ImVec4 m_NormalShade = ImVec4(0.192f, 0.157f, 0.267f, 1.0f);
		ImVec4 m_DarkShade = ImVec4(0.125f, 0.102f, 0.188f, 1.0f);
		
		ImFont* m_PoppinsLightFont = nullptr;
		ImFont* m_PoppinsRegularFont = nullptr;
		ImFont* m_PoppinsRegularFontBig = nullptr;
		ImFont* m_PoppinsMediumFont = nullptr;
		ImFont* m_PoppinsSemiBoldFont = nullptr;
		ImFont* m_PoppinsSemiBoldFontBig = nullptr;
		ImFont* m_PoppinsExtraBoldFont = nullptr;

		//Time for fixedUpdate loop
		std::chrono::steady_clock::time_point m_OldTime = std::chrono::high_resolution_clock::now();;
		// Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		Ref<ContentBrowserPanel> m_ContentBrowserPanel = nullptr;
		Scope<AddResourcePanel> m_AddResourcePanel = nullptr;
		Scope<WelcomingPanel> m_WelcomePanel = nullptr;
		Scope<NameNewProjectPanel> m_NameNewProjectPanel = nullptr;
		Ref<ProjectDirectoryPanel> m_ProjectDirectoryPanel = nullptr;
		Scope<ConsolePanel> m_ConsolePanel = nullptr;
		Scope<AssistantPanel> m_AssistantPanel = nullptr;

		// Popups
		bool IsWindowTryingToClose = false; // Helper for unsaved changes
	};

}