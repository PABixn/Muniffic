#include "egpch.h"
#include "EditorLayer.h"
#include "Imgui/imgui.h"
#include "Engine/Core/Core.h"
#include "Engine/Scene/SceneSerializer.h"
#include "Engine/Utils/PlatformUtils.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Scene/Components.h"
#include "Engine/Math/Math.h"
#include "Commands/Commands.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Renderer/Font.h"
#include <optional>
#include "ImGuizmo.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Engine/Assistant/AssistantManager.h>
#include "Engine/Project/ProjectSerializer.h"
#include "Engine/Project/RecentProjectSerializer.h"
#include "Engine/Project/ScriptSerializer.h"
#include "IconLoader.h"
#ifdef EG_RELEASE
#define HELLO "Hello from Release"
#endif
namespace eg
{

	const std::string customScriptsCmakeLists = "CustomScriptsCmakeLists.txt";

	static UUID s_Font;
	bool projectOpened = false;
	RecentProjectSerializer m_RecentProjectSerializer;
	EditorLayer::EditorLayer()
		: Layer("Sandbox2D"), m_Camera(1280.0f / 720.0f, true)
	{
		EG_PROFILE_FUNCTION();
           
		m_RecentProjectSerializer = RecentProjectSerializer();
		m_WelcomePanel = CreateScope<WelcomingPanel>(m_RecentProjectSerializer.getProjectList(), m_RecentProjectSerializer);
		m_NameNewProjectPanel = CreateScope<NameNewProjectPanel>();
		m_NameNewProjectPanel->ShowWindow(s_Font);
	}

	constexpr float AxisLength = 100000000.0f;

	void EditorLayer::OnAttach()
	{
		LoadFonts();
		ResourceSystemConfig resourceSystemConfig;
		resourceSystemConfig.MaxLoaderCount = 4;
		resourceSystemConfig.ResourceDirectory = "../resources";

		if (!resourceSystemInit(resourceSystemConfig))
		{
			EG_ERROR("Failed to initialize resource system.");
			return;
		}

		IconLoader::LoadIcons();

		m_WelcomePanel->InitWelcomingPanel();


		m_IconPlay = IconLoader::GetIcon(Icons::Editor_PlayButton);
		m_IconPause = IconLoader::GetIcon(Icons::Editor_PauseButton);
		m_IconSimulate = IconLoader::GetIcon(Icons::Editor_SimulateButton);
		m_IconStop = IconLoader::GetIcon(Icons::Editor_StopButton);
		m_IconStep = IconLoader::GetIcon(Icons::Editor_StepButton);

		FrameBufferSpecification fbSpec;
		fbSpec.Attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_FrameBuffer = FrameBuffer::Create(fbSpec);

		m_ActiveScene = CreateRef<Scene>();
		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
		Renderer2D::SetLineThickness(3.0f);
		s_Font = Font::GetDefaultFontUUID();
	}

	void EditorLayer::OnDetach()
	{
		EG_PROFILE_FUNCTION();
		resourceSystemShutdown();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		EG_PROFILE_FUNCTION();
		if (m_WelcomePanel->IsShown()) {
			return;
		}
		
		if (!m_ActiveScene->GetRegistry().valid(m_HoveredEntity))
			m_HoveredEntity = Entity({});
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		if (FrameBufferSpecification spec = m_FrameBuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_Camera.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
		}

		if (m_ViewportFocused)
			m_Camera.OnUpdate(ts);


		m_EditorCamera.OnUpdate(ts);
		m_SceneHierarchyPanel.Update(ts);
		if(m_ContentBrowserPanel)
			m_ContentBrowserPanel->Update(ts);

		Renderer2D::ResetStats();
		m_FrameBuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		m_FrameBuffer->ClearAttachment(1, -1);

		switch (m_SceneState)
		{
		case SceneState::Edit:
		{
			// Update
			if (m_ViewportFocused)
				m_Camera.OnUpdate(ts);
			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
			if (projectOpened) {
				m_AddResourcePanel->Update(ts);
			}


			// Render
			// Renderer2D::BeginScene(m_EditorCamera);
			// m_ActiveScene->OnRenderEditor(m_EditorCamera);
			// Renderer2D::EndScene();
			break;
		}
		case SceneState::Play:
		{
			// Update

			m_RuntimeScene->OnUpdateRuntime(ts, m_OldTime);

			// Render
			// Renderer2D::BeginScene(m_EditorCamera);
			// m_ActiveScene->OnRenderRuntime(m_EditorCamera);
			// Renderer2D::EndScene();
			break;
		}
		case SceneState::Simulate:
		{
			m_EditorCamera.OnUpdate(ts);

			m_RuntimeScene->OnUpdateSimulation(ts, m_EditorCamera);
			break;
		}
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		my = m_ViewportSize.y - my;

		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];

		int mouseX = (int)mx;
		int mouseY = (int)my;
		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixedData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixedData == -1 ? Entity() : Entity((entt::entity)pixedData, m_ActiveScene.get());
		}

		OnOverlayRender();
		m_FrameBuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
        EG_PROFILE_FUNCTION();
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		/*if (m_WelcomePanel->IsShown()) {
			dockspace_flags |= ImGuiDockNodeFlags_AutoHideTabBar;
			dockspace_flags |= ImGuiDockNodeFlags_NoSplit;
		}*/

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1529f, 0.1333f, 0.2000f, 1.0f));

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		ImGui::PopStyleColor();
		//Welcoming panel code
		if (m_WelcomePanel->IsShown()) {
			ImGuiStyle& style = ImGui::GetStyle();
			float minWinSizeX = style.WindowMinSize.x;
			float minWinSizeY = style.WindowMinSize.y;
			style.WindowMinSize.x = 1500.0f;
			style.WindowMinSize.y = 1500.0f;
			style.WindowMinSize.x = minWinSizeX;
			style.WindowMinSize.y = minWinSizeY;

			m_WelcomePanel->OnImGuiRender();
			ImGui::End();
		}
		else if (m_NameNewProjectPanel->isNameGiven() && m_WelcomePanel->isNewProjectCreated() && m_NameNewProjectPanel->IsShown()) {
			//m_NameNewProjectPanel->ShowWindow(s_Font)
			m_NameNewProjectPanel->OnImGuiRender();
			ImGui::End();

		}
		else {
			if (!projectOpened) {
				if (m_WelcomePanel->isNewProjectCreated()) {
					//nya 2
					NewProject();
					m_NameNewProjectPanel->setNameGiven(true);
				}
				else
				{
					if (m_WelcomePanel->getSelectedProject() != "")
					{
						auto& projectFilePath = m_WelcomePanel->getSelectedProject();
						OpenProject(projectFilePath);
					}
					else
					{
						auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
						if (commandLineArgs.Count > 1)
						{
							auto projectFilePath = commandLineArgs[1];
							OpenProject(projectFilePath);
						}
						else
							if (!OpenProject())
								Application::Get().Close();
							else m_AddResourcePanel = CreateScope<AddResourcePanel>();
					}
				}
				//m_AddResourcePanel = CreateScope<AddResourcePanel>();

				projectOpened = true;
		     	}

			style.WindowMinSize.x = minWinSizeX;

			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0, 0, 0, 0);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0, 0, 0, 0);
			style.Colors[ImGuiCol_TextDisabled] = m_LightTextShade;

		ImGui::PushFont(m_PoppinsSemiBoldFont);
		ImGui::PushStyleColor(ImGuiCol_Text, m_LightTextShade);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
					OpenProject();

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
					Save();

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
					SaveAs();

				if (ImGui::MenuItem("Exit"))
					Application::Get().Close();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Script"))
			{
				if (ImGui::MenuItem("Reload assembly", "Ctrl+R"))
					ScriptEngine::ReloadAssembly();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Resources"))
			{
				if (ImGui::MenuItem("Add Resource", "Ctrl+A"))
					m_AddResourcePanel->showResourcePanel(true);

				ImGui::EndMenu();
			}

			if(ImGui::MenuItem("Game Objects")){}
			if(ImGui::MenuItem("Components")){}
			if(ImGui::MenuItem("Window")){}
			if(ImGui::MenuItem("Help")){}

			ImGui::EndMenuBar();
		}
		ImGui::PopStyleColor();
		ImGui::PopFont();

			m_SceneHierarchyPanel.OnImGuiRender();
			m_ContentBrowserPanel->OnImGuiRender();
			m_ProjectDirectoryPanel->OnImGuiRender();
			m_ConsolePanel->OnImGuiRender();
			m_AssistantPanel->OnImGuiRender();

			if (IsWindowTryingToClose) {
				if (!IsProjectSaved()) {
					ImGui::OpenPopup("Unsaved Changes");
					ImGui::SetNextWindowPos(ImVec2(Application::Get().GetWindow().GetWidth() / 2, Application::Get().GetWindow().GetHeight() / 2));

					ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.f);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.f, 10.f));

					if (ImGui::BeginPopupModal("Unsaved Changes", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize)) {

						ImVec2 spacing = style.ItemSpacing;
						style.ItemSpacing = ImVec2(10.f, 10.f);
						ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.f, 5.f));

						ImGui::Text("Warning: Unsaved changes");
						bool SaveBttn = ImGui::Button("Save");
						ImGui::SameLine();
						bool NotSaveBttn = ImGui::Button("Don't save");
						ImGui::SameLine();
						bool CancelBttn = ImGui::Button("Cancel");
						if (SaveBttn) {
							(*(dynamic_cast<EditorLayer*>(Application::Get().GetFirstLayer()))).Save();
							Application::Get().Close();
						}
						else if (NotSaveBttn) {
							Application::Get().Close();
						}
						else if (CancelBttn) {
							SetIsWindowTryingToClose(false);
						}

						style.ItemSpacing = spacing;
						ImGui::PopStyleVar(2);
						ImGui::EndPopup();
					}

					ImGui::PopStyleVar(2);
				};
			}

			ImGui::Begin("Stats");
			std::string name = "None";
			if (m_HoveredEntity.GetScene() && m_HoveredEntity.Exists()) { 
				name = m_HoveredEntity.GetComponent<TagComponent>().Tag;
				ImGui::Text("Hovered Entity: %s", name.c_str());
			}

			auto stats = Renderer2D::GetStats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

			ImGui::End();

			ImGui::Begin("Settings");
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
			if (ImGui::Checkbox("Show Physics Colliders", &m_ShowPhysicsColliders))
				Commands::ExecuteRawValueCommand(&m_ShowPhysicsColliders, !m_ShowPhysicsColliders, "Show Physics Colliders");
			if (ImGui::Checkbox("Show Axis", &m_ShowAxis))
				Commands::ExecuteRawValueCommand(&m_ShowAxis, !m_ShowAxis, "Show Axis");
			if (ImGui::Checkbox("Show Grid", &m_ShowGrid))
				Commands::ExecuteRawValueCommand(&m_ShowGrid, !m_ShowGrid, "Show Grid");
			ImGui::PopStyleVar();
			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin("Viewport");
			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			auto viewportOffset = ImGui::GetWindowPos();
			m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
			m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused);

			ImVec2 viewportSize = ImGui::GetContentRegionAvail();

			m_ViewportSize = { viewportSize.x, viewportSize.y };
			uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID(0);
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserPanel"))
				{
					const wchar_t* path = (const wchar_t*)payload->Data;
					std::filesystem::path p = path;
					if (p.extension() == ".egscene")
   						OpenScene((Project::GetProjectName()) / Project::GetAssetDirectory() / path);
				}
				ImGui::EndDragDropTarget();
			}

			// Gizmos
			Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
			if (selectedEntity && m_GizmoType != -1)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, m_ViewportSize.x, m_ViewportSize.y);

				ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);
				// Camera
				// Runtime camera from entity
				// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
				// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
				// const glm::mat4& cameraProjection = camera.GetProjection();
				// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

				// Editor camera
				const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
				glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

				// Entity Transform
				auto& tc = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = tc.GetTransform();

				// Snapping
				bool snap = Input::IsKeyPressed(KeyCode::LeftControl);
				float snapValue = 0.5f; // Snap to 0.5m for translation/scale
				// Snap to 45 degrees for rotation
				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				float snapValues[3] = { snapValue, snapValue, snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
					nullptr, snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing())
				{
					glm::vec3 translation, rotation, scale;
					Math::DecomposeTransform(transform, translation, rotation, scale);

					glm::vec3 deltaRotation = rotation - tc.Rotation;
					tc.Translation = translation;
					tc.Rotation = rotation;
					tc.Scale = scale;
				}

				// ImGuizmo::ViewManipulate(glm::value_ptr(cameraView), m_Camera.GetZoomLevel(), ImVec2{ ImGui::GetWindowPos().x, ImGui::GetWindowPos().y }, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, 0x10101010);
			}

			ImGui::End();
			ImGui::PopStyleVar();

			UI_Toolbar();

			ImGui::End();
			if (projectOpened)
				m_AddResourcePanel->OnImGuiRender();
		}
	}

	void EditorLayer::UI_Toolbar()
	{
		EG_PROFILE_FUNCTION();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.0f, 2.0f });
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2{ 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.0f, 0.0f, 0.0f });
		ImGui::PushStyleColor(ImGuiCol_Border, m_DarkShade);
		auto& colors = ImGui::GetStyle().Colors;
		auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f });
		auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ buttonActive.x, buttonActive.y, buttonActive.z, 0.5f });

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		bool toolbarEnabled = (bool)m_ActiveScene;

		ImVec4 tintColor = ImVec4(1, 1, 1, 1);
		if (!toolbarEnabled)
			tintColor.w = 0.5f;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 0));
		float size = ImGui::GetWindowHeight() - 4.0f;
		ImVec2 elementSize = ImGui::GetContentRegionAvail();
		float iconPosX = (elementSize.x - size) * 0.5f - 19.0f;
		float iconPosY = (elementSize.y - size) * 0.5f + 2.0f;
		ImGui::SetCursorPos(ImVec2(iconPosX, iconPosY));

		bool hasPlayButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play;
		bool hasSimulateButton = m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate;
		bool hasPauseButton = m_SceneState != SceneState::Edit;

		if (hasPlayButton)
		{
			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate) ? m_IconPlay : m_IconStop;
			if (ImGui::ImageButton("EditOrSimulate##xx", (ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulate)
					OnScenePlay();
				else if (m_SceneState == SceneState::Play)
					OnSceneStop();
			}
		}

		if (ImGui::BeginPopupModal("No Camera", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration))
		{
			ImVec2 window_size = ImGui::GetWindowSize();
			ImVec2 screen_center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetWindowPos(ImVec2(screen_center.x - window_size.x * 0.5f, screen_center.y - window_size.y * 0.5f));

			ImGui::Text("No entity with Camera Component was found in current scene. \n At least one must be present.");

			float window_width = ImGui::GetWindowSize().x;
			float button_width = 120.0f;
			ImGui::SetCursorPosX((window_width - button_width) * 0.5f);

			if (ImGui::Button("OK", ImVec2(button_width, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}


		if (hasSimulateButton)
		{
			if (hasPlayButton)
				ImGui::SameLine();

			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? m_IconSimulate : m_IconStop;
			if (ImGui::ImageButton("EditOrPlay##xx", (ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play)
					OnSceneSimulate();
				else if (m_SceneState == SceneState::Simulate)
					OnSceneStop();
			}
		}
		if (hasPauseButton)
		{
			bool isPaused = m_ActiveScene->IsPaused();
			ImGui::SameLine();
			{
				Ref<Texture2D> icon = m_IconPause;
				if (ImGui::ImageButton("Pause##xx", (ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
				{
					m_ActiveScene->SetPaused(!isPaused);
				}
			}

			// Step button
			if (isPaused)
			{
				ImGui::SameLine();
				{
					Ref<Texture2D> icon = m_IconStep;
					bool isPaused = m_ActiveScene->IsPaused();
					if (ImGui::ImageButton("Step##xx", (ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
					{
						m_ActiveScene->Step();
					}
				}
			}
		}
		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor(4);
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		EG_PROFILE_FUNCTION();
		m_Camera.OnEvent(e);
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		EG_PROFILE_FUNCTION();
		if (e.IsRepeat())
			return false;

		bool control = Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl);
		bool shift = Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift);

		bool controlPressed = Input::IsKeyPressed(KeyCode::LeftControl) || Input::IsKeyPressed(KeyCode::RightControl);
		bool shiftPressed = Input::IsKeyPressed(KeyCode::LeftShift) || Input::IsKeyPressed(KeyCode::RightShift);

		switch (e.GetKeyCode())
		{
		case Key::Z:
		{
			if (controlPressed)
			{
				if (Commands::CanRevert(true))
					Commands::GetCurrentCommand()->Undo();
			}

			break;
		}
		case Key::Y:
		{
			if (controlPressed)
			{
				if (Commands::CanRevert(false))
					Commands::GetCurrentCommand(1)->Redo();
			}

			break;
		}

		case Key::S:
		{
			if (controlPressed)
				if (shiftPressed)
					SaveAs();
				else
					Save();
			break;
		}
		case Key::A:
		{
			if (controlPressed)
				m_AddResourcePanel->showResourcePanel(true);
			break;
		}
		case Key::N:
		{
			if (controlPressed)
				NewScene();
			break;
		}
		case Key::O:
		{
			if (controlPressed)
				OpenProject();
			break;
		}
		// Commands
		case Key::D:
		{
			if (controlPressed)
				OnDuplicateEntity();
			break;
		}
		// Gizmos
		case Key::Q:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = -1;
			break;
		}
		case Key::W:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case Key::E:
		{
			if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case Key::R:
		{
			if (control)
				ScriptEngine::ReloadAssembly();
			else if (!ImGuizmo::IsUsing())
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
		case Key::Delete:
		{
			if (Application::Get().GetImGuiLayer()->GetActiveWidgetID() == 0)
			{
				Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
				if (selectedEntity)
				{
					m_SceneHierarchyPanel.SetSelectedEntity({});
					Commands::ExecuteCommand<Commands::DeleteEntityCommand>(Commands::CommandArgs("", selectedEntity, m_ActiveScene, selectedEntity));
				}
			}
			break;
		}
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		EG_PROFILE_FUNCTION();
		if (e.GetMouseButton() == (int)Mouse::ButtonLeft)
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(KeyCode::LeftAlt))
				m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}
		return false;
	}

	void EditorLayer::OnOverlayRender()
	{
		EG_PROFILE_FUNCTION();
		if (m_SceneState == SceneState::Play)
		{
			Entity cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
 			const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			Renderer2D::BeginScene(camera, cameraEntity.GetComponent<TransformComponent>().GetTransform());
		}
		else
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

		if (m_ShowAxis)
		{
			glm::vec3 start = { 0,0,0 };
			Renderer2D::DrawLine(start, { AxisLength, 0,0 }, { 1.0f, 0.0f, 0.0f, 1.0f });
			Renderer2D::DrawLine(start, { 0, AxisLength,0 }, { 0.0f, 1.0f, 0.0f, 1.0f });
			Renderer2D::DrawLine(start, { 0, 0, AxisLength }, { 1.0f, 0.0f, 1.0f, 1.0f });
		}
		if (m_ShowGrid)
		{
			int linesAmount = 10000;
			for (int i = -linesAmount; i <= linesAmount; i++)
			{
				if (i == 0)
					continue;
				Renderer2D::DrawLine({ -linesAmount ,i , 0 }, { linesAmount , i, 0 }, { 0.2f, 0.2f, 0.2f, 1.0f });
				Renderer2D::DrawLine({ i ,-linesAmount , 0 }, { i , linesAmount, 0 }, { 0.2f, 0.2f, 0.2f, 1.0f });
				Renderer2D::DrawLine({ -linesAmount ,0 , i }, { linesAmount , 0, i }, { 0.2f, 0.2f, 0.2f, 1.0f });
				Renderer2D::DrawLine({ 0 ,-linesAmount , i }, { 0 , linesAmount, i }, { 0.2f, 0.2f, 0.2f, 1.0f });
				Renderer2D::DrawLine({ i ,0 , -linesAmount }, { i , 0, linesAmount }, { 0.2f, 0.2f, 0.2f, 1.0f });
				Renderer2D::DrawLine({ 0 ,i , -linesAmount }, { 0 , i, linesAmount }, { 0.2f, 0.2f, 0.2f, 1.0f });
			}
		}

		if (m_ShowPhysicsColliders)
		{

			// Circle Colliders
			{
				auto view = m_ActiveScene->GetEntitiesWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

					glm::vec3 translation = tc.Translation + glm::vec3(cc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation) * glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawCircle(transform, { 0, 1, 0, 1 }, 0.01f);
				}
			}

			// Box Colliders
			{
				auto view = m_ActiveScene->GetEntitiesWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

					glm::vec3 translation = tc.Translation + glm::vec3(bc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Translation) * glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0, 0, 1.0f)) * glm::translate(glm::mat4(1.0f), glm::vec3(bc2d.Offset, 0.001f)) * glm::scale(glm::mat4(1.0f), scale);
					Renderer2D::DrawRect(transform, { 0, 1, 0, 1 });
				}
			}
		}

		// Draw selected entity outline
		if (Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity())
		{
			if (selectedEntity.HasComponent<TransformComponent>())
			{
				const TransformComponent& transform = selectedEntity.GetComponent<TransformComponent>();
				Renderer2D::DrawRect(transform.GetTransform(), glm::vec4(1, 0.5f, 0, 1));
			}
		}

		Renderer2D::EndScene();
	}

	void EditorLayer::NewScene()
	{
		EG_PROFILE_FUNCTION();
		m_ActiveScene = CreateRef<Scene>();
		m_RuntimeScene = m_ActiveScene;

		if (m_SceneState != SceneState::Edit)
			OnSceneStop();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_ActiveScenePath = std::filesystem::path();
		ConsolePanel::Log("File: EditorLayer.cpp - New Scene Created", ConsolePanel::LogType::Info);
	}

	void EditorLayer::OpenScene()
	{
		EG_PROFILE_FUNCTION();
		std::string filepath = FileDialogs::OpenFile("Muniffic Scene (*.egscene)\0*.egscene\0");
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
		else {
			ConsolePanel::Log("File: EditorLayer.cpp - Empty file path", ConsolePanel::LogType::Error);
			return;
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		EG_PROFILE_FUNCTION();
		if (m_SceneState != SceneState::Edit)
			OnSceneStop();

		if (path.extension().string() != ".egscene")
		{
			ConsolePanel::Log("File: EditorLayer.cpp - Could not load " + path.filename().string() + " - not a scene file", ConsolePanel::LogType::Error);
			return;
		}

		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.Deserialize(path.string()))
		{
			m_RuntimeScene = CreateRef<Scene>();
			m_EditorScene = newScene;
			m_ActiveScene = m_EditorScene;
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
			m_ActiveScenePath = path;
		}
	}

	void EditorLayer::SaveAs()
	{
		EG_PROFILE_FUNCTION();
		std::string filepath = FileDialogs::SaveFile("Muniffic Scene (*.egscene)\0*.egscene\0");
		if (!filepath.empty())
		{
			SerializeScene(m_ActiveScene, filepath);

			m_ActiveScenePath = filepath;
		}
		else {
			ConsolePanel::Log("File: EditorLayer.cpp - Empty file path", ConsolePanel::LogType::Error);
			return;
		}
		ConsolePanel::Log("File: EditorLayer.cpp - Scene saved", ConsolePanel::LogType::Info);
		SetIsSaved(true);
	}

	void EditorLayer::Save()
	{
		EG_PROFILE_FUNCTION();
		if (m_ActiveScenePath.empty())
			SaveAs();
		else
		{
			SerializeScene(m_ActiveScene, m_ActiveScenePath);
		}
		ConsolePanel::Log("File: EditorLayer.cpp - Scene saved", ConsolePanel::LogType::Info);
		SetIsSaved(true);
	}


	bool EditorLayer::CreateCmakelists(const std::filesystem::path path)
	{
		EG_PROFILE_FUNCTION();
		std::filesystem::path pathToScripts(path.string() + "\\Assets\\Scripts");
		m_CustomScriptsDirectory = pathToScripts;
		std::filesystem::path cmakeFilePath = pathToScripts / "CMakeLists.txt";

		if(std::filesystem::exists(cmakeFilePath) && std::filesystem::is_regular_file(cmakeFilePath)) return true;

		std::filesystem::path cscmakelistsPath = customScriptsCmakeLists;
		std::filesystem::path abs = std::filesystem::absolute(cscmakelistsPath);
		std::ifstream source(cscmakelistsPath, std::ios::binary);

		std::ofstream destination(cmakeFilePath, std::ios::binary);

		if (!source.is_open() || !destination.is_open()) {
			return false;
		}

		destination << source.rdbuf();

		destination.flush();
		destination.close();
		source.close();
		return true;
	}

	const std::string EditorLayer::CompileCustomScripts()
	{
		EG_PROFILE_FUNCTION();
        if (!m_CurrentProject || !Project::GetActive())
			return "No active project";
		std::filesystem::path projectDirectory =  m_CurrentProject->GetProjectDirectory() / "Assets" / "Scripts";
		const std::string& projectName = m_CurrentProject->GetProjectName();
		std::string command = "cd " + projectDirectory.string() + " && cmake -DPROJECT_NAME=" + projectName + " . && cmake --build .";
		if(system(command.c_str())!=0) return "Compilation failed. Look into console for more information.";
		auto scriptModulePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / Project::GetActive()->GetScriptModulePath();
		ScriptEngine::ReloadAssembly();
		EG_TRACE("Compilation of Custom Scripts succeded");
		return "Compilation succeded.";
	}

	void EditorLayer::NewProject()
	{
		EG_PROFILE_FUNCTION();
		std::filesystem::path saveDir = std::filesystem::current_path() / "Projects" / m_NameNewProjectPanel->projectName / (m_NameNewProjectPanel->projectName + ".mnproj");
		std::filesystem::path absolutePath = std::filesystem::absolute(saveDir);
		//std::filesystem::path savePath = absolutePath / "Projects";

		std::string scriptsFolder = m_NameNewProjectPanel->projectName + "\\Assets\\Scripts";
		std::string scenesFolder = m_NameNewProjectPanel->projectName + "\\Assets\\Scenes";
		
		m_RecentProjectSerializer.Serialize(absolutePath.string(), "recentProjectSerializer.txt");

		Project::New();

		Project::SetProjectName(m_NameNewProjectPanel->projectName);
		Application::Get().ChangeNameWithCurrentProject(true);
		Project::SetScenesDirectory("Scenes");
		Project::SetProjectDirectory(absolutePath.parent_path());

		std::filesystem::path dir = Project::GetProjectDirectory();

		UUID rootUUID = UUID();
		AssetDirectory* root = new AssetDirectory(rootUUID, "Assets", 0);

		AssetDirectoryManager::initDefault(rootUUID);

		bool createdCmakeLists = CreateCmakelists(absolutePath.parent_path().string())

		EG_ASSERT(createdCmakeLists);
		
		Project::Save(absolutePath);
		NewScene();
		m_ActiveScenePath = Project::GetSceneFileSystemPath(Project::GetStartScene());
		Save();
		OpenScene(m_ActiveScenePath);

		OpenProject(absolutePath);
		/*ScriptEngine::Init();
		m_ContentBrowserPanel = CreateScope<ContentBrowserPanel>();
		m_ProjectDirectoryPanel = CreateRef<ProjectDirectoryPanel>();
		m_ProjectDirectoryPanel->SetContentBrowserPanel(m_ContentBrowserPanel);
		m_ContentBrowserPanel->InitPanels();
		m_AddResourcePanel = CreateScope<AddResourcePanel>();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);*/


		
		ScriptSerializer::Serialize(scriptsFolder, m_NameNewProjectPanel->projectName);
		ConsolePanel::Log("File: EditorLayer.cpp - New project created", ConsolePanel::LogType::Info);
	}

	bool EditorLayer::OpenProject()
	{
		EG_PROFILE_FUNCTION();
		std::string filepath = FileDialogs::OpenFile("Muniffic Project (*.mnproj)\0*.mnproj\0");
		std::filesystem::path path = filepath;
        if (filepath.empty() )
		{
			ConsolePanel::Log("File: EditorLayer.cpp - Empty file path", ConsolePanel::LogType::Error);
			return false;
		}
		if (path.extension() != ".mnproj"){
            ConsolePanel::Log("File: EditorLayer.cpp - Could not load " + path.filename().string() + " - not a project file", ConsolePanel::LogType::Error);
            return false;
		}
		if (Project::GetActive() && path == Project::GetProjectPath()) {
			ConsolePanel::Log("File: EditorLayer.cpp - Project already opened", ConsolePanel::LogType::Warning);
            return false;
	    }
  
		OpenProject(filepath);
		m_RecentProjectSerializer.Serialize(filepath, "recentProjectSerializer.txt");
		ConsolePanel::Log("File: EditorLayer.cpp - Project opened", ConsolePanel::LogType::Info);
		return true;
	}

	void EditorLayer::OpenProject(const std::filesystem::path& path)
	{
        EG_PROFILE_FUNCTION();
        bool isLoaded = Project::IsLoaded();
		//if (isLoaded)
            //ScriptEngine::Shutdown();
		if (m_CurrentProject = Project::Load(path))
		{
			ScriptEngine::Init();
			auto startScenePath = Project::GetSceneFileSystemPath(Project::GetStartScene());
			OpenScene(startScenePath);
			m_ContentBrowserPanel = CreateScope<ContentBrowserPanel>();
			m_ProjectDirectoryPanel = CreateRef<ProjectDirectoryPanel>();
			m_ProjectDirectoryPanel->SetContentBrowserPanel(m_ContentBrowserPanel);
			m_AddResourcePanel = CreateScope<AddResourcePanel>();
			m_AssistantPanel = CreateScope<AssistantPanel>();
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);
			m_ContentBrowserPanel->InitPanels();
			Application::Get().ChangeNameWithCurrentProject(true);


			ConsolePanel::Log("File: EditorLayer.cpp - Project opened", ConsolePanel::LogType::Info);
		}
		else
		{
			ConsolePanel::Log("File: EditorLayer.cpp - Failed to open project", ConsolePanel::LogType::Error);
		}
	}

	void EditorLayer::SaveProjectAs()
	{
        EG_PROFILE_FUNCTION();
		ConsolePanel::Log("File: EditorLayer.cpp - Project saved", ConsolePanel::LogType::Info);
	}

	void EditorLayer::SaveProject()
	{
        EG_PROFILE_FUNCTION();
		ConsolePanel::Log("File: EditorLayer.cpp - Project saved", ConsolePanel::LogType::Info);
	}

	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
        EG_PROFILE_FUNCTION();
		SceneSerializer serializer(scene);
		serializer.Serialize(path.string());
		ConsolePanel::Log("File: EditorLayer.cpp - Scene serialized", ConsolePanel::LogType::Info);
	}

	void EditorLayer::OnScenePlay()
	{
		EG_PROFILE_FUNCTION();
		{
			EG_PROFILE_SCOPE("Check Camera");
		    if (m_ActiveScene->GetEntitiesWith<CameraComponent>().empty())
		    {
		    	ImGui::OpenPopup("No Camera");
		    
		    	return;
		    }
		}
		if (m_SceneState == SceneState::Simulate)
			OnSceneStop();
		m_SceneState = SceneState::Play;
		{
            EG_PROFILE_SCOPE("Copying Scene");
		    m_RuntimeScene = Scene::Copy(m_ActiveScene);
		}
		{
            EG_PROFILE_SCOPE("OnRuntimeStart");
			m_OldTime = std::chrono::high_resolution_clock::now();
		    m_RuntimeScene->OnRuntimeStart();
		    m_ActiveScene = m_RuntimeScene;
		    m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		    ConsolePanel::Log("File: EditorLayer.cpp - Scene started", ConsolePanel::LogType::Info);
		}
	}

	void EditorLayer::OnSceneSimulate()
	{
		EG_PROFILE_FUNCTION();
		if (m_SceneState == SceneState::Play)
			OnSceneStop();

		m_SceneState = SceneState::Simulate;

		m_RuntimeScene = Scene::Copy(m_ActiveScene);
		m_RuntimeScene->OnSimulationStart();
		m_ActiveScene = m_RuntimeScene;
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		ConsolePanel::Log("File: EditorLayer.cpp - Simulation started", ConsolePanel::LogType::Info);
	}

	void EditorLayer::OnScenePause()
	{
		EG_PROFILE_FUNCTION();

		if (m_SceneState == SceneState::Edit)
			return;
		ConsolePanel::Log("File: EditorLayer.cpp - Scene paused", ConsolePanel::LogType::Info);
		m_ActiveScene->SetPaused(true);
	}

	void EditorLayer::OnSceneStop()
	{
        EG_PROFILE_FUNCTION();
		EG_CORE_ASSERT(m_SceneState == SceneState::Play || m_SceneState == SceneState::Simulate);
		

		if (m_SceneState == SceneState::Play)
			m_ActiveScene->OnRuntimeStop();
		else if (m_SceneState == SceneState::Simulate)
			m_ActiveScene->OnSimulationStop();

		m_SceneState = SceneState::Edit;
		m_RuntimeScene->OnRuntimeStop();
		m_ActiveScene = m_EditorScene;
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
		m_RuntimeScene = nullptr;
		ConsolePanel::Log("File: EditorLayer.cpp - Scene stopped", ConsolePanel::LogType::Info);
	}

	void EditorLayer::OnDuplicateEntity()
	{
		EG_PROFILE_FUNCTION();

		if (m_SceneState == SceneState::Play) {
			ConsolePanel::Log("File: EditorLayer.cpp - Cannot duplicate entities while playing", ConsolePanel::LogType::Warning);
			return;
		}
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
		{
			Entity newEntity = m_ActiveScene->DuplicateEntity(selectedEntity);
			m_SceneHierarchyPanel.SetSelectedEntity(newEntity);
			ConsolePanel::Log("File: EditorLayer.cpp - Entity duplicated", ConsolePanel::LogType::Info);
		}
		else {
			ConsolePanel::Log("File: EditorLayer.cpp - No entity selected for duplication", ConsolePanel::LogType::Warning);
		}
	}

	void EditorLayer::CloseAddResourcePanel()
	{
		EG_PROFILE_FUNCTION();
		m_AddResourcePanel = nullptr;
	}

	void EditorLayer::LoadFonts() {
		auto& io = ImGui::GetIO();

		ImFontConfig font_config;
		font_config.OversampleH = 2;
		font_config.OversampleV = 1;
		font_config.MergeMode = false;
		font_config.PixelSnapH = true;

		static const ImWchar full_ranges[] = { 0x0020, 0xFFFF, 0 };

		m_PoppinsLightFont = io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-Light.ttf", 25.0f, &font_config, full_ranges);
		m_PoppinsRegularFont = io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-Regular.ttf", 18.0f, &font_config, full_ranges);
		m_PoppinsRegularFontBig = io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-Regular.ttf", 20.0f, NULL, io.Fonts->GetGlyphRangesDefault());
		m_PoppinsMediumFont = io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-Medium.ttf", 50.0f, &font_config, full_ranges);
		m_PoppinsSemiBoldFont = io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-SemiBold.ttf", 20.0f, NULL, io.Fonts->GetGlyphRangesDefault());
		m_PoppinsSemiBoldFontBig = io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-SemiBold.ttf", 40.0f, NULL, io.Fonts->GetGlyphRangesDefault());
		m_PoppinsExtraBoldFont = io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-ExtraBold.ttf", 50.0f, NULL, io.Fonts->GetGlyphRangesDefault());

		io.FontDefault = m_PoppinsRegularFont;

		io.Fonts->Build();
	}

}