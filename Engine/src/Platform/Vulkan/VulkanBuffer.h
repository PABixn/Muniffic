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

			void FreeMemory(VkDevice device);
			void Destroy(VkDevice device);

			const VkBuffer& GetBuffer() const { return m_Buffer; }
			const VulkanDeviceMemory& GetBufferMemory() const { return m_BufferMemory; }
			const size_t& GetSize() const { return m_Size; }
			VkBuffer& GetBuffer() { return m_Buffer; }
			VulkanDeviceMemory& GetBufferMemory() { return m_BufferMemory; }
			VulkanDeviceMemory& GetBufferMemoryRef() { return m_BufferMemory; }
			size_t GetSize() { return m_Size; }

			friend class VulkanBufferFactory;
	private:
		VkBuffer m_Buffer;
		VulkanDeviceMemory m_BufferMemory;
		size_t m_Size;
	};
}