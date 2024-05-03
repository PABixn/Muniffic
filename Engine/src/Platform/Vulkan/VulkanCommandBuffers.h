#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanCommandBuffer.h"
#include <GLFW/glfw3.h>

namespace eg {
	// Wrapper for Vulkan command buffers (don't think it's necessary)
	class VulkanCommandBuffers {
	public:
		VulkanCommandBuffers() = default;
		~VulkanCommandBuffers() = default;

		void Create(VkDevice device, VkCommandPool commandPool, uint32_t imageCount);
		void Reset(uint32_t index, VkCommandBufferResetFlags flags);

		VkCommandBuffer getCommandBuffer(uint32_t index) const { return (VkCommandBuffer)m_CommandBuffers[index]; }
		const std::vector<VulkanCommandBuffer>& getCommandBuffers() const { return m_CommandBuffers; }
	private:
		std::vector<VulkanCommandBuffer> m_CommandBuffers;
	};
}