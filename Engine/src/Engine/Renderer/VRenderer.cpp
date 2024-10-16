#include "VRenderer.h"
#include "Platform/Vulkan/VulkanRenderer.h"
#include "Engine/Core/Application.h"
void eg::VRenderer::Init()
{
	VRen::get().init();
}

void eg::VRenderer::Render()
{
	VRen::get().render();
}

void eg::VRenderer::Shutdown()
{
	VRen::get().cleanUp();
}

void eg::VRenderer::ImGuiInit()
{
#if WIN32
	GLFWwindow* win = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
#else
	EG_CORE_ASSERT(false);// FOR NOW THE APP IS ONLY FOR WIDNOWS USING GLFW
#endif // WIN32
	VRen::get().initImGui(win);
}
