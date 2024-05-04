#pragma once
#include "egpch.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanPipelineColorBlendAttachmentState {
	public:
		VulkanPipelineColorBlendAttachmentState();
		VulkanPipelineColorBlendAttachmentState(const VkPipelineColorBlendAttachmentState& state);
		VulkanPipelineColorBlendAttachmentState(VkColorComponentFlags colorWriteMask, VkBool32 blendEnable);
		VulkanPipelineColorBlendAttachmentState(VkColorComponentFlags colorWriteMask, VkBool32 blendEnable, VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor, VkBlendOp colorBlendOp, VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor, VkBlendOp alphaBlendOp);
		~VulkanPipelineColorBlendAttachmentState() = default;

		VkPipelineColorBlendAttachmentState* getColorBlendAttachmentState() { return &m_CreateInfo; }
		void setColorBlendAttachmentState(VkPipelineColorBlendAttachmentState colorBlendAttachmentState) { colorBlendAttachmentState = colorBlendAttachmentState; }
		void setBlendEnable(VkBool32 blendEnable) { m_CreateInfo.blendEnable = blendEnable; }
		void setSrcColorBlendFactor(VkBlendFactor srcColorBlendFactor) { m_CreateInfo.srcColorBlendFactor = srcColorBlendFactor; }
		void setDstColorBlendFactor(VkBlendFactor dstColorBlendFactor) { m_CreateInfo.dstColorBlendFactor = dstColorBlendFactor; }
		void setColorBlendOp(VkBlendOp colorBlendOp) { m_CreateInfo.colorBlendOp = colorBlendOp; }
		void setSrcAlphaBlendFactor(VkBlendFactor srcAlphaBlendFactor) { m_CreateInfo.srcAlphaBlendFactor = srcAlphaBlendFactor; }
		void setDstAlphaBlendFactor(VkBlendFactor dstAlphaBlendFactor) { m_CreateInfo.dstAlphaBlendFactor = dstAlphaBlendFactor; }
		void setAlphaBlendOp(VkBlendOp alphaBlendOp) { m_CreateInfo.alphaBlendOp = alphaBlendOp; }
		void setColorWriteMask(VkColorComponentFlags colorWriteMask) { m_CreateInfo.colorWriteMask = colorWriteMask; }

		operator VkPipelineColorBlendAttachmentState* () { return &m_CreateInfo; }
		operator VkPipelineColorBlendAttachmentState() { return m_CreateInfo; }
		operator VkPipelineColorBlendAttachmentState&() { return m_CreateInfo; }
		operator const VkPipelineColorBlendAttachmentState* () const { return &m_CreateInfo; }
		operator const VkPipelineColorBlendAttachmentState() const { return m_CreateInfo; }
		operator const VkPipelineColorBlendAttachmentState&() const { return m_CreateInfo; }
	private:
		VkPipelineColorBlendAttachmentState m_CreateInfo;
	};
}