#include "egpch.h"
#include "ImguiLayer.h"
#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"
//#include "Platform/OpenGL/imgui_impl_opengl3.h"
#include "Imgui/backends/imgui_impl_vulkan.h"
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
		VRenderer::ImGuiInit();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImFontConfig font_config;
		font_config.OversampleH = 2;      
		font_config.OversampleV = 1;      
		font_config.MergeMode = false;    
		font_config.PixelSnapH = true;    

		static const ImWchar full_ranges[] = { 0x0020, 0xFFFF, 0 };

		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-Regular.ttf", 18.0f, &font_config, full_ranges);
		io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-Light.ttf", 25.0f, &font_config, full_ranges);
		io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-Medium.ttf", 50.0f, &font_config, full_ranges);

		io.Fonts->Build(); 

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

    }

    void ImGuiLayer::OnDetach()
    {
		EG_PROFILE_FUNCTION();
		ImGui_ImplVulkan_Shutdown();
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
		EG_PROFILE_FUNCTION();
    	if (m_BlockEvents) {
    		ImGuiIO& io = ImGui::GetIO();
    		e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
    		e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
    	}
    }

    void ImGuiLayer::Begin()
    {
		EG_PROFILE_FUNCTION();
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
    }

    uint32_t ImGuiLayer::GetActiveWidgetID() const
    {
		EG_PROFILE_FUNCTION();
    	return GImGui->ActiveId;
    }

	void ImGuiLayer::End()
	{
		EG_PROFILE_FUNCTION();
		Application& app = Application::Get();
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
		//ImGui::Render();
		//glViewport(0, 0, app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
		ImGui::Render();
		int display_w, display_h;
		//glfwGetFramebufferSize(window, &display_w, &display_h);
		//glViewport(0, 0, display_w, display_h);
		VRenderer::Render();

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
		auto& colors = ImGui::GetStyle().Colors;
		auto& style = ImGui::GetStyle();

		//Background
		colors[ImGuiCol_WindowBg] = m_DarkShade;
		colors[ImGuiCol_ChildBg] = m_DarkShade;
		colors[ImGuiCol_PopupBg] = m_DarkShade;

		// Headers
		colors[ImGuiCol_Header] = m_NormalShade;
		colors[ImGuiCol_HeaderHovered] = m_NormalShade;
		colors[ImGuiCol_HeaderActive] = m_NormalShade;

		// Buttons
		colors[ImGuiCol_Button] = m_NormalShade;
		colors[ImGuiCol_ButtonHovered] = m_LightShade;
		colors[ImGuiCol_ButtonActive] = m_LightShade;

		// Frame BG
		colors[ImGuiCol_FrameBg] = m_NormalShade;
		colors[ImGuiCol_FrameBgHovered] = m_NormalShade;
		colors[ImGuiCol_FrameBgActive] = m_NormalShade;

		// Tabs
		colors[ImGuiCol_Tab] = m_LightShade;
		colors[ImGuiCol_TabHovered] = m_DarkShade;
		colors[ImGuiCol_TabActive] = m_DarkShade;
		colors[ImGuiCol_TabUnfocused] = m_LightShade;
		colors[ImGuiCol_TabUnfocusedActive] = m_DarkShade;

		// Title
		colors[ImGuiCol_TitleBg] = m_NormalShade;
		colors[ImGuiCol_TitleBgActive] = m_NormalShade;
		colors[ImGuiCol_TitleBgCollapsed] = m_NormalShade;

		//Scrollbar
		colors[ImGuiCol_ScrollbarBg] = m_DarkShade;
		colors[ImGuiCol_ScrollbarGrab] = m_LightShade;
		colors[ImGuiCol_ScrollbarGrabHovered] = m_LightShade;
		colors[ImGuiCol_ScrollbarGrabActive] = m_LightShade;

		//Others
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.153f, 0.133f, 0.200f, 1.0f);
	}
    
}
