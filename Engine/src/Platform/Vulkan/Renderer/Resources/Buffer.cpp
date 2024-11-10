#include "Buffer.h"
#include "VulkanRenderer.h"

void eg::VulkanBuffer::Create(const VkDeviceSize& size, const VkBufferUsageFlags& usage, const VkMemoryPropertyFlags& properties)
{
	Device& device = VRen::get().getDevice();
	// Buffer creation
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	
	if (vkCreateBuffer(device.getNativeDevice(), &bufferInfo, nullptr, &m_Buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer!");
	}
	m_Size = size;

	// Checking memory requirements
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device.getNativeDevice(), m_Buffer, &memRequirements);

	// Memory allocation
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
	if (vkAllocateMemory(device.getNativeDevice(), &allocInfo, nullptr, &m_Memory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	// Memory binding
	vkBindBufferMemory(device.getNativeDevice(), m_Buffer, m_Memory, 0);

}

void eg::VulkanBuffer::Destroy()
{
	vkDestroyBuffer(VRen::get().getNativeDevice(), m_Buffer, nullptr);
	vkFreeMemory(VRen::get().getNativeDevice(), m_Memory, nullptr);
}
