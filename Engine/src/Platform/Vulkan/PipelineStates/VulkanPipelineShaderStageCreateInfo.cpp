#include "egpch.h"
#include "VulkanPipelineShaderStageCreateInfo.h"

namespace eg {

	VulkanPipelineShaderStageCreateInfo::VulkanPipelineShaderStageCreateInfo()
	{
		m_CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_CreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		m_CreateInfo.module = VK_NULL_HANDLE;
		m_CreateInfo.pName = "main";
		m_CreateInfo.pSpecializationInfo = nullptr;
	}

	VulkanPipelineShaderStageCreateInfo::VulkanPipelineShaderStageCreateInfo(const VkPipelineShaderStageCreateInfo& info)
	{
		m_CreateInfo = info;
	}

	VulkanPipelineShaderStageCreateInfo::VulkanPipelineShaderStageCreateInfo(ShaderType stage, VkShaderModule module, const char* name)
	{
		m_CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_CreateInfo.stage = static_cast<VkShaderStageFlagBits>(stage);
		m_CreateInfo.module = module;
		m_CreateInfo.pName = name;
	}

	VulkanPipelineShaderStageCreateInfo::VulkanPipelineShaderStageCreateInfo(VulkanShader* shader, ShaderType stage, const char* name)
	{
		m_CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		m_CreateInfo.stage = static_cast<VkShaderStageFlagBits>(stage);
		m_CreateInfo.module = shader->GetShaderModule(stage);
		m_CreateInfo.pName = name;
		m_CreateInfo.pSpecializationInfo = nullptr;
	}

}
