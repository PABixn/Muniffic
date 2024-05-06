#include "egpch.h"
#include "VulkanUniformBuffer.h"
#include "VulkanBufferFactory.h"
namespace eg {

	Ref<VulkanUniformBuffer> VulkanUniformBuffer::Create(uint32_t size, uint32_t binding) {
		return CreateRef<VulkanUniformBuffer>(VulkanBufferFactory::GetInstance()->CreateUniformBuffer(size, binding));
	}

	void VulkanUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		memcpy((void*)(offset + (uint8_t*)mappedMemory), data, size);
	}

}