#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "../VulkanShader.h"
#include "../VulkanIndexBuffer.h"
#include "../VulkanVertex.h"
#include "../VulkanUniformBuffer.h"
#include "Engine/Renderer/GeometrySystem.h"
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanGeometry : public Geometry {
	public:
		VulkanGeometry() = default;
		VulkanGeometry(size_t vertexBufferSize, BufferLayout& layout, size_t indexBufferSize, std::string shaderPath);
		~VulkanGeometry() = default;

		static Ref<VulkanGeometry> Create(size_t vertexBufferSize, BufferLayout& layout, size_t indexBufferSize, std::string shaderPath);
	private:
		Ref<VulkanShader> m_Shader;
		Ref<VulkanIndexBuffer> m_IndexBuffer;
		Ref<VulkanVertexBuffer> m_VertexBuffer;
		Ref<VulkanUniformBuffer> m_UniformBuffer;
		VkCommandBuffer m_CommandBuffer;
	};
}