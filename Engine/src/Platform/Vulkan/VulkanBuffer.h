#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanDeviceMemory.h"
#include "VulkanVertex.h"
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanBuffer {
	public:
			VulkanBuffer() = default;
			~VulkanBuffer() = default;

			static VulkanBuffer CreateBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
			template <typename T>
			static VulkanBuffer CreateBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, T* data);
			
			static VulkanBuffer CreateUniformBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, const VulkanVertexInputLayout& layout);
			void CopyTo(VulkanBuffer dstBuffer, VkDeviceSize size);
			void CopyFrom(VulkanBuffer srcBuffer, VkDeviceSize size);
			static void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
			static void CopyBuffer(VkBuffer srcBuffer, VulkanBuffer dstBuffer, VkDeviceSize size);
			static void CopyBuffer(VulkanBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
			static void CopyBuffer(VulkanBuffer srcBuffer, VulkanBuffer dstBuffer, VkDeviceSize size);

			void FreeMemory(VkDevice device);
			void Destroy(VkDevice device);

			const VkBuffer& GetBuffer() const { return m_Buffer; }
			const VulkanDeviceMemory& GetBufferMemory() const { return m_BufferMemory; }
			const size_t& GetSize() const { return m_Size; }
			VkBuffer GetBuffer() { return m_Buffer; }
			VulkanDeviceMemory GetBufferMemory() { return m_BufferMemory; }
			VulkanDeviceMemory& GetBufferMemoryRef() { return m_BufferMemory; }
			size_t GetSize() { return m_Size; }
	private:
		VkBuffer m_Buffer;
		VulkanDeviceMemory m_BufferMemory;
		size_t m_Size;
	};
}