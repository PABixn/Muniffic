#pragma once

#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#endif // !RENDERER_INCLUDE_GLFW_VULKAN

namespace eg {
	struct VulkanBuffer {
		size_t m_Size;
		VkBuffer m_Buffer;
		VkDeviceMemory m_Memory;
	};
}