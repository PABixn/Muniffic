#include "egpch.h"
#include "VulkanDeviceMemory.h"

namespace eg {
	VulkanDeviceMemory& VulkanDeviceMemory::Create(VkDevice device, VkPhysicalDevice physicalDevice, VkMemoryRequirements memoryRequirements, VkMemoryPropertyFlags memoryProperties)
	{
		VulkanDeviceMemory deviceMemory;
		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memoryRequirements.memoryTypeBits, memoryProperties);;

		if (vkAllocateMemory(device, &allocInfo, nullptr, &deviceMemory.m_deviceMemory) != VK_SUCCESS) {
			EG_CORE_ASSERT("failed to allocate buffer memory!");
		}

		return deviceMemory;
	}
	VulkanDeviceMemory& VulkanDeviceMemory::Create(VkDevice m_Device, VkPhysicalDevice m_PhysicalDevice, VkMemoryRequirements memoryRequirements, VkMemoryPropertyFlags memoryProperties, void* data, size_t size)
	{
		VulkanDeviceMemory deviceMemory;
		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(m_PhysicalDevice, memoryRequirements.memoryTypeBits, memoryProperties);;

		if (vkAllocateMemory(m_Device, &allocInfo, nullptr, &deviceMemory.m_deviceMemory) != VK_SUCCESS) {
			EG_CORE_ASSERT("failed to allocate buffer memory!");
		}

		void* mappedData;
		vkMapMemory(m_Device, deviceMemory.m_deviceMemory, 0, size, 0, &mappedData);
		memcpy(mappedData, data, size);
		vkUnmapMemory(m_Device, deviceMemory.m_deviceMemory);

		return deviceMemory;
	}

	VulkanDeviceMemory& VulkanDeviceMemory::Create(VkDevice device, VkPhysicalDevice physicalDevice, VkMemoryPropertyFlags memoryProperties, VkBuffer buffer)
	{
		VulkanDeviceMemory deviceMemory;
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, memoryProperties);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &deviceMemory.m_deviceMemory) != VK_SUCCESS) {
			EG_CORE_ASSERT("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(device, buffer, deviceMemory.m_deviceMemory, 0);

		return deviceMemory;
	}

	VulkanDeviceMemory& VulkanDeviceMemory::Create(VkDevice device, VkPhysicalDevice physicalDevice, VkMemoryPropertyFlags memoryProperties, VkImage image)
	{
		VulkanDeviceMemory deviceMemory;
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, memoryProperties);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &deviceMemory.m_deviceMemory) != VK_SUCCESS) {
			EG_CORE_ASSERT("failed to allocate image memory!");
		}

		vkBindImageMemory(device, image, deviceMemory.m_deviceMemory, 0);
		return deviceMemory;
	}

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
