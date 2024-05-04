#include "egpch.h"
#include "VulkanPipelineTessellationStateCreateInfo.h"

namespace eg {

	VulkanPipelineTesselationStateCreateInfo::VulkanPipelineTesselationStateCreateInfo()
	{
		m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		m_createInfo.pNext = nullptr;
		m_createInfo.flags = 0;
		m_createInfo.patchControlPoints = 3;
	}

	VulkanPipelineTesselationStateCreateInfo::VulkanPipelineTesselationStateCreateInfo(VkPipelineTessellationStateCreateFlags flags, uint32_t patchControlPoints)
	{
		m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
		m_createInfo.pNext = nullptr;
		m_createInfo.flags = flags;
		m_createInfo.patchControlPoints = patchControlPoints;
	}

}
