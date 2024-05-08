#include "egpch.h"
#include "VulkanIndexBuffer.h"
#include "VulkanBufferFactory.h"

namespace eg {

	Ref<VulkanIndexBuffer> VulkanIndexBuffer::Create(uint32_t* vertices, uint32_t count)
	{
		return CreateRef<VulkanIndexBuffer>(VulkanBufferFactory::GetInstance()->CreateIndexBuffer(vertices, count), count);
	}

	void VulkanIndexBuffer::AddData(uint32_t* data, uint32_t size)
	{
		m_Indices.insert(m_Indices.end(), data, data + size);
		indexCount += size;
	}

	void VulkanIndexBuffer::SetData(void* data, uint32_t size)
	{
		m_Indexbuffer = VulkanBufferFactory::GetInstance()->CreateIndexBuffer((uint32_t*)data, size);
		indexCount = size;
	}

	void VulkanIndexBuffer::SetData()
	{
		m_Indexbuffer = VulkanBufferFactory::GetInstance()->CreateIndexBuffer(m_Indices.data(), m_Indices.size());
		indexCount = m_Indices.size();
	}

	void eg::VulkanIndexBuffer::Cleanup(VkDevice device)
	{
		m_Indexbuffer.Destroy(device);
	}
	void VulkanIndexBuffer::Bind() const
	{
		vkCmdBindIndexBuffer(m_CommandBuffer, m_Indexbuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
	}
}
