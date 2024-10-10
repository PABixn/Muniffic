#include "egpch.h"
#include "VulkanRenderer.h"
#include "Engine/Core/Application.h"
#include "Platform/Windows/WindowsWindow.h"
eg::VulkanRenderer eg::VRen::vren = eg::VulkanRenderer();

void eg::VulkanRenderer::init()
{
#if WIN32
	GLFWwindow* win = (GLFWwindow*) Application::Get().GetWindow().GetNativeWindow();
#else
	EG_CORE_ASSERT(false);// FOR NOW THE APP IS ONLY FOR WIDNOWS USING GLFW
#endif // WIN32
	m_Device.init(win);
	m_SwapChain.create(win);
	EG_TRACE("Vulkan Renderer initialization Succesfull");
}

void eg::VulkanRenderer::cleanUp()
{
	m_Device.cleanUp();
	m_SwapChain.cleanUp();
	EG_TRACE("Vulkan Renderer Destruction Succesfull");
}

