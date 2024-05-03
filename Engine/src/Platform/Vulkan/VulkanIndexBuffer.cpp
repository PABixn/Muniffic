#include "egpch.h"
#include "VulkanIndexBuffer.h"
#include "VulkanDeviceMemoryManager.h"

namespace eg {
	void eg::VulkanIndexBuffer::Create(VkDevice device, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, VkQueue queue, uint32_t* indices, uint32_t indexCount)
	{
		VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

		VulkanBuffer stagingBuffer;
		stagingBuffer.CreateBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indices);

		m_Indexbuffer.CreateBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		
		m_Indexbuffer.CopyFrom(stagingBuffer, bufferSize);

		stagingBuffer.Destroy(device);
	}

	void eg::VulkanIndexBuffer::Cleanup(VkDevice device)
	{
		m_Indexbuffer.Destroy(device);
	}
}
