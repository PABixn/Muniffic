#pragma once
#include "VulkanGraphicsPipeline.h"
#include "VulkanShader.h"
#include "VulkanLogicalDevice.h"

namespace eg {
	class VulkanGraphicsPipelineBuilder {
	public:
		VulkanGraphicsPipelineBuilder(VulkanLogicalDevice* device);
		~VulkanGraphicsPipelineBuilder();

		VulkanGraphicsPipelineBuilder& setShader(VulkanShader* shader);
		VulkanGraphicsPipelineBuilder& setInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo* inputAssemblyState);
		VulkanGraphicsPipelineBuilder& setViewportState(VkPipelineViewportStateCreateInfo* viewportState);
		VulkanGraphicsPipelineBuilder& setRasterizationState(VkPipelineRasterizationStateCreateInfo* rasterizationState);
		VulkanGraphicsPipelineBuilder& setMultisampleState(VkPipelineMultisampleStateCreateInfo* multisampleState);
		VulkanGraphicsPipelineBuilder& setDepthStencilState(VkPipelineDepthStencilStateCreateInfo* depthStencilState);
		VulkanGraphicsPipelineBuilder& setColorBlendState(VkPipelineColorBlendStateCreateInfo* colorBlendState);
		VulkanGraphicsPipelineBuilder& setDynamicState(VkPipelineDynamicStateCreateInfo* dynamicState);
		VulkanGraphicsPipelineBuilder& setRenderPass(VkRenderPass renderPass);
		VulkanGraphicsPipelineBuilder& setSubpass(uint32_t subpass);
		VulkanGraphicsPipelineBuilder& setLayout(VkPipelineLayout layout);
		VulkanGraphicsPipelineBuilder& setBasePipelineHandle(VkPipeline basePipelineHandle);
		VulkanGraphicsPipelineBuilder& setBasePipelineIndex(int32_t basePipelineIndex);

		VulkanGraphicsPipeline* build();

	private:
		VulkanLogicalDevice* device;
		VulkanShader* shader;
		VkPipelineVertexInputStateCreateInfo* vertexInputState;
		VkPipelineInputAssemblyStateCreateInfo* inputAssemblyState;
		VkPipelineViewportStateCreateInfo* viewportState;
		VkPipelineRasterizationStateCreateInfo* rasterizationState;
		VkPipelineMultisampleStateCreateInfo* multisampleState;
		VkPipelineDepthStencilStateCreateInfo* depthStencilState;
		VkPipelineColorBlendStateCreateInfo* colorBlendState;
		VkPipelineDynamicStateCreateInfo* dynamicState;
		VkRenderPass renderPass;
		uint32_t subpass;
		VkPipelineLayout layout;
		VkPipeline basePipelineHandle;
		int32_t basePipelineIndex;
	};
}