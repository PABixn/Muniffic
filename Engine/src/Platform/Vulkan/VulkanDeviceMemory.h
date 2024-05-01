#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanDeviceMemory {
	public:
		VulkanDeviceMemory(VkDevice device, VkMemoryPropertyFlags properties, VkImage& image);
		VulkanDeviceMemory(VkDevice device, VkMemoryPropertyFlags properties, VkBuffer& buffer);
		~VulkanDeviceMemory();

		VkDeviceMemory GetDeviceMemory() const { return m_deviceMemory; }
	private:
		VkDeviceMemory m_deviceMemory;
	};
}