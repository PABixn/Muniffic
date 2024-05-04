#include "egpch.h"
#include "VulkanPipelineColorBlendCreateInfo.h"

namespace eg {
	VulkanPipelineColorBlendCreateInfo::VulkanPipelineColorBlendCreateInfo()
	{
		m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		m_createInfo.logicOpEnable = VK_FALSE;
		m_createInfo.logicOp = VK_LOGIC_OP_COPY;
		m_createInfo.attachmentCount = 0;
		m_createInfo.pAttachments = VulkanPipelineColorBlendAttachmentState();
		m_createInfo.blendConstants[0] = 0.0f;
		m_createInfo.blendConstants[1] = 0.0f;
		m_createInfo.blendConstants[2] = 0.0f;
		m_createInfo.blendConstants[3] = 0.0f;
	}

	VulkanPipelineColorBlendCreateInfo::VulkanPipelineColorBlendCreateInfo(const VkPipelineColorBlendStateCreateInfo& info)
	{
		m_createInfo = info;
	}

	VulkanPipelineColorBlendCreateInfo::VulkanPipelineColorBlendCreateInfo(const VulkanPipelineColorBlendAttachmentState& attachmentState)
	{
		m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		m_createInfo.logicOpEnable = VK_FALSE;
		m_createInfo.logicOp = VK_LOGIC_OP_COPY;
		m_createInfo.attachmentCount = 1;
		m_createInfo.pAttachments = attachmentState;
		m_createInfo.blendConstants[0] = 0.0f;
		m_createInfo.blendConstants[1] = 0.0f;
		m_createInfo.blendConstants[2] = 0.0f;
		m_createInfo.blendConstants[3] = 0.0f;
	}
	VulkanPipelineColorBlendCreateInfo::VulkanPipelineColorBlendCreateInfo(const VulkanPipelineColorBlendAttachmentState& attachmentState, VkBool32 logicOpEnable, VkLogicOp logicOp, float blendConstants[4])
	{
		m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		m_createInfo.logicOpEnable = logicOpEnable;
		m_createInfo.logicOp = logicOp;
		m_createInfo.attachmentCount = 1;
		m_createInfo.pAttachments = attachmentState;
		m_createInfo.blendConstants[0] = blendConstants[0];
		m_createInfo.blendConstants[1] = blendConstants[1];
		m_createInfo.blendConstants[2] = blendConstants[2];
		m_createInfo.blendConstants[3] = blendConstants[3];
	}
}
