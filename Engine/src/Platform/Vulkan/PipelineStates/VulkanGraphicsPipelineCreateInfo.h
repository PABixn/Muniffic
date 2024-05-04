#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanGraphicsPipelineCreateInfo {
	public:
		VulkanGraphicsPipelineCreateInfo(bool setBaseInfo = true);
		VulkanGraphicsPipelineCreateInfo(VkPipelineCreateFlags flags, uint32_t stageCount, VkPipelineShaderStageCreateInfo* pStages, VkPipelineVertexInputStateCreateInfo* pVertexInputState, VkPipelineInputAssemblyStateCreateInfo* pInputAssemblyState, VkPipelineTessellationStateCreateInfo* pTessellationState, VkPipelineViewportStateCreateInfo* pViewportState, VkPipelineRasterizationStateCreateInfo* pRasterizationState, VkPipelineMultisampleStateCreateInfo* pMultisampleState, VkPipelineDepthStencilStateCreateInfo* pDepthStencilState, VkPipelineColorBlendStateCreateInfo* pColorBlendState, VkPipelineDynamicStateCreateInfo* pDynamicState, VkPipelineLayout layout, VkRenderPass renderPass, uint32_t subpass, VkPipeline);
		VulkanGraphicsPipelineCreateInfo(const VkGraphicsPipelineCreateInfo& info);
		~VulkanGraphicsPipelineCreateInfo() = default;

		void SetBaseInfo();

		VkGraphicsPipelineCreateInfo* GetCreateInfo() { return &m_CreateInfo; }

		void SetFlags(VkPipelineCreateFlags flags) { m_CreateInfo.flags = flags; }
		void SetStageCount(uint32_t stageCount) { m_CreateInfo.stageCount = stageCount; }
		void SetStages(VkPipelineShaderStageCreateInfo* pStages) { m_CreateInfo.pStages = pStages; }
		void SetVertexInputState(VkPipelineVertexInputStateCreateInfo* pVertexInputState) { m_CreateInfo.pVertexInputState = pVertexInputState; }
		void SetInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo* pInputAssemblyState) { m_CreateInfo.pInputAssemblyState = pInputAssemblyState; }
		void SetTessellationState(VkPipelineTessellationStateCreateInfo* pTessellationState) { m_CreateInfo.pTessellationState = pTessellationState; }
		void SetViewportState(VkPipelineViewportStateCreateInfo* pViewportState) { m_CreateInfo.pViewportState = pViewportState; }
		void SetRasterizationState(VkPipelineRasterizationStateCreateInfo* pRasterizationState) { m_CreateInfo.pRasterizationState = pRasterizationState; }
		void SetMultisampleState(VkPipelineMultisampleStateCreateInfo* pMultisampleState) { m_CreateInfo.pMultisampleState = pMultisampleState; }
		void SetDepthStencilState(VkPipelineDepthStencilStateCreateInfo* pDepthStencilState) { m_CreateInfo.pDepthStencilState = pDepthStencilState; }
		void SetColorBlendState(VkPipelineColorBlendStateCreateInfo* pColorBlendState) { m_CreateInfo.pColorBlendState = pColorBlendState; }
		void SetDynamicState(VkPipelineDynamicStateCreateInfo* pDynamicState) { m_CreateInfo.pDynamicState = pDynamicState; }
		void SetLayout(VkPipelineLayout layout) { m_CreateInfo.layout = layout; }
		void SetRenderPass(VkRenderPass renderPass) { m_CreateInfo.renderPass = renderPass; }
		void SetSubpass(uint32_t subpass) { m_CreateInfo.subpass = subpass; }
		void SetBasePipelineHandle(VkPipeline basePipelineHandle) { m_CreateInfo.basePipelineHandle = basePipelineHandle; }
		void SetBasePipelineIndex(int32_t basePipelineIndex) { m_CreateInfo.basePipelineIndex = basePipelineIndex; }

		VkPipelineCreateFlags GetFlags() { return m_CreateInfo.flags; }
		uint32_t GetStageCount() { return m_CreateInfo.stageCount; }
		const VkPipelineShaderStageCreateInfo* GetStages() { return m_CreateInfo.pStages; }
		const VkPipelineVertexInputStateCreateInfo* GetVertexInputState() { return m_CreateInfo.pVertexInputState; }
		const VkPipelineInputAssemblyStateCreateInfo* GetInputAssemblyState() { return m_CreateInfo.pInputAssemblyState; }
		const VkPipelineTessellationStateCreateInfo* GetTessellationState() { return m_CreateInfo.pTessellationState; }
		const VkPipelineViewportStateCreateInfo* GetViewportState() { return m_CreateInfo.pViewportState; }
		const VkPipelineRasterizationStateCreateInfo* GetRasterizationState() { return m_CreateInfo.pRasterizationState; }
		const VkPipelineMultisampleStateCreateInfo* GetMultisampleState() { return m_CreateInfo.pMultisampleState; }
		const VkPipelineDepthStencilStateCreateInfo* GetDepthStencilState() { return m_CreateInfo.pDepthStencilState; }
		const VkPipelineColorBlendStateCreateInfo* GetColorBlendState() { return m_CreateInfo.pColorBlendState; }
		const VkPipelineDynamicStateCreateInfo* GetDynamicState() { return m_CreateInfo.pDynamicState; }
		VkPipelineLayout GetLayout() { return m_CreateInfo.layout; }
		VkRenderPass GetRenderPass() { return m_CreateInfo.renderPass; }
		uint32_t GetSubpass() { return m_CreateInfo.subpass; }
		VkPipeline GetBasePipelineHandle() { return m_CreateInfo.basePipelineHandle; }
		int32_t GetBasePipelineIndex() { return m_CreateInfo.basePipelineIndex; }
	private:
		VkGraphicsPipelineCreateInfo m_CreateInfo = {};
	};
}