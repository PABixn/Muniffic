#include "egpch.h"
#include "VulkanContext.h"

namespace eg {
	VulkanContext::VulkanContext(GLFWwindow* windowHandle): m_WindowHandle(windowHandle), m_VulkanHandler(windowHandle) {}

	void VulkanContext::Init() {
		m_VulkanHandler.Init();
	}
	void VulkanContext::SwapBuffers()
	{
		m_VulkanHandler.DrawFrame();
	}
}