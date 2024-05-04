#include "egpch.h"
#include "VulkanPipelineDynamicStateCreateInfo.h"

namespace eg {

	VulkanPipelineDynamicStateCreateInfo::VulkanPipelineDynamicStateCreateInfo()
	{
		m_CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		m_CreateInfo.pNext = nullptr;
		m_CreateInfo.flags = 0;
		m_CreateInfo.dynamicStateCount = 0;
		m_CreateInfo.pDynamicStates = nullptr;
	}

	VulkanPipelineDynamicStateCreateInfo::VulkanPipelineDynamicStateCreateInfo(const VkPipelineDynamicStateCreateInfo& info)
	{
		m_CreateInfo = info;
		m_DynamicStates = std::vector<VkDynamicState>(info.pDynamicStates, info.pDynamicStates + info.dynamicStateCount);
	}

	VulkanPipelineDynamicStateCreateInfo::VulkanPipelineDynamicStateCreateInfo(const std::vector<VkDynamicState>& dynamicStates)
	{
		m_CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		m_CreateInfo.pNext = nullptr;
		m_CreateInfo.flags = 0;
		m_CreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		m_CreateInfo.pDynamicStates = dynamicStates.data();
		m_DynamicStates = dynamicStates;
	}

}
