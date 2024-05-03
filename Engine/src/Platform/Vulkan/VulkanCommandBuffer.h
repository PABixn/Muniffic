#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {

	class VulkanCommandBuffer {
	public:
		VulkanCommandBuffer BeginSingleTimeCommands(VkDevice logicalDevice,  VkCommandPool commandPool);
		void EndSingleTimeCommands(VkDevice logicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool);
		void RecordCommandBuffer(VkDevice logicalDevice, VkCommandPool commandPool, VkRenderPass renderPass, VkExtent2D swapChainextent,VkFramebuffer framebuffer, VkPipeline pipeline, VkPipelineLayout pipelineLayout, VkDescriptorSet descriptorSet, VkBuffer vertexBuffer, VkBuffer indexBuffer, uint32_t indexCount);

		void Reset(VkCommandBufferResetFlags flags = 0);
		VkCommandBuffer getCommandBuffer() const { return m_commandBuffer; }

		operator VkCommandBuffer() const { return m_commandBuffer; }
	private:
		VkCommandBuffer m_commandBuffer;
	};

}