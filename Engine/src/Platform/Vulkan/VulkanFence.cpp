#include "egpch.h"
#include "VulkanFence.h"
#include "Engine/Core/Core.h"
namespace eg {

	VulkanFence::VulkanFence(VkDevice device, bool signaled)
	{
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

		if (vkCreateFence(device, &fenceInfo, nullptr, &m_Fence) != VK_SUCCESS) {
			EG_ASSERT("Failed to create fence!");
		}
	}

	void VulkanFence::Reset()
	{
		vkResetFences(m_Device, 1, &m_Fence);
	}

	void VulkanFence::Wait()
	{
		vkWaitForFences(m_Device, 1, &m_Fence, VK_TRUE, UINT64_MAX);
	}

	void VulkanFence::Wait(uint64_t timeout)
	{
		vkWaitForFences(m_Device, 1, &m_Fence, VK_TRUE, timeout);
	}

}