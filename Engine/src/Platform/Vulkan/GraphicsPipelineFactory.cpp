#include "egpch.h"
#include "GraphicsPipelineFactory.h"
#include "PipelineStates/VulkanPipelineDynamicStateCreateInfo.h"
#include "PipelineStates/VulkanPipelineInputAssemblyStateCreateInfo.h"
#include "PipelineStates/VulkanPipelineMultisampleStateCreateInfo.h"
#include "PipelineStates/VulkanPipelineRasterizationStateCreateInfo.h"
#include "PipelineStates/VulkanPipelineVertexInputStateCreateInfo.h"
#include "PipelineStates/VulkanPipelineViewportStateCreateInfo.h"
#include "PipelineStates/VulkanPipelineColorBlendCreateInfo.h"
#include "PipelineStates/VulkanPipelineDepthStencilStateCreateInfo.h"
#include "PipelineStates/VulkanPipelineShaderStageCreateInfo.h"
#include "PipelineStates/VulkanPipelineLayoutCreateInfo.h"
namespace eg {

	eg::VulkanGraphicsPipelineBuilder::VulkanGraphicsPipelineBuilder(VulkanLogicalDevice* device, VkRenderPass renderPass)
	{
		m_Device = device;
		m_RenderPass = renderPass;
	}

	VulkanGraphicsPipelineBuilder& VulkanGraphicsPipelineBuilder::SetBaseInfo()
	{
		m_InputAssemblyState = VulkanPipelineInputAssemblyStateCreateInfo();
		m_RasterizationState = VulkanPipelineRasterizationStateCreateInfo();
		m_ColorBlendState = VulkanPipelineColorBlendCreateInfo();
		m_MultisampleState = VulkanPipelineMultisampleStateCreateInfo();
		m_ViewportState = VulkanPipelineViewportStateCreateInfo();
		m_DynamicState = VulkanPipelineDynamicStateCreateInfo();
		m_DepthStencilState = VulkanPipelineDepthStencilStateCreateInfo();
		return *this;
	}

	VulkanGraphicsPipelineBuilder& VulkanGraphicsPipelineBuilder::setShader(VulkanShader* shader)
	{
		m_Shader = shader; 
		m_VertexInputState = VulkanPipelineVertexInputStateCreateInfo(m_Shader);
		m_DescriptorSetLayouts->push_back(m_Shader->GetDescriptorSetLayout().GetDescriptorSetLayout());
		return *this; 
	}

	Ref<VulkanGraphicsPipeline> VulkanGraphicsPipelineBuilder::build()
	{
		VulkanPipelineLayoutCreateInfo layoutInfo = { m_DescriptorSetLayouts->size() > 0 ?m_DescriptorSetLayouts->data() : nullptr, m_DescriptorSetLayouts->size(),m_PushConstantRanges->size() > 0 ? m_PushConstantRanges->data() : nullptr, m_PushConstantRanges->size() };
		VkPipeline pipeline;
		VkPipelineLayout layout;
		if (vkCreatePipelineLayout(m_Device->GetDevice(), layoutInfo, nullptr, &layout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create graphicsPipeline layout!");
		}

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		std::vector<VulkanPipelineShaderStageCreateInfo> shaderStages;
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = m_Shader->GetStageCount();
		pipelineInfo.pStages = m_Shader->GetShaderStages().data();
		pipelineInfo.pVertexInputState = m_VertexInputState;
		pipelineInfo.pInputAssemblyState = m_InputAssemblyState;
		pipelineInfo.pViewportState = m_ViewportState;
		pipelineInfo.pRasterizationState = m_RasterizationState;
		pipelineInfo.pMultisampleState = m_MultisampleState;
		pipelineInfo.pDepthStencilState = m_DepthStencilState;
		pipelineInfo.pColorBlendState = m_ColorBlendState;
		pipelineInfo.pDynamicState = m_DynamicState;
		pipelineInfo.layout = layout;
		pipelineInfo.renderPass = m_RenderPass;
		pipelineInfo.subpass = m_Subpass;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineInfo.basePipelineIndex = -1;
		if (vkCreateGraphicsPipelines(m_Device->GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create graphicsPipeline!");
		}
		Ref<VulkanGraphicsPipeline> vulkanPipeline = CreateRef<VulkanGraphicsPipeline>(layout, pipeline, m_Device->GetDevice());
		reset();
		return vulkanPipeline;
	}

	void VulkanGraphicsPipelineBuilder::reset()
	{
		m_Shader = nullptr;
		m_VertexInputState = nullptr;
		m_InputAssemblyState = nullptr;
		m_RasterizationState = nullptr;
		m_ColorBlendState = nullptr;
		m_MultisampleState = nullptr;
		m_ViewportState = nullptr;
		m_DynamicState = nullptr;
		m_DepthStencilState = nullptr;
		m_RenderPass = VK_NULL_HANDLE;
		m_Subpass = 0;
		m_DescriptorSetLayouts = nullptr;
		m_PushConstantRanges = nullptr;
		m_Layout = VK_NULL_HANDLE;
		m_BasePipelineHandle = VK_NULL_HANDLE;
		m_BasePipelineIndex = -1;
	}

}
