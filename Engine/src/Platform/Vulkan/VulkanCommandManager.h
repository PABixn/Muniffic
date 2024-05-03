#pragma once
#define GLFW_INCLUDE_VULKAN
#include "VulkanCommandBuffer.h"
#include "VulkanQueue.h"
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanCommandManager {
	public:
		static void Init(VkDevice logicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool);
		static VulkanCommandBuffer BeginSingleTimeCommands();
		static void EndSingleTimeCommands(VulkanCommandBuffer& commandBuffer);
		static void RecordCommandBuffer(VkRenderPass renderPass, VkExtent2D swapChainextent, VkFramebuffer framebuffer, VkPipeline pipeline, VkPipelineLayout pipelineLayout, VkDescriptorSet descriptorSet, VkBuffer vertexBuffer, VkBuffer indexBuffer, uint32_t indexCount);
	private:
		static VkDevice m_logicalDevice;
		static VkQueue m_graphicsQueue;
		static VkCommandPool m_commandPool;
	};
}