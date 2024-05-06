#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanRenderPass.h"
#include "RenderPassInfo/VulkanSubpassBuilder.h"
#include "RenderPassInfo/VulkanSubpassDependency.h"

namespace eg {
	class VulkanRenderPassBuilder {
	public:
		VulkanRenderPassBuilder() = default;
		~VulkanRenderPassBuilder() = default;

		void Init(VkDevice device) { this->device = device; }

		VulkanRenderPassBuilder& AddSubpass(VkPipelineBindPoint bindPoint, std::vector<VulkanAttachment> colorAttachments, VulkanAttachment depthStencilAttachment);
		VulkanRenderPassBuilder& AddSubpass(VulkanSubpass& subpass) { m_Subpasses.push_back(subpass); }
		VulkanRenderPassBuilder& AddDependency(uint32_t srcSubpass, uint32_t dstSubpass);
		VulkanRenderPassBuilder& AddDependency(VulkanSubpassDependency& dependency) { m_Dependencies.push_back(dependency); }

		VulkanRenderPass& Build();
	private:
		VkDevice device;
		std::vector<VulkanSubpass> m_Subpasses;
		std::vector<VulkanSubpassDependency> m_Dependencies;
	};
}