#include "egpch.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanUtils.h"
#include "VulkanVertex.h"
#include "VulkanShader.h"

namespace eg {

	void VulkanGraphicsPipeline::Cleanup()
	{
		VulkanHandler* handler = GetVulkanHandler();
		VkDevice device = handler->GetVulkanLogicalDevice().GetDevice();
		vkDestroyPipelineLayout(GetVulkanHandler()->GetVulkanLogicalDevice().GetDevice(), m_PipelineLayout, nullptr);
		vkDestroyPipeline(GetVulkanHandler()->GetVulkanLogicalDevice().GetDevice(), m_GraphicsPipeline, nullptr);
	}

	void VulkanGraphicsPipeline::Bind(VkCommandBuffer commandBuffer)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
	}
}
