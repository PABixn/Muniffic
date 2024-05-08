#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Renderer/RendererAPI.h"
#include "VulkanCommandBuffer.h"
#include <GLFW/glfw3.h>
namespace eg {
	class VulkanRendererAPI : public RendererAPI
	{
	public:
		VulkanRendererAPI() = default;
		static Ref<VulkanRendererAPI> Create();
		void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer,const Ref<Shader>& shader) override;
		virtual void DrawIndexed(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer, uint32_t indexCount = 0) override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
		virtual void SetLineThickness(float thickness) override;

		void SetCommandBuffer(VulkanCommandBuffer* commandBuffer) { m_CommandBuffer = commandBuffer; }
	private:
		VulkanCommandBuffer* m_CommandBuffer;
	};
}