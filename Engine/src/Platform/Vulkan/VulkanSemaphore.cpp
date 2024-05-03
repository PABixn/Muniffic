#include "egpch.h"
#include "VulkanSemaphore.h"
#include "Engine/Core/Core.h"
namespace eg {
	VulkanSemaphore::VulkanSemaphore(VkDevice device)
	{
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_Semaphore) != VK_SUCCESS) {
			EG_CORE_ERROR("Failed to create semaphore!");
		}
	}

	VulkanSemaphore::~VulkanSemaphore()
	{
		vkDestroySemaphore(m_Device, m_Semaphore, nullptr);
	}
}
