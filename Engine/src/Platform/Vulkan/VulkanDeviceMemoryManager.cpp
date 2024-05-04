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
		VulkanDeviceMemory memory = VulkanDeviceMemory::Create(m_Device, m_PhysicalDevice, memoryRequirements, memoryProperties);
		return memory;
	}

	VulkanDeviceMemory& VulkanDeviceMemoryManager::AllocateMemory(VkMemoryRequirements memoryRequirements, VkMemoryPropertyFlags memoryProperties, void* data, size_t size)
	{
		VulkanDeviceMemory memory = VulkanDeviceMemory::Create(m_Device, m_PhysicalDevice, memoryRequirements, memoryProperties, data, size);
		return memory;
	}

	void VulkanDeviceMemoryManager::MapMemory(VulkanDeviceMemory& deviceMemory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, float* data)
	{
		VulkanDeviceMemory::Allocate(m_Device, deviceMemory, size, usage, properties, data);
	}

	/*VulkanDeviceMemory& VulkanDeviceMemoryManager::AllocateMemory(VkMemoryRequirements memoryRequirements, VkMemoryPropertyFlags memoryProperties, void* data, size_t size, VkBuffer buffer)
	{
		VulkanDeviceMemory memory = VulkanDeviceMemory::Create(m_Device, m_PhysicalDevice, memoryRequirements, memoryProperties, data, size, buffer);
		return memory;
	}*/

	VulkanDeviceMemory& VulkanDeviceMemoryManager::AllocateImageMemory(VkImage image, VkMemoryPropertyFlags memoryProperties)
	{
		VulkanDeviceMemory memory = VulkanDeviceMemory::Create(m_Device, m_PhysicalDevice, memoryProperties, image);
		return memory;
	}

	VulkanDeviceMemory& VulkanDeviceMemoryManager::AllocateBufferMemory(VkBuffer buffer, VkMemoryPropertyFlags memoryProperties)
	{
		VulkanDeviceMemory memory = VulkanDeviceMemory::Create(m_Device, m_PhysicalDevice, memoryProperties, buffer);
		return memory;
	}
	

	void VulkanDeviceMemoryManager::FreeMemory(VulkanDeviceMemory& memory)
	{
		memory.Cleanup(m_Device);
	}
}