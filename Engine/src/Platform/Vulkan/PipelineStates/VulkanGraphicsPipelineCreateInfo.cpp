#include "egpch.h"
#include "VulkanGraphicsPipelineCreateInfo.h"

namespace eg {
	VulkanGraphicsPipelineCreateInfo::VulkanGraphicsPipelineCreateInfo()
	{
// Set default values
		
	}

	VulkanGraphicsPipelineCreateInfo::VulkanGraphicsPipelineCreateInfo(VkPipelineCreateFlags flags, uint32_t stageCount, VkPipelineShaderStageCreateInfo* pStages, VkPipelineVertexInputStateCreateInfo* pVertexInputState, VkPipelineInputAssemblyStateCreateInfo* pInputAssemblyState, VkPipelineTessellationStateCreateInfo* pTessellationState, VkPipelineViewportStateCreateInfo* pViewportState, VkPipelineRasterizationStateCreateInfo* pRasterizationState, VkPipelineMultisampleStateCreateInfo* pMultisampleState, VkPipelineDepthStencilStateCreateInfo* pDepthStencilState, VkPipelineColorBlendStateCreateInfo* pColorBlendState, VkPipelineDynamicStateCreateInfo* pDynamicState, VkPipelineLayout layout, VkRenderPass renderPass, uint32_t subpass, VkPipeline)
	{
		m_CreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		m_CreateInfo.pNext = nullptr;
		m_CreateInfo.flags = flags;
		m_CreateInfo.stageCount = stageCount;
		m_CreateInfo.pStages = pStages;
		m_CreateInfo.pVertexInputState = pVertexInputState;
		m_CreateInfo.pInputAssemblyState = pInputAssemblyState;
		m_CreateInfo.pTessellationState = pTessellationState;
		m_CreateInfo.pViewportState = pViewportState;
		m_CreateInfo.pRasterizationState = pRasterizationState;
		m_CreateInfo.pMultisampleState = pMultisampleState;
		m_CreateInfo.pDepthStencilState = pDepthStencilState;
		m_CreateInfo.pColorBlendState = pColorBlendState;
		m_CreateInfo.pDynamicState = pDynamicState;
		m_CreateInfo.layout = layout;
		m_CreateInfo.renderPass = renderPass;
		m_CreateInfo.subpass = subpass;
		m_CreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		m_CreateInfo.basePipelineIndex = -1;
	}

	VulkanGraphicsPipelineCreateInfo::VulkanGraphicsPipelineCreateInfo(const VkGraphicsPipelineCreateInfo& info)
	{
		m_CreateInfo = info;
	}


}
