#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Renderer/Shader.h"
#include "Platform/Vulkan/VulkanShader.h"
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanPipelineVertexInputStateCreateInfo {
	public:
		VulkanPipelineVertexInputStateCreateInfo();
		VulkanPipelineVertexInputStateCreateInfo(const VkPipelineVertexInputStateCreateInfo& info);
		VulkanPipelineVertexInputStateCreateInfo(VulkanShader* shader);
		VulkanPipelineVertexInputStateCreateInfo(VulkanVertexInputLayout* layout);
		~VulkanPipelineVertexInputStateCreateInfo() = default;

		void addBinding(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate);
		void addAttribute(uint32_t binding, uint32_t location, VkFormat format, uint32_t offset);

		VkPipelineVertexInputStateCreateInfo* getCreateInfo() { return &m_createInfo; }

		operator VkPipelineVertexInputStateCreateInfo&() { return m_createInfo; }
		operator VkPipelineVertexInputStateCreateInfo () { return m_createInfo; }
		operator VkPipelineVertexInputStateCreateInfo*() { return &m_createInfo; }
		operator const VkPipelineVertexInputStateCreateInfo&() const { return m_createInfo; }
		operator const VkPipelineVertexInputStateCreateInfo*() const { return &m_createInfo; }
		operator const VkPipelineVertexInputStateCreateInfo() const { return m_createInfo; }
	private:
		VkPipelineVertexInputStateCreateInfo m_createInfo;
	};
}