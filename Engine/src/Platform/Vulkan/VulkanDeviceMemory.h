#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanDeviceMemory {
	public:
		VulkanDeviceMemory() = default;
		~VulkanDeviceMemory() = default;

		void Cleanup(VkDevice device);

		VkDeviceMemory GetDeviceMemory() const { return m_deviceMemory; }
		static uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

		friend class VulkanDeviceMemoryManager;
	private:
		VkDeviceMemory m_deviceMemory;
	};
}