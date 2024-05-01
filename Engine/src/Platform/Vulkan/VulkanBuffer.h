#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanDeviceMemory.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanBuffer {
	public:
			VulkanBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
			~VulkanBuffer();

			void CopyTo(VkBuffer dstBuffer, VkDeviceSize size);
			void CopyFrom(VkBuffer dstBuffer, VkDeviceSize size);

			VkBuffer GetBuffer() { return m_Buffer; }
			VulkanDeviceMemory GetBufferMemory() { return m_BufferMemory; }
	private:
		VkBuffer m_Buffer;
		VulkanDeviceMemory m_BufferMemory;
	};
}