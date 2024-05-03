#include "egpch.h"
#include "VulkanBuffer.h"
#include "VulkanDeviceMemoryManager.h"
#include "VulkanCommandManager.h"
#include "VulkanCommandBuffer.h"
namespace eg {

    VulkanBuffer VulkanBuffer::CreateBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
    {
        VulkanBuffer buffer;
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &buffer.m_Buffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create buffer!");
        }

        VulkanDeviceMemoryManager::AllocateBufferMemory(buffer.m_Buffer, properties);

        buffer.m_Size = size;

        return buffer;
    }

    VulkanBuffer VulkanBuffer::CreateUniformBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, const VulkanVertexInputLayout& layout)
    {
        VkDeviceSize bufferSize = static_cast<uint64_t>(layout.GetStride());
        VulkanBuffer buffer = CreateBuffer(logicalDevice, physicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        return buffer;
    }

    void VulkanBuffer::CopyTo(VulkanBuffer dstBuffer, VkDeviceSize size)
    {
        CopyBuffer(m_Buffer, dstBuffer, size);
    }

    void VulkanBuffer::CopyFrom(VulkanBuffer dstBuffer, VkDeviceSize size)
    {
        CopyBuffer(dstBuffer, m_Buffer, size);
    }

    void VulkanBuffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
    {
        VulkanCommandBuffer commandBuffer = VulkanCommandManager::BeginSingleTimeCommands();
        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;
        copyRegion.srcOffset = 0;
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
        VulkanCommandManager::EndSingleTimeCommands(commandBuffer);
    }

    void VulkanBuffer::CopyBuffer(VkBuffer srcBuffer, VulkanBuffer dstBuffer, VkDeviceSize size)
    {
		CopyBuffer(srcBuffer, dstBuffer.m_Buffer, size);
		dstBuffer.m_Size = size;
    }

    void VulkanBuffer::CopyBuffer(VulkanBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
    {
        CopyBuffer(srcBuffer.m_Buffer, dstBuffer, size);
        srcBuffer.m_Size = size;
    }

    void VulkanBuffer::CopyBuffer(VulkanBuffer srcBuffer, VulkanBuffer dstBuffer, VkDeviceSize size)
    {
		CopyBuffer(srcBuffer.m_Buffer, dstBuffer.m_Buffer, size);
        dstBuffer.m_Size = size;
    }

}