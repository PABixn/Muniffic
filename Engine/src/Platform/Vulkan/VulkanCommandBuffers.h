#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanCommandBuffers {
	public:
		VulkanCommandBuffers();
		~VulkanCommandBuffers();

		void createCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t imageCount);
		void destroyCommandBuffers(VkDevice device, VkCommandPool commandPool, uint32_t imageCount);

		VkCommandBuffer getCommandBuffer(uint32_t index) const { return m_commandBuffers[index]; }
		const std::vector<VkCommandBuffer>& getCommandBuffers() const { return m_commandBuffers; }
	private:
		std::vector<VkCommandBuffer> m_commandBuffers;
	};
}