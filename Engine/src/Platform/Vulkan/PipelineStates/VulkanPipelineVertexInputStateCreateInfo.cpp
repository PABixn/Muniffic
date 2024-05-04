#include "egpch.h"
#include "VulkanPipelineVertexInputStateCreateInfo.h"

namespace eg {

	VulkanPipelineVertexInputStateCreateInfo::VulkanPipelineVertexInputStateCreateInfo()
	{
		m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	}

	VulkanPipelineVertexInputStateCreateInfo::VulkanPipelineVertexInputStateCreateInfo(const VkPipelineVertexInputStateCreateInfo& info)
	{
		m_createInfo = info;
	}

	VulkanPipelineVertexInputStateCreateInfo::VulkanPipelineVertexInputStateCreateInfo(VulkanShader* shader)
	{
		m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		auto& bindingDescription = shader->GetLayout().GetBindingDescription();
		auto& attributeDescriptions = shader->GetLayout().GetAttributeDescriptions();
		m_createInfo.vertexBindingDescriptionCount = 1;
		m_createInfo.pVertexBindingDescriptions = &bindingDescription;
		m_createInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
		m_createInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
	}

	VulkanPipelineVertexInputStateCreateInfo::VulkanPipelineVertexInputStateCreateInfo(VulkanVertexInputLayout* layout)
	{
		m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		auto& bindingDescription = layout->GetBindingDescription();
		auto& attributeDescriptions = layout->GetAttributeDescriptions();
		m_createInfo.vertexBindingDescriptionCount = 1;
		m_createInfo.pVertexBindingDescriptions = &bindingDescription;
		m_createInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
		m_createInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
	}

	void VulkanPipelineVertexInputStateCreateInfo::addBinding(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate)
	{
		//TODO: Implement
	}

	void VulkanPipelineVertexInputStateCreateInfo::addAttribute(uint32_t binding, uint32_t location, VkFormat format, uint32_t offset)
	{
		//TODO: Implement
	}
}
