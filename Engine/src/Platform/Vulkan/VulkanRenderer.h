#pragma once
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif // !RENDERER_INCLUDE_GLFW_VULKAN
#include "Platform/Vulkan/Renderer/Core/Device/Device.h"
namespace eg {
	class VulkanRenderer
	{
	public:
		void init();
		void cleanUp();
		Device& getDevice() { return m_Device; }
		VkDevice& getNativeDevice() { return m_Device.m_LogicalDevice; }
	private:
		Device m_Device;
	};
}