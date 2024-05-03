#include "egpch.h"
#include "VulkanCommandBuffers.h"
#include "VulkanConstants.h"
namespace eg {

	void VulkanCommandBuffers::Create(VkDevice device, VkCommandPool commandPool, uint32_t imageCount)
	{
		m_CommandBuffers.resize(VulkanConstants::MAX_FRAMES_IN_FLIGHT);
		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)m_CommandBuffers.size();

		if (vkAllocateCommandBuffers(device, &allocInfo, (VkCommandBuffer*)m_CommandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate command buffers!");
		}
	}

	void VulkanCommandBuffers::Reset(uint32_t index, VkCommandBufferResetFlags flags)
	{
		vkResetCommandBuffer(m_CommandBuffers[index], flags);
	}

}
