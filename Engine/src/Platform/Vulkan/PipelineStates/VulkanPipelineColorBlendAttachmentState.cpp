#include "egpch.h"
#include "VulkanPipelineColorBlendAttachmentState.h"

namespace eg {

	VulkanPipelineColorBlendAttachmentState::VulkanPipelineColorBlendAttachmentState()
	{
		m_CreateInfo.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		m_CreateInfo.blendEnable = VK_FALSE;
	}

	VulkanPipelineColorBlendAttachmentState::VulkanPipelineColorBlendAttachmentState(const VkPipelineColorBlendAttachmentState& state)
	{
		m_CreateInfo = state;
	}

	VulkanPipelineColorBlendAttachmentState::VulkanPipelineColorBlendAttachmentState(VkColorComponentFlags colorWriteMask, VkBool32 blendEnable)
	{
		m_CreateInfo.colorWriteMask = colorWriteMask;
		m_CreateInfo.blendEnable = blendEnable;
	}

	VulkanPipelineColorBlendAttachmentState::VulkanPipelineColorBlendAttachmentState(VkColorComponentFlags colorWriteMask, VkBool32 blendEnable, VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor, VkBlendOp colorBlendOp, VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor, VkBlendOp alphaBlendOp)
	{
		m_CreateInfo.colorWriteMask = colorWriteMask;
		m_CreateInfo.blendEnable = blendEnable;
		m_CreateInfo.srcColorBlendFactor = srcColorBlendFactor;
		m_CreateInfo.dstColorBlendFactor = dstColorBlendFactor;
		m_CreateInfo.colorBlendOp = colorBlendOp;
		m_CreateInfo.srcAlphaBlendFactor = srcAlphaBlendFactor;
		m_CreateInfo.dstAlphaBlendFactor = dstAlphaBlendFactor;
		m_CreateInfo.alphaBlendOp = alphaBlendOp;
	}

}
