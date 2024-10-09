#include "egpch.h"
#include "VulkanRenderer.h"
#include "Engine/Core/Application.h"
#include "Platform/Windows/WindowsWindow.h"


void eg::VulkanRenderer::init()
{
#if WIN32
	void* win = (GLFWwindow*) Application::Get().GetWindow().GetNativeWindow();
#else
	EG_CORE_ASSERT(false);// FOR NOW THE APP IS ONLY FOR WIDNOWS USING GLFW
#endif // WIN32
	m_Device.init((GLFWwindow*)win);
	EG_TRACE("Vulkan Renderer initialization Succesfull");
}

void eg::VulkanRenderer::cleanUp()
{
	m_Device.cleanUp();
	EG_TRACE("Vulkan Renderer Destruction Succesfull");
}

