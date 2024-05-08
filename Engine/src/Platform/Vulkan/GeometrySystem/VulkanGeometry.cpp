#include "egpch.h"
#include "VulkanGeometry.h"
#include "Platform/Vulkan/VulkanBufferFactory.h"
namespace eg {
	VulkanGeometry::VulkanGeometry(size_t vertexBufferSize, BufferLayout& layout, size_t indexBufferSize, std::string shaderPath)
	{
		m_Shader = CreateRef<VulkanShader>(shaderPath, layout);
		m_VertexBuffer = CreateRef<VulkanVertexBuffer>(VulkanBufferFactory::GetInstance()->CraeteVertexBuffer(vertexBufferSize, layout));
		m_IndexBuffer = CreateRef<VulkanIndexBuffer>(VulkanBufferFactory::GetInstance()->CreateIndexBuffer(indexBufferSize));
		m_UniformBuffer = VulkanUniformBuffer::Create(sizeof(UniformBufferObject));
	}
	Ref<VulkanGeometry> VulkanGeometry::Create(size_t vertexBufferSize, BufferLayout& layout, size_t indexBufferSize, std::string shaderPath)
	{
		return CreateRef<VulkanGeometry>(vertexBufferSize, layout, indexBufferSize, shaderPath);
	}
	void VulkanGeometry::Bind() const
	{
		VkBuffer vertexBuffers[] = { m_VertexBuffer.GetBuffer().GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_CommandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(m_CommandBuffer, m_IndexBuffer.GetBuffer().GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
		vkCmdBindPipeline(m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Shader.GetGraphicsPipeline()->GetPipeline());
	}
	void eg::VulkanGeometry::Bind(VkCommandBuffer commandBuffer)
	{
		SetCommandBuffer(commandBuffer);
		Bind();
	}
}
