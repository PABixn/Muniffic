#include "egpch.h"
#include "ImguiLayer.h"
#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
#include "Platform/OpenGL/imgui_impl_opengl3.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Engine/Core/Application.h"
#include "Engine/Core/Log.h"
#include "Engine/Core/Timestep.h"
#include "ImGuizmo.h"
#include <imgui_internal.h>

namespace eg {

    ImGuiLayer::ImGuiLayer()
		:Layer("ImguiLayer"), m_Time(0.0f)
	{
	}

    ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
		EG_PROFILE_FUNCTION();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImFontConfig font_config;
		font_config.OversampleH = 2;
		font_config.OversampleV = 1;

		static const ImWchar full_ranges[] = { 0x0020, 0xFFFF, 0 };

		io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", 18.0f, &font_config, full_ranges);

		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", 18.0f, &font_config, full_ranges);

		//io.Fonts->Build();


		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void ImGuiLayer::OnDetach()
	{
		EG_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnUpdate(Timestep ts)
	{
	}

	void ImGuiLayer::OnImGuiRender()
	{
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (m_BlockEvents) {
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		EG_PROFILE_FUNCTION();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		EG_PROFILE_FUNCTION();
		Application& app = Application::Get();
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		ImGui::Render();
		glViewport(0, 0, app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		/*m_LightTextShade = ImVec4(0.7765f, 0.7333f, 0.8863f, 1.0f);
		m_LightShade = ImVec4(0.251f, 0.212f, 0.349f, 1.0f);
		m_NormalShade = ImVec4(0.192f, 0.157f, 0.267f, 1.0f);
		m_DarkShade = ImVec4(0.125f, 0.102f, 0.188f, 1.0f);
		*/

		auto& colors = ImGui::GetStyle().Colors;
		auto& style = ImGui::GetStyle();

		//Background
		colors[ImGuiCol_WindowBg] = ImVec4(0.125f, 0.102f, 0.188f, 1.0f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.125f, 0.102f, 0.188f, 1.0f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.125f, 0.102f, 0.188f, 1.0f);

		// Headers
		colors[ImGuiCol_Header] = ImVec4(0.192f, 0.157f, 0.267f, 1.0f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.192f, 0.157f, 0.267f, 1.0f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.192f, 0.157f, 0.267f, 1.0f);

		// Buttons
		colors[ImGuiCol_Button] = ImVec4(0.192f, 0.157f, 0.267f, 1.0f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.251f, 0.212f, 0.349f, 1.0f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.251f, 0.212f, 0.349f, 1.0f);

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4(0.192f, 0.157f, 0.267f, 1.0f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.192f, 0.157f, 0.267f, 1.0f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.192f, 0.157f, 0.267f, 1.0f);

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4(0.251f, 0.212f, 0.349f, 1.0f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.125f, 0.102f, 0.188f, 1.0f);
		colors[ImGuiCol_TabActive] = ImVec4(0.125f, 0.102f, 0.188f, 1.0f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.251f, 0.212f, 0.349f, 1.0f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.125f, 0.102f, 0.188f, 1.0f);

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4(0.192f, 0.157f, 0.267f, 1.0f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.192f, 0.157f, 0.267f, 1.0f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.192f, 0.157f, 0.267f, 1.0f);

		//Scrollbar
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.125f, 0.102f, 0.188f, 1.0f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.251f, 0.212f, 0.349f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.251f, 0.212f, 0.349f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.251f, 0.212f, 0.349f, 1.0f);

		//Others
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.125f, 0.102f, 0.188f, 1.0f);
	}
	uint32_t ImGuiLayer::GetActiveWidgetID() const
	{
		return GImGui->ActiveId;
	}
    
}