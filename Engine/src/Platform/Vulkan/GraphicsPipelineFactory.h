#pragma once
#include "Engine/Core/Core.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanShader.h"
#include "VulkanLogicalDevice.h"

namespace eg {
	class VulkanGraphicsPipelineBuilder {
	public:
		VulkanGraphicsPipelineBuilder(VulkanLogicalDevice* device, VkRenderPass renderPass);
		~VulkanGraphicsPipelineBuilder() = default;

		VulkanGraphicsPipelineBuilder& SetBaseInfo();
		VulkanGraphicsPipelineBuilder& setShader(VulkanShader* shader);
		VulkanGraphicsPipelineBuilder& setInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo* inputAssemblyState) { m_InputAssemblyState = inputAssemblyState; return *this; }
		VulkanGraphicsPipelineBuilder& setViewportState(VkPipelineViewportStateCreateInfo* viewportState) { m_ViewportState = viewportState; return *this; }
		VulkanGraphicsPipelineBuilder& setRasterizationState(VkPipelineRasterizationStateCreateInfo* rasterizationState) { m_RasterizationState = rasterizationState; return *this; }
		VulkanGraphicsPipelineBuilder& setMultisampleState(VkPipelineMultisampleStateCreateInfo* multisampleState) { m_MultisampleState = multisampleState; return *this; }
		VulkanGraphicsPipelineBuilder& setDepthStencilState(VkPipelineDepthStencilStateCreateInfo* depthStencilState) { m_DepthStencilState = depthStencilState; return *this; }
		VulkanGraphicsPipelineBuilder& setColorBlendState(VkPipelineColorBlendStateCreateInfo* colorBlendState) { m_ColorBlendState = colorBlendState; return *this; }
		VulkanGraphicsPipelineBuilder& setDynamicState(VkPipelineDynamicStateCreateInfo* dynamicState) { m_DynamicState = dynamicState; return *this; }
		VulkanGraphicsPipelineBuilder& setDescriptorSetLayouts(std::vector<VkDescriptorSetLayout>* descriptorSetLayouts) { m_DescriptorSetLayouts = descriptorSetLayouts; return *this; }
		VulkanGraphicsPipelineBuilder& setPushConstantRanges(std::vector<VkPushConstantRange>* pushConstantRanges) { m_PushConstantRanges = pushConstantRanges; return *this; }
		VulkanGraphicsPipelineBuilder& setRenderPass(VkRenderPass renderPass) { m_RenderPass = renderPass; return *this; }
		VulkanGraphicsPipelineBuilder& setSubpass(uint32_t subpass) { m_Subpass = subpass; return *this; }
		VulkanGraphicsPipelineBuilder& setLayout(VkPipelineLayout layout) { m_Layout = layout; return *this; }
		VulkanGraphicsPipelineBuilder& setBasePipelineHandle(VkPipeline basePipelineHandle) { m_BasePipelineHandle = basePipelineHandle; return *this; }
		VulkanGraphicsPipelineBuilder& setBasePipelineIndex(int32_t basePipelineIndex) { m_BasePipelineIndex = basePipelineIndex; return *this; }

		Ref<VulkanGraphicsPipeline> build();
		void reset();
	private:
		VulkanLogicalDevice* m_Device;
		VulkanShader* m_Shader;
		VkPipelineVertexInputStateCreateInfo* m_VertexInputState;
		VkPipelineInputAssemblyStateCreateInfo* m_InputAssemblyState;
		VkPipelineViewportStateCreateInfo* m_ViewportState;
		VkPipelineRasterizationStateCreateInfo* m_RasterizationState;
		VkPipelineMultisampleStateCreateInfo* m_MultisampleState;
		VkPipelineDepthStencilStateCreateInfo* m_DepthStencilState;
		VkPipelineColorBlendStateCreateInfo* m_ColorBlendState;
		VkPipelineDynamicStateCreateInfo* m_DynamicState;
		std::vector<VkDescriptorSetLayout>* m_DescriptorSetLayouts;
		std::vector<VkPushConstantRange>* m_PushConstantRanges;
		VkRenderPass m_RenderPass;
		uint32_t m_Subpass;
		VkPipelineLayout m_Layout;
		VkPipeline m_BasePipelineHandle;
		int32_t m_BasePipelineIndex;
	};
}