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
		VulkanVertexInputLayout(BufferLayout& layout) : BufferLayout(layout) {}
		VkVertexInputBindingDescription& GetBindingDescription();

		std::vector<VkVertexInputAttributeDescription>& GetAttributeDescriptions();
	};

	class VulkanVertexBuffer {
	public:
		VulkanVertexBuffer() = default;
		~VulkanVertexBuffer() = default;

		void Create(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, BufferLayout& layout, float* vertices, uint32_t count);
		void Cleanup(VkDevice logicalDevice);

		void Bind();
		void Unbind();

		VulkanBuffer& GetVulkanBuffer() { return m_VertexBuffer; }
	private:
		VulkanBuffer m_VertexBuffer;
		VulkanVertexInputLayout m_VertexInputLayout;
	};
}