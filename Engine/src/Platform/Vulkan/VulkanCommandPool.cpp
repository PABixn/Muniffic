#include "egpch.h"
#include "VulkanCommandPool.h"
#include "VulkanUtils.h"

void eg::VulkanCommandPool::Create(VkCommandPoolCreateFlags flags)
{
	VulkanHandler* handler = GetVulkanHandler();
	VkPhysicalDevice physicalDevice = handler->GetVulkanPhysicalDevice().GetPhysicalDevice();
	VkSurfaceKHR surface = handler->GetVulkanSurface().GetSurface();
	VkDevice logicalDevice = handler->GetVulkanLogicalDevice().GetDevice();
	VulkanQueueFamilyIndices queueFamilyIndices = VulkanQueue::FindQueueFamilies(physicalDevice, surface);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	if (vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}
}

void eg::VulkanCommandPool::Cleanup()
{
	VulkanHandler* handler = GetVulkanHandler();
	VkDevice device = handler->GetVulkanLogicalDevice().GetDevice();
	vkDestroyCommandPool(device, m_commandPool, nullptr);
}
