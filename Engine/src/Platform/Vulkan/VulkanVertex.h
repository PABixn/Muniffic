#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "Engine/Renderer/Buffer.h"
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

	class VulkanVertexInputLayout: BufferLayout{
	public:
		VkVertexInputBindingDescription GetBindingDescription();

		std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
	};

	class VulkanVertexBuffer {
	public:
		VulkanVertexBuffer(VkDevice logicalDevice, float* vertices, uint32_t size, uint32_t stride);
		~VulkanVertexBuffer();

		void Bind();
		void Unbind();

		VkBuffer GetBuffer() { return m_Buffer; }
		VkDeviceMemory GetBufferMemory() { return m_BufferMemory; }

	private:
		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;
		VkDevice m_LogicalDevice;
	};
}