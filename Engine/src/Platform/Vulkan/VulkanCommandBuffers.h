#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanCommandBuffer.h"
#include <GLFW/glfw3.h>

namespace eg {
	// Wrapper for Vulkan command buffers (don't think it's necessary)
	class VulkanCommandBuffers {
	public:
		VulkanCommandBuffers();
		~VulkanCommandBuffers();

		void createCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t imageCount);
		void destroyCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t imageCount);

		VkCommandBuffer getCommandBuffer(uint32_t index) const { return (VkCommandBuffer)m_commandBuffers[index]; }
		const std::vector<VulkanCommandBuffer>& getCommandBuffers() const { return m_commandBuffers; }
	private:
		std::vector<VulkanCommandBuffer> m_commandBuffers;
	};
}