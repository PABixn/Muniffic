#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanDeviceMemory {
	public:
		VulkanDeviceMemory() = default;
		~VulkanDeviceMemory() = default;
		static VulkanDeviceMemory& Create(VkDevice m_Device, VkPhysicalDevice m_PhysicalDevice, VkMemoryRequirements memoryRequirements, VkMemoryPropertyFlags memoryProperties);
		static VulkanDeviceMemory& Create(VkDevice m_Device, VkPhysicalDevice m_PhysicalDevice, VkMemoryRequirements memoryRequirements, VkMemoryPropertyFlags memoryProperties, void* data, size_t size);
		static VulkanDeviceMemory& Create(VkDevice m_Device, VkPhysicalDevice m_PhysicalDevice, VkMemoryPropertyFlags memoryProperties, VkBuffer buffer);
		static VulkanDeviceMemory& Create(VkDevice m_Device, VkPhysicalDevice m_PhysicalDevice, VkMemoryPropertyFlags memoryProperties, VkImage image);

		static void Allocate(VkDevice device, VulkanDeviceMemory& deviceMemory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, float* data);
		void Cleanup(VkDevice device);

		VkDeviceMemory GetDeviceMemory() const { return m_deviceMemory; }
		static uint32_t FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
	private:
		VkDeviceMemory m_deviceMemory;
	};
}