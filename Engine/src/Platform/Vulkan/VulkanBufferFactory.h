#pragma once
#define GLFW_INCLUDE_VULKAN
#include "VulkanBuffer.h"
#include "VulkanIndexBuffer.h"
#include "VulkanVertex.h"
#include "VulkanUniformBuffer.h"
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanBufferFactory {
	public:
		static VulkanBufferFactory* GetInstance() {
			if (!s_instance)
				s_instance = new VulkanBufferFactory();
			return s_instance;
		}
		VulkanBuffer& CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		template <typename T>
		VulkanBuffer& CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, T* data);

		void CopyBuffer(VulkanBuffer& srcBuffer, VulkanBuffer& dstBuffer, VkDeviceSize& size);
		void CopyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize& size);
		void CopyBuffer(VulkanBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize& size);
		void CopyBuffer(VkBuffer& srcBuffer, VulkanBuffer& dstBuffer, VkDeviceSize& size);

		VulkanBuffer& CreateIndexBuffer(uint32_t* vertices, uint32_t count);
		VulkanUniformBuffer& CreateUniformBuffer(uint32_t size, uint32_t binding);
		VulkanVertexBuffer& CreateVertexBuffer(float* vertices, int size, BufferLayout& layout);
		VulkanVertexBuffer& CraeteVertexBuffer(int size, BufferLayout& layout);
		VulkanVertexBuffer& CreateVertexBuffer(float* vertices, int size);
		VulkanVertexBuffer& CreateVertexBuffer(int size);

		void DestroyBuffer(VulkanBuffer& buffer);
	private:
		VulkanBufferFactory(const VulkanBufferFactory&) = delete;
		VulkanBufferFactory() = default;
	private:
		static VulkanBufferFactory* s_instance;
		VkDevice m_device;
		VkPhysicalDevice m_physicalDevice;
		VkCommandPool m_commandPool;
	};
}