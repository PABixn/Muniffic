#include "SamplerArray.h"
#include "Platform/Vulkan/Renderer/Resources/Scene/ObjectRenderData.h"
#include <glm/glm.hpp>
#include "VulkanRenderer.h"
uint32_t eg::VulkanSamplerArray::addSampler(VkSampler* sampler)
{
	if (m_LastOffset + sizeof(VkSampler) <= m_Buffer.m_Size)
	{
		uintptr_t srcAddress = reinterpret_cast<uintptr_t>(m_Mapped);
		uintptr_t incrementedAddress = srcAddress + m_LastOffset;
		void* whereStart = reinterpret_cast<void*>(incrementedAddress);
		memcpy(whereStart, sampler, sizeof(VkSampler));
		m_LastOffset += sizeof(VkSampler);
		return m_SamplerCount++;
	}
	else
	{
		EG_ASSERT(false, "For now every sampler should fit into one samplerArray");// TO-DO Vulkan
	}
}

void eg::VulkanSamplerArray::create(size_t size)
{
	m_Buffer.Create(size, VK_BUFFER_USAGE_SAMPLER_DESCRIPTOR_BUFFER_BIT_EXT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	VkResult res = vkMapMemory(VRen::get().getNativeDevice(), m_Buffer.m_Memory, 0, m_Buffer.m_Size, 0, &m_Mapped);
	EG_ASSERT(res == 0);
}

void eg::VulkanSamplerArray::destroy()
{
	vkUnmapMemory(VRen::get().getNativeDevice(), m_Buffer.m_Memory);
	m_Buffer.Destroy();
}
