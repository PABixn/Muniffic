#include "egpch.h"
#include "VulkanPipelineLayoutCreateInfo.h"

namespace eg {

	VulkanPipelineLayoutCreateInfo::VulkanPipelineLayoutCreateInfo()
	{
		m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	}

	VulkanPipelineLayoutCreateInfo::VulkanPipelineLayoutCreateInfo(const VkPipelineLayoutCreateInfo& info)
	{
		m_createInfo = info;
	}

	VulkanPipelineLayoutCreateInfo::VulkanPipelineLayoutCreateInfo(VkDescriptorSetLayout* setLayouts, uint32_t setLayoutCount)
	{
		m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		m_createInfo.setLayoutCount = setLayoutCount;
		m_createInfo.pSetLayouts = setLayouts;
	}

	VulkanPipelineLayoutCreateInfo::VulkanPipelineLayoutCreateInfo(VkPushConstantRange* pushConstantRanges, uint32_t pushConstantRangeCount)
	{
		m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		m_createInfo.pushConstantRangeCount = pushConstantRangeCount;
		m_createInfo.pPushConstantRanges = pushConstantRanges;
	}

	VulkanPipelineLayoutCreateInfo::VulkanPipelineLayoutCreateInfo(VkDescriptorSetLayout* setLayouts, uint32_t setLayoutCount, VkPushConstantRange* pushConstantRanges, uint32_t pushConstantRangeCount)
	{
		m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		m_createInfo.setLayoutCount = setLayoutCount;
		m_createInfo.pSetLayouts = setLayouts;
		m_createInfo.pushConstantRangeCount = pushConstantRangeCount;
		m_createInfo.pPushConstantRanges = pushConstantRanges;
	}

}
