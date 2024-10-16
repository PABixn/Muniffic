#include <iostream>
#include "Platform/Vulkan/VulkanRenderer.h"
#include "ResourceManager.h"

std::vector<eg::VulkanVertex> eg::Vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};
std::vector<uint16_t> eg::Indices = {
	0, 1, 2, 2, 3, 0
};

// Helper
uint32_t eg::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(VRen::get().getDevice().m_PhysicalDevice, &memProperties);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");

}

// General usage functions
eg::VulkanBuffer eg::ResourceManager::createBuffer(const VkDeviceSize& size, const VkBufferUsageFlags& usage, const VkMemoryPropertyFlags& properties)
{
	Device& device = VRen::get().getDevice();
	// Buffer creation
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	VulkanBuffer Buffer;
	if (vkCreateBuffer(device.getNativeDevice(), &bufferInfo, nullptr, &Buffer.m_Buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer!");
	}
	Buffer.m_Size = size;

	// Checking memory requirements
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device.getNativeDevice(), Buffer.m_Buffer, &memRequirements);

	// Memory allocation
	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
	if (vkAllocateMemory(device.getNativeDevice(), &allocInfo, nullptr, &Buffer.m_Memory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	// Memory binding
	vkBindBufferMemory(device.getNativeDevice(), Buffer.m_Buffer, Buffer.m_Memory, 0);

	return Buffer;
}

void eg::ResourceManager::DestroyBuffer(VulkanBuffer& buffer)
{
	vkDestroyBuffer(VRen::get().getNativeDevice(), buffer.m_Buffer, nullptr);
	vkFreeMemory(VRen::get().getNativeDevice(), buffer.m_Memory, nullptr);
}

void eg::ResourceManager::LoadModel(const char* path)
{
	VulkanBuffer stagingBuffer = createBuffer(Vertices.size() * sizeof(VulkanVertex), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	copyDataToBuffer(stagingBuffer, (void*)Vertices.data());
	copyBuffer(stagingBuffer, m_VertexBuffer.m_Buffer, Vertices.size() * sizeof(VulkanVertex));
	DestroyBuffer(stagingBuffer);


	VulkanBuffer stagingBufferIndex = createBuffer(Indices.size() * sizeof(Indices[0]), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	copyDataToBuffer(stagingBufferIndex, (void*)Indices.data());
	copyBuffer(stagingBufferIndex, m_IndexBuffer, Indices.size() * sizeof(Indices[0]));
	DestroyBuffer(stagingBufferIndex);

	m_IndicesCount = Indices.size();
	m_VertexBuffer.m_VerticesCount = Vertices.size();
	m_ModelLoaded = true;
}

void eg::ResourceManager::copyDataToBuffer(VulkanBuffer& buffer, void* data)
{
	void* mappedData;
	vkMapMemory(VRen::get().getNativeDevice(), buffer.m_Memory, 0, buffer.m_Size, 0, &mappedData);
	memcpy(mappedData, data, (size_t)buffer.m_Size);
	vkUnmapMemory(VRen::get().getNativeDevice(), buffer.m_Memory);

}

void eg::ResourceManager::copyBuffer(VulkanBuffer& srcBuffer, VulkanBuffer& destBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = VRen::get().BeginSingleTimeCommands();

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer.m_Buffer, destBuffer.m_Buffer, 1, &copyRegion);

	VRen::get().EndSingleTimeCommands(commandBuffer);
}


// Initialization and cleanup
void eg::ResourceManager::init()
{
	m_DescriptorManager.init(this);
	m_VertexBuffer.m_Buffer = createBuffer(Vertices.size() * sizeof(VulkanVertex), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	m_IndexBuffer = createBuffer(Indices.size() * sizeof(Indices[0]), VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	LoadModel("just test");
	m_TextureManager.init();

}

void eg::ResourceManager::cleanUp()
{
	m_DescriptorManager.cleanUp();
	DestroyBuffer(m_VertexBuffer.m_Buffer);
	DestroyBuffer(m_IndexBuffer);
}

