#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Engine/Renderer/RendererAPI.h"
#include "VulkanCommandBuffer.h"
#include "VulkanShader.h"
#include "VulkanVertex.h"
#include "VulkanIndexBuffer.h"

namespace eg {
	class VulkanRendererAPI : public RendererAPI
	{
	public:
		VulkanRendererAPI() = default;
		static Ref<VulkanRendererAPI> Create();
		virtual void Clear() override;
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void DrawIndexed(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer, uint32_t indexCount = 0) override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
		virtual void SetLineThickness(float thickness) override;

		void SetCommandBuffer(VulkanCommandBuffer* commandBuffer) { m_CommandBuffer = commandBuffer; }
	private:
		VulkanCommandBuffer* m_CommandBuffer;
	};
}