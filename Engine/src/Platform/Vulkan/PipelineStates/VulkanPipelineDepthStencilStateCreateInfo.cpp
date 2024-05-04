#include "egpch.h"
#include "VulkanPipelineDepthStencilStateCreateInfo.h"

namespace eg {

	VulkanPipelineDepthStencilStateCreateInfo::VulkanPipelineDepthStencilStateCreateInfo()
	{
		m_CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		m_CreateInfo.pNext = nullptr;
		m_CreateInfo.flags = 0;
		m_CreateInfo.depthTestEnable = VK_TRUE;
		m_CreateInfo.depthWriteEnable = VK_TRUE;
		m_CreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		m_CreateInfo.depthBoundsTestEnable = VK_FALSE;
		m_CreateInfo.stencilTestEnable = VK_FALSE;
		m_CreateInfo.front = {};
		m_CreateInfo.back = {};
		m_CreateInfo.minDepthBounds = 0.0f;
		m_CreateInfo.maxDepthBounds = 1.0f;
	}

	VulkanPipelineDepthStencilStateCreateInfo::VulkanPipelineDepthStencilStateCreateInfo(const VkPipelineDepthStencilStateCreateInfo& info)
	{
		m_CreateInfo = info;
	}

	VulkanPipelineDepthStencilStateCreateInfo::VulkanPipelineDepthStencilStateCreateInfo(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, VkCompareOp depthCompareOp, VkBool32 depthBoundsTestEnable, VkBool32 stencilTestEnable, VkStencilOpState front, VkStencilOpState back, float minDepthBounds, float maxDepthBounds)
	{
		m_CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		m_CreateInfo.pNext = nullptr;
		m_CreateInfo.flags = 0;
		m_CreateInfo.depthTestEnable = depthTestEnable;
		m_CreateInfo.depthWriteEnable = depthWriteEnable;
		m_CreateInfo.depthCompareOp = depthCompareOp;
		m_CreateInfo.depthBoundsTestEnable = depthBoundsTestEnable;
		m_CreateInfo.stencilTestEnable = stencilTestEnable;
		m_CreateInfo.front = front;
		m_CreateInfo.back = back;
		m_CreateInfo.minDepthBounds = minDepthBounds;
		m_CreateInfo.maxDepthBounds = maxDepthBounds;
	}

}
