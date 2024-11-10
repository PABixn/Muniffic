#include "VertexBuffer.h"
#include "Platform/Vulkan/Renderer/Resources/Scene/SceneRenderData.h"
#include "Platform/Vulkan/Renderer/Resources/Vertex/Vertex.h"
#include "VulkanRenderer.h"
bool eg::VulkanVertexBuffer::addBasic2DObjectVertices(ObjectRenderData* objectToAdd, void* verticesData)
{
	if (objectToAdd->m_Update == RenderUpdate_Created)
	{
		if (m_LastOffset + (objectToAdd->m_VerticesCount * sizeof(VulkanVertex)) <= m_Buffer.m_Size)
		{
			objectToAdd->m_VertexBufferOffset = m_LastOffset;
			uintptr_t srcAddress = reinterpret_cast<uintptr_t>(m_Mapped);
			uintptr_t incrementedAddress = srcAddress + m_LastOffset;
			void* whereStart = reinterpret_cast<void*>(incrementedAddress);
			memcpy(whereStart, verticesData, objectToAdd->m_VerticesCount * sizeof(VulkanVertex));
			m_LastOffset += objectToAdd->m_VerticesCount * sizeof(VulkanVertex);
			m_VerticesCount += objectToAdd->m_VerticesCount;
			objectToAdd->m_VertexBuffer = this;
			return true;
		}
		else
		{
			EG_ASSERT(false, "For now every object should fit into one vertex buffer");// TO-DO Vulkan
		}
	}
	return false;
}

void eg::VulkanVertexBuffer::create(size_t size)
{
	m_Buffer.Create(size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	VkResult res = vkMapMemory(VRen::get().getNativeDevice(), m_Buffer.m_Memory, 0, m_Buffer.m_Size, 0, &m_Mapped);
	EG_ASSERT(res == 0);
}

void eg::VulkanVertexBuffer::destroy()
{
	vkUnmapMemory(VRen::get().getNativeDevice(), m_Buffer.m_Memory);
	m_Buffer.Destroy();
}
