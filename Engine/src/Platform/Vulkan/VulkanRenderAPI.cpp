#include "egpch.h"
#include "VulkanRenderAPI.h"

namespace eg {
    Ref<VulkanRendererAPI> eg::VulkanRendererAPI::Create()
    {
        return CreateRef<VulkanRendererAPI>();
    }
    void VulkanRendererAPI::Clear()
    {
    }
    void VulkanRendererAPI::SetClearColor(const glm::vec4& color)
    {
    }
    void VulkanRendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
    }
    void VulkanRendererAPI::DrawIndexed(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer, uint32_t indexCount)
    {
        vkCmdBindVertexBuffers(m_CommandBuffer->getCommandBuffer(), 0, 1, &((VulkanVertexBuffer&)(*vertexBuffer)).GetVulkanBuffer().GetBuffer(), 0);
        vkCmdBindIndexBuffer(m_CommandBuffer->getCommandBuffer(), ((VulkanIndexBuffer&)(*indexBuffer)).getBuffer().GetBuffer(), 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(m_CommandBuffer->getCommandBuffer(), indexCount, 1, 0, 0, 0);
    }
    void VulkanRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
    {
    }
    void VulkanRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
    {
    }
    void VulkanRendererAPI::SetLineThickness(float thickness)
    {
    }
}
