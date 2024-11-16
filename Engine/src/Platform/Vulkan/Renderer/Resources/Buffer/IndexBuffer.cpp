#include "IndexBuffer.h"
#include "Platform/Vulkan/Renderer/Resources/Scene/ObjectRenderData.h"
#include "Platform/Vulkan/Renderer/Resources/Vertex/Vertex.h"
#include "VulkanRenderer.h"
bool eg::VulkanIndexBuffer::addBasic2DObjectIndices(ObjectRenderData* objectToAdd, void* indicesData)
{
	if ((objectToAdd->m_Update & RenderUpdate::Created) != RenderUpdate::None)
	{
		if (m_LastOffset + (objectToAdd->m_IndicesCount * sizeof(uint32_t)) <= m_Buffer.m_Size)
		{
			objectToAdd->m_IndexBufferOffset = m_LastOffset;
			uintptr_t srcAddress = reinterpret_cast<uintptr_t>(m_Mapped);
			uintptr_t incrementedAddress = srcAddress + m_LastOffset;
			void* whereStart = reinterpret_cast<void*>(incrementedAddress);
			memcpy(whereStart, indicesData, objectToAdd->m_IndicesCount * sizeof(uint32_t));
			m_LastOffset += objectToAdd->m_IndicesCount * sizeof(uint32_t);
			objectToAdd->m_IndexBuffer = this;
			m_IndicesCount += objectToAdd->m_IndicesCount;
			return true;
		}
		else
		{
			EG_ASSERT(false, "For now every object should fit into one index buffer");// TO-DO Vulkan
		}
	}
	return false;
}

void eg::VulkanIndexBuffer::create(size_t size)
{
	m_Buffer.Create(size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	VkResult res = vkMapMemory(VRen::get().getNativeDevice(), m_Buffer.m_Memory, 0, m_Buffer.m_Size, 0, &m_Mapped);
	EG_ASSERT(res == 0);
}

void eg::VulkanIndexBuffer::destroy()
{
	vkUnmapMemory(VRen::get().getNativeDevice(), m_Buffer.m_Memory);
	m_Buffer.Destroy();
}
