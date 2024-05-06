#include "egpch.h"
#include "VulkanIndexBuffer.h"
#include "VulkanBufferFactory.h"

namespace eg {

	Ref<VulkanIndexBuffer> VulkanIndexBuffer::Create(uint32_t* vertices, uint32_t count)
	{
		return CreateRef<VulkanIndexBuffer>(VulkanBufferFactory::GetInstance()->CreateIndexBuffer(vertices, count));
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
