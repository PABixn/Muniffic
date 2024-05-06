#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "Engine/Renderer/Buffer.h"
#include "VulkanBuffer.h"
#include <GLFW/glfw3.h>
namespace eg {
	VkFormat ShaderDataTypeToVulkanFormat(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return VK_FORMAT_R32_SFLOAT;
		case ShaderDataType::Float2:	return VK_FORMAT_R32G32_SFLOAT;
		case ShaderDataType::Float3:	return VK_FORMAT_R32G32B32_SFLOAT;
		case ShaderDataType::Float4:	return VK_FORMAT_R32G32B32A32_SFLOAT;
		case ShaderDataType::Mat3:		return VK_FORMAT_R32G32B32_SFLOAT;
		case ShaderDataType::Mat4:		return VK_FORMAT_R32G32B32A32_SFLOAT;
		case ShaderDataType::Int:		return VK_FORMAT_R32_SINT;
		case ShaderDataType::Int2:		return VK_FORMAT_R32G32_SINT;
		case ShaderDataType::Int3:		return VK_FORMAT_R32G32B32_SINT;
		case ShaderDataType::Int4:		return VK_FORMAT_R32G32B32A32_SINT;
		case ShaderDataType::Bool:		return VK_FORMAT_R8_SINT;
		}

		EG_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return VK_FORMAT_UNDEFINED;
	}

	class VulkanVertexInputLayout: public BufferLayout{
	public:
		VulkanVertexInputLayout() = default;
		VulkanVertexInputLayout(BufferLayout& layout) : BufferLayout(layout) {}
		VulkanVertexInputLayout(const BufferLayout& layout) : BufferLayout(layout) {}
		VkVertexInputBindingDescription& GetBindingDescription();

		std::vector<VkVertexInputAttributeDescription>& GetAttributeDescriptions();
	};

	class VulkanVertexBuffer : public VertexBuffer{
	public:
		VulkanVertexBuffer() = default;
		VulkanVertexBuffer(const VulkanBuffer& buffer) : m_VertexBuffer(buffer) {}
		VulkanVertexBuffer(const VulkanVertexBuffer& buffer) = default;
		VulkanVertexBuffer(const VulkanVertexBuffer& buffer) = default;
		VulkanVertexBuffer(const VulkanBuffer& buffer);
		~VulkanVertexBuffer() = default;

		// static Ref<VulkanVertexBuffer> Create(BufferLayout& layout, float* vertices, uint32_t count);
		void Cleanup(VkDevice logicalDevice);

		virtual void Bind() const override;

		virtual void SetLayout(const BufferLayout& layout) { m_VertexInputLayout = VulkanVertexInputLayout(layout); }
		virtual const BufferLayout& GetLayout() const { return m_VertexInputLayout; }

		virtual void SetData(void* data, uint32_t size);

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);
		VulkanBuffer& GetVulkanBuffer() { return m_VertexBuffer; }

		friend class VulkanBufferFactory;
	private:
		VulkanBuffer m_VertexBuffer;
		VulkanVertexInputLayout m_VertexInputLayout;
	};
}