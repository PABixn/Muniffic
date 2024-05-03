#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanBuffer.h"
namespace eg {
	class VulkanIndexBuffer {
	public:
		//Might turn this into a template class later for different index types
		VulkanIndexBuffer() = default;
		~VulkanIndexBuffer() = default;

		void Create(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, uint32_t* indices, uint32_t indexCount);
		void Cleanup(VkDevice device);

		VulkanBuffer& getBuffer() { return m_Indexbuffer; }
		uint32_t getIndexCount() { return indexCount; }
	private:
		VulkanBuffer m_Indexbuffer;
		uint32_t indexCount;
	};
}