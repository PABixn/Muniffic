#include "egpch.h"
#include "VulkanCommandManager.h"

namespace eg {
    void VulkanCommandManager::Init(VkDevice logicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool)
    {
		m_logicalDevice = logicalDevice;
		m_graphicsQueue = graphicsQueue;
		m_commandPool = commandPool;
    }
    VulkanCommandBuffer VulkanCommandManager::BeginSingleTimeCommands()
    {
        VulkanCommandBuffer commandBuffer;
        commandBuffer.BeginSingleTimeCommands(m_logicalDevice, m_commandPool);
        return commandBuffer;
    }

    void VulkanCommandManager::EndSingleTimeCommands(VulkanCommandBuffer& commandBuffer)
    {
		commandBuffer.EndSingleTimeCommands(m_logicalDevice, m_graphicsQueue, m_commandPool);
    }

    void VulkanCommandManager::RecordCommandBuffer(VkRenderPass renderPass, VkExtent2D swapChainextent, VkFramebuffer framebuffer, VkPipeline pipeline, VkPipelineLayout pipelineLayout, VkDescriptorSet descriptorSet, VkBuffer vertexBuffer, VkBuffer indexBuffer, uint32_t indexCount)
    {
    }
}