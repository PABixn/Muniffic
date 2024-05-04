#include "egpch.h"
#include "VulkanPipelineRasterizationStateCreateInfo.h"

namespace eg {

	VulkanPipelineRasterizationStateCreateInfo::VulkanPipelineRasterizationStateCreateInfo()
	{
		m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		m_createInfo.depthClampEnable = VK_FALSE;
		m_createInfo.rasterizerDiscardEnable = VK_FALSE;
		m_createInfo.polygonMode = VK_POLYGON_MODE_FILL;
		m_createInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		m_createInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		m_createInfo.depthBiasEnable = VK_FALSE;
		m_createInfo.depthBiasConstantFactor = 0.0f;
		m_createInfo.depthBiasClamp = 0.0f;
		m_createInfo.depthBiasSlopeFactor = 0.0f;
		m_createInfo.lineWidth = 1.0f;
	}

	VulkanPipelineRasterizationStateCreateInfo::VulkanPipelineRasterizationStateCreateInfo(const VkPipelineRasterizationStateCreateInfo& info)
	{
		m_createInfo = info;
	}

	VulkanPipelineRasterizationStateCreateInfo::VulkanPipelineRasterizationStateCreateInfo(VkBool32 depthClampEnable, VkBool32 rasterizerDiscardEnable, VkPolygonMode polygonMode, VkCullModeFlags cullMode, VkFrontFace frontFace, VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth)
	{
		m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		m_createInfo.depthClampEnable = depthClampEnable;
		m_createInfo.rasterizerDiscardEnable = rasterizerDiscardEnable;
		m_createInfo.polygonMode = polygonMode;
		m_createInfo.cullMode = cullMode;
		m_createInfo.frontFace = frontFace;
		m_createInfo.depthBiasEnable = depthBiasEnable;
		m_createInfo.depthBiasConstantFactor = depthBiasConstantFactor;
		m_createInfo.depthBiasClamp = depthBiasClamp;
		m_createInfo.depthBiasSlopeFactor = depthBiasSlopeFactor;
		m_createInfo.lineWidth = lineWidth;
	}

}
