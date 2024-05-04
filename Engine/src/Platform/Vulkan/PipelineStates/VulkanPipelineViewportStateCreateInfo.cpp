#include "egpch.h"
#include "VulkanPipelineViewportStateCreateInfo.h"

namespace eg {

	VulkanPipelineViewportStateCreateInfo::VulkanPipelineViewportStateCreateInfo()
	{
		m_CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		m_CreateInfo.viewportCount = 1;
		m_CreateInfo.scissorCount = 1;
	}

	VulkanPipelineViewportStateCreateInfo::VulkanPipelineViewportStateCreateInfo(const VkPipelineViewportStateCreateInfo& info)
	{
		m_CreateInfo = info;
	}

	VulkanPipelineViewportStateCreateInfo::VulkanPipelineViewportStateCreateInfo(uint32_t viewportCount, uint32_t scissorCount, VkViewport* viewports, VkRect2D* scissors)
	{
		m_CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		m_CreateInfo.viewportCount = viewportCount;
		m_CreateInfo.scissorCount = scissorCount;
		m_CreateInfo.pViewports = viewports;
		m_CreateInfo.pScissors = scissors;
		VulkanPipelineViewportStateCreateInfo inf;
		(VkPipelineViewportStateCreateInfo)inf;
	}

}
