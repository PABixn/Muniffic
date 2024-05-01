#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanIndexBuffer {
	public:
		//Might turn this into a template class later for different index types
		VulkanIndexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, uint32_t* indices, uint32_t indexCount);
		~VulkanIndexBuffer();

		VkBuffer getBuffer() { return buffer; }
		VkDeviceMemory getBufferMemory() { return bufferMemory; }
		uint32_t getIndexCount() { return indexCount; }
	private:
		VkBuffer buffer;
		VkDeviceMemory bufferMemory;
		uint32_t indexCount;
		VkDevice device;
	};
}