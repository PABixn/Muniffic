#include "egpch.h"
#include "VulkanBuffer.h"
#include "VulkanDeviceMemoryManager.h"
#include "VulkanCommandManager.h"
#include "VulkanCommandBuffer.h"
namespace eg {

    void VulkanBuffer::FreeMemory(VkDevice device)
    {
        m_BufferMemory.Cleanup(device);
    }

    void VulkanBuffer::Destroy(VkDevice device)
    {
        vkDestroyBuffer(device, m_Buffer, nullptr);
		m_BufferMemory.Cleanup(device);
        m_Size = 0;
    }

}