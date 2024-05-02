#pragma once
#define GLFW_INCLUDE_VULKAN
#include "VulkanDeviceMemory.h"
#include "GLFW/glfw3.h"

namespace eg {
	class VulkanDeviceMemoryManager {
	public:
		VulkanDeviceMemoryManager() = default;
		~VulkanDeviceMemoryManager() = default;

		static void Init(VkDevice device, VkPhysicalDevice physicalDevice);

		static VulkanDeviceMemory& AllocateMemory(VkMemoryRequirements memoryRequirements, VkMemoryPropertyFlags memoryProperties);
		static VulkanDeviceMemory& AllocateMemory(VkMemoryRequirements memoryRequirements, VkMemoryPropertyFlags memoryProperties, void* data, size_t size);
		static VulkanDeviceMemory& AllocateImageMemory(VkImage image, VkMemoryPropertyFlags memoryProperties);
		static VulkanDeviceMemory& AllocateBufferMemory(VkBuffer buffer, VkMemoryPropertyFlags memoryProperties);
		static VulkanDeviceMemory& AllocateCommandBufferMemory(VkCommandPool commandPool, VkCommandBuffer commandBuffer, VkMemoryPropertyFlags memoryProperties);

		static void FreeMemory(VulkanDeviceMemory& memory);

		static void SetDevice(VkDevice device) { m_Device = device; }
		static void SetPhysicalDevice(VkPhysicalDevice physicalDevice) { m_PhysicalDevice = physicalDevice; }
	private:
		static VkDevice m_Device;
		static VkPhysicalDevice m_PhysicalDevice;
	};
}