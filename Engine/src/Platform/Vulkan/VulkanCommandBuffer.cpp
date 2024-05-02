#include "egpch.h"
#include "VulkanCommandBuffer.h"

namespace eg {
    VulkanCommandBuffer eg::VulkanCommandBuffer::BeginSingleTimeCommands(VkDevice logicalDevice, VkCommandPool commandPool)
    {
		VulkanCommandBuffer vulkanCommandBuffer;
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = commandPool;
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);
		vulkanCommandBuffer.m_commandBuffer = commandBuffer;
		return vulkanCommandBuffer;
    }

	void VulkanCommandBuffer::EndSingleTimeCommands(VkDevice logicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool)
	{
		vkEndCommandBuffer(m_commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_commandBuffer;

		vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(graphicsQueue);

		vkFreeCommandBuffers(logicalDevice, commandPool, 1, &m_commandBuffer);
	}

	void VulkanCommandBuffer::RecordCommandBuffer(VkDevice logicalDevice, VkCommandPool commandPool, VkRenderPass renderPass, VkExtent2D swapChainextent, VkFramebuffer framebuffer, VkPipeline pipeline, VkPipelineLayout pipelineLayout, VkDescriptorSet descriptorSet, VkBuffer vertexBuffer, VkBuffer indexBuffer, uint32_t indexCount)
	{

	}
}