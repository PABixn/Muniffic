#include "egpch.h"
#include "VulkanDeviceMemory.h"
#include "VulkanCommandManager.h"
namespace eg {

	void VulkanDeviceMemory::Cleanup(VkDevice device)
	{
		vkFreeMemory(device, m_deviceMemory, nullptr);
	}

	uint32_t VulkanDeviceMemory::FindMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		EG_CORE_ASSERT("failed to find suitable memory type!");
    }   
}
