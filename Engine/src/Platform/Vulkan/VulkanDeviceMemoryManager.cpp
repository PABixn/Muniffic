#include "egpch.h"
#include "VulkanDeviceMemoryManager.h"

namespace eg {

	void VulkanDeviceMemoryManager::Init(VkDevice device, VkPhysicalDevice physicalDevice)
	{
		m_Device = device;
		m_PhysicalDevice = physicalDevice;
	}

	VulkanDeviceMemory& VulkanDeviceMemoryManager::AllocateMemory(VkMemoryRequirements memoryRequirements, VkMemoryPropertyFlags memoryProperties)
	{
		VulkanDeviceMemory deviceMemory;
		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = VulkanDeviceMemory::FindMemoryType(m_PhysicalDevice, memoryRequirements.memoryTypeBits, memoryProperties);;

		if (vkAllocateMemory(m_Device, &allocInfo, nullptr, &deviceMemory.m_deviceMemory) != VK_SUCCESS) {
			EG_CORE_ASSERT("failed to allocate buffer memory!");
		}

		return deviceMemory;
	}

	VulkanDeviceMemory& VulkanDeviceMemoryManager::AllocateMemory(VkMemoryRequirements memoryRequirements, VkMemoryPropertyFlags memoryProperties, void* data, size_t size)
	{
		VulkanDeviceMemory deviceMemory;
		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = VulkanDeviceMemory::FindMemoryType(m_PhysicalDevice, memoryRequirements.memoryTypeBits, memoryProperties);;

		if (vkAllocateMemory(m_Device, &allocInfo, nullptr, &deviceMemory.m_deviceMemory) != VK_SUCCESS) {
			EG_CORE_ASSERT("failed to allocate buffer memory!");
		}

		void* mappedData;
		vkMapMemory(m_Device, deviceMemory.m_deviceMemory, 0, size, 0, &mappedData);
		memcpy(mappedData, data, size);
		vkUnmapMemory(m_Device, deviceMemory.m_deviceMemory);

		return deviceMemory;
	}

	void VulkanDeviceMemoryManager::MapMemory(VulkanDeviceMemory& deviceMemory, VkDeviceSize size, float* data)
	{
		void* mappedData;
		vkMapMemory(m_Device, deviceMemory.m_deviceMemory, 0, size, 0, &mappedData);
		memcpy(mappedData, data, size);
		vkUnmapMemory(m_Device, deviceMemory.m_deviceMemory);
	}

	void VulkanDeviceMemoryManager::MapMemoryBind(VulkanDeviceMemory& deviceMemory, VkDeviceSize size, void** data)
	{
		vkMapMemory(m_Device, deviceMemory.m_deviceMemory, 0, size, 0, data);
	}

	void VulkanDeviceMemoryManager::FreeMemory(VulkanDeviceMemory& memory)
	{
		vkFreeMemory(m_Device, memory.m_deviceMemory, nullptr);
		memory.m_deviceMemory = VK_NULL_HANDLE;
	}

	VulkanDeviceMemory& VulkanDeviceMemoryManager::AllocateImageMemory(VkImage image, VkMemoryPropertyFlags memoryProperties)
	{
		VulkanDeviceMemory deviceMemory;
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(m_Device, image, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VulkanDeviceMemory::FindMemoryType(m_PhysicalDevice, memRequirements.memoryTypeBits, memoryProperties);

		if (vkAllocateMemory(m_Device, &allocInfo, nullptr, &deviceMemory.m_deviceMemory) != VK_SUCCESS) {
			EG_CORE_ASSERT("failed to allocate image memory!");
		}

		vkBindImageMemory(m_Device, image, deviceMemory.m_deviceMemory, 0);
		return deviceMemory;
	}

	VulkanDeviceMemory& VulkanDeviceMemoryManager::AllocateBufferMemory(VkBuffer buffer, VkMemoryPropertyFlags memoryProperties)
	{
		VulkanDeviceMemory deviceMemory;
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(m_Device, buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VulkanDeviceMemory::FindMemoryType(m_PhysicalDevice, memRequirements.memoryTypeBits, memoryProperties);

		if (vkAllocateMemory(m_Device, &allocInfo, nullptr, &deviceMemory.m_deviceMemory) != VK_SUCCESS) {
			EG_CORE_ASSERT("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(m_Device, buffer, deviceMemory.m_deviceMemory, 0);

		return deviceMemory;
	}
}