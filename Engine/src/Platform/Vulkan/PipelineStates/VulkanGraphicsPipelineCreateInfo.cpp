#include "egpch.h"
#include "VulkanGraphicsPipelineCreateInfo.h"
#include "VulkanPipelineColorBlendAttachmentState.h"
#include "VulkanPipelineDynamicStateCreateInfo.h"
#include "VulkanPipelineInputAssemblyStateCreateInfo.h"
#include "VulkanPipelineMultisampleStateCreateInfo.h"
#include "VulkanPipelineRasterizationStateCreateInfo.h"
#include "VulkanPipelineShaderStageCreateInfo.h"
#include "VulkanPipelineVertexInputStateCreateInfo.h"
#include "VulkanPipelineViewportStateCreateInfo.h"
#include "VulkanPipelineColorBlendCreateInfo.h"
#include "VulkanPipelineDepthStencilStateCreateInfo.h"
#include "VulkanPipelineTessellationStateCreateInfo.h"
namespace eg {
	VulkanGraphicsPipelineCreateInfo::VulkanGraphicsPipelineCreateInfo(bool setBaseInfo)
	{
// Set default values
		if(setBaseInfo)
			SetBaseInfo();
		else {
			m_CreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			m_CreateInfo.pNext = nullptr;
			m_CreateInfo.flags = 0;
			m_CreateInfo.stageCount = 0;
			m_CreateInfo.pStages = nullptr;
			m_CreateInfo.pVertexInputState = nullptr;
			m_CreateInfo.pInputAssemblyState = nullptr;
			m_CreateInfo.pTessellationState = nullptr;
			m_CreateInfo.pViewportState = nullptr;
			m_CreateInfo.pRasterizationState = nullptr;
			m_CreateInfo.pMultisampleState = nullptr;
			m_CreateInfo.pDepthStencilState = nullptr;
			m_CreateInfo.pColorBlendState = nullptr;
			m_CreateInfo.pDynamicState = nullptr;
			m_CreateInfo.layout = VK_NULL_HANDLE;
			m_CreateInfo.renderPass = VK_NULL_HANDLE;
			m_CreateInfo.subpass = 0;
			m_CreateInfo.basePipelineHandle = VK_NULL_HANDLE;
			m_CreateInfo.basePipelineIndex = -1;
		}
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

	void VulkanGraphicsPipelineCreateInfo::SetBaseInfo()
	{
		m_CreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		m_CreateInfo.pNext = nullptr;
		m_CreateInfo.flags = 0;
		m_CreateInfo.stageCount = 0;
		m_CreateInfo.pStages = nullptr;
		m_CreateInfo.pVertexInputState = nullptr;
		m_CreateInfo.pInputAssemblyState = VulkanPipelineInputAssemblyStateCreateInfo();
		m_CreateInfo.pTessellationState = VulkanPipelineTesselationStateCreateInfo();
		m_CreateInfo.pViewportState = VulkanPipelineViewportStateCreateInfo();
		m_CreateInfo.pRasterizationState = VulkanPipelineRasterizationStateCreateInfo();
		m_CreateInfo.pMultisampleState = VulkanPipelineMultisampleStateCreateInfo();
		m_CreateInfo.pDepthStencilState = VulkanPipelineDepthStencilStateCreateInfo();
		m_CreateInfo.pColorBlendState = VulkanPipelineColorBlendCreateInfo();
		m_CreateInfo.pDynamicState = VulkanPipelineDynamicStateCreateInfo();
		m_CreateInfo.layout = VK_NULL_HANDLE;
		m_CreateInfo.renderPass = VK_NULL_HANDLE;
		m_CreateInfo.subpass = 0;
		m_CreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		m_CreateInfo.basePipelineIndex = -1;
	}


}
