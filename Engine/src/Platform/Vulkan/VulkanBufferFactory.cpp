#include "egpch.h"
#include "VulkanBufferFactory.h"
#include "VulkanDeviceMemoryManager.h"
#include "VulkanCommandManager.h"
namespace eg {
    VulkanVertexBuffer& VulkanBufferFactory::CreateVertexBuffer(float* vertices, int size)
	{
         
		VkDeviceSize bufferSize = size;

		VulkanBuffer stagingBuffer = CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertices);

        VulkanBuffer VertexBuffer = CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        CopyBuffer(stagingBuffer, VertexBuffer, bufferSize);

		stagingBuffer.Destroy(m_device);

        VulkanVertexBuffer buffer(VertexBuffer);

        return buffer;
	}

    VulkanVertexBuffer& VulkanBufferFactory::CreateVertexBuffer(int size)
    {
        VkDeviceSize bufferSize = size;

		VulkanBuffer VertexBuffer = CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VulkanVertexBuffer buffer(VertexBuffer);

		return buffer;
    }

    void VulkanBufferFactory::DestroyBuffer(VulkanBuffer& buffer)
    {
		buffer.Destroy(m_device);
    }

    VulkanBuffer& VulkanBufferFactory::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
    {
        VulkanBuffer buffer;
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(m_device, &bufferInfo, nullptr, &buffer.m_Buffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create buffer!");
        }

        buffer.m_BufferMemory = VulkanDeviceMemoryManager::AllocateBufferMemory(buffer.m_Buffer, properties);

        buffer.m_Size = size;

        return buffer;
    }

    template <typename T>
    VulkanBuffer& VulkanBufferFactory::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, T* data)
    {
        VulkanBuffer buffer;
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer.m_Buffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create buffer!");
        }

        buffer.m_BufferMemory = VulkanDeviceMemoryManager::AllocateBufferMemory(buffer.m_Buffer, properties);

        VulkanDeviceMemoryManager::MapMemory(buffer.m_BufferMemory, size, usage, properties, data);

        buffer.m_Size = size;

        return buffer;
    }

    void VulkanBufferFactory::CopyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize& size)
    {
        VulkanCommandBuffer commandBuffer = VulkanCommandManager::BeginSingleTimeCommands();
        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;
        copyRegion.srcOffset = 0;
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
        VulkanCommandManager::EndSingleTimeCommands(commandBuffer);
    }

    void VulkanBufferFactory::CopyBuffer(VulkanBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize& size)
    {
        CopyBuffer(srcBuffer.m_Buffer, dstBuffer, size);
    }

    void VulkanBufferFactory::CopyBuffer(VkBuffer& srcBuffer, VulkanBuffer& dstBuffer, VkDeviceSize& size)
    {
        CopyBuffer(srcBuffer, dstBuffer.m_Buffer, size);
    }

    void VulkanBufferFactory::CopyBuffer(VulkanBuffer& srcBuffer, VulkanBuffer& dstBuffer, VkDeviceSize& size)
    {
		CopyBuffer(srcBuffer.m_Buffer, dstBuffer.m_Buffer, size);
    }

    VulkanIndexBuffer& VulkanBufferFactory::CreateIndexBuffer(uint32_t* vertices, uint32_t count)
    {
        VkDeviceSize bufferSize = count * sizeof(uint32_t);

		VulkanBuffer stagingBuffer = CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertices);

		VulkanBuffer indexBuffer = CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		CopyBuffer(stagingBuffer, indexBuffer, bufferSize);

		stagingBuffer.Destroy(m_device);

		VulkanIndexBuffer buffer(indexBuffer, count);

		return buffer;
    }

    VulkanUniformBuffer& VulkanBufferFactory::CreateUniformBuffer(uint32_t size, uint32_t binding)
    {
        VulkanUniformBuffer buffer;
        VkDeviceSize bufferSize = static_cast<uint64_t>(size);
        buffer.m_Buffer = CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        VulkanDeviceMemoryManager::MapMemoryBind(buffer.m_Buffer.m_BufferMemory, bufferSize, &buffer.mappedMemory);
        return buffer;
    }
}
