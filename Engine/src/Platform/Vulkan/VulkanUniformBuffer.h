#pragma once
#define GLFW_INCLUDE_VULKAN
#include "VulkanBuffer.h"
#include "Engine/Renderer/UniformBuffer.h"
#include "Engine/Core/Core.h"
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanUniformBuffer : public UniformBuffer{
	public:
		VulkanUniformBuffer() = default;
		VulkanUniformBuffer(const VulkanUniformBuffer&) = default;
		VulkanUniformBuffer(VulkanUniformBuffer&) = default;
		~VulkanUniformBuffer() = default;

		static Ref<VulkanUniformBuffer> Create(uint32_t size, uint32_t binding);
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

		const VulkanBuffer& GetBuffer() { return m_Buffer; }
		void* GetMappedMemory() { return mappedMemory; }

		friend class VulkanBufferFactory;
	private:
		VulkanBuffer m_Buffer;
		void* mappedMemory;
	};
}