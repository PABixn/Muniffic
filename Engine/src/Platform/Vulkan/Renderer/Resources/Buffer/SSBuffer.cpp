#include "SSBuffer.h"
#include "Platform/Vulkan/Renderer/Resources/Scene/ObjectRenderData.h"
#include <glm/glm.hpp>
#include "VulkanRenderer.h"
bool eg::VulkanShaderStorageBuffer::addBasic2DObjectSRComponentData(ObjectRenderData* objectToAdd, void* colorNMatrixData)
{
	if ((objectToAdd->m_Update & RenderUpdate::Created) != RenderUpdate::None)
	{
		if (m_LastOffset + sizeof(ObjectMatrixData) <= m_Buffer.m_Size)
		{
			objectToAdd->m_MatricesBufferOffset = m_LastOffset;
			uintptr_t srcAddress = reinterpret_cast<uintptr_t>(m_Mapped);
			uintptr_t incrementedAddress = srcAddress + m_LastOffset;
			void* whereStart = reinterpret_cast<void*>(incrementedAddress);
			memcpy(whereStart, colorNMatrixData, sizeof(ObjectMatrixData));
			m_LastOffset += sizeof(ObjectMatrixData);
			objectToAdd->m_MatricesBuffer = this;
			m_InstancesCount++;
			return true;
		}
		else
		{
			EG_ASSERT(false, "For now every object should fit into one SSBO");// TO-DO Vulkan
		}
	}
	return false;
}

void eg::VulkanShaderStorageBuffer::create(size_t size)
{
	m_Buffer.Create(size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	VkResult res = vkMapMemory(VRen::get().getNativeDevice(), m_Buffer.m_Memory, 0, m_Buffer.m_Size, 0, &m_Mapped);
	EG_ASSERT(res == 0);
}

void eg::VulkanShaderStorageBuffer::destroy()
{
	vkUnmapMemory(VRen::get().getNativeDevice(), m_Buffer.m_Memory);
	m_Buffer.Destroy();
}
