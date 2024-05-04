#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Renderer/Shader.h"
#include "Platform/Vulkan/VulkanShader.h"
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanPipelineShaderStageCreateInfo {
	public:
		VulkanPipelineShaderStageCreateInfo();
		VulkanPipelineShaderStageCreateInfo(const VkPipelineShaderStageCreateInfo& info);
		VulkanPipelineShaderStageCreateInfo(ShaderType stage, VkShaderModule module, const char* name = "main");
		VulkanPipelineShaderStageCreateInfo(VulkanShader* shader, ShaderType stage, const char* name = "main");
		~VulkanPipelineShaderStageCreateInfo() = default;

		VkPipelineShaderStageCreateInfo* getCreateInfo() { return &m_CreateInfo; }

		void setStage(VkShaderStageFlagBits stage) { m_CreateInfo.stage = stage; }
		void setModule(VkShaderModule module) { m_CreateInfo.module = module; }
		void setName(const char* name) { m_CreateInfo.pName = name; }

		operator VkPipelineShaderStageCreateInfo& () { return m_CreateInfo; }
		operator VkPipelineShaderStageCreateInfo () { return m_CreateInfo; }
		operator VkPipelineShaderStageCreateInfo* () { return &m_CreateInfo; }
		operator const VkPipelineShaderStageCreateInfo& () const { return m_CreateInfo; }
		operator const VkPipelineShaderStageCreateInfo* () const { return &m_CreateInfo; }
		operator const VkPipelineShaderStageCreateInfo () const { return m_CreateInfo; }

	private:
		VkPipelineShaderStageCreateInfo m_CreateInfo;
	};
}