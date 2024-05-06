#include "egpch.h"
#include "VulkanRenderPaassBuilder.h"

namespace eg {

	VulkanRenderPassBuilder& VulkanRenderPassBuilder::AddSubpass(VkPipelineBindPoint bindPoint, std::vector<VulkanAttachment> colorAttachments, VulkanAttachment depthStencilAttachment)
	{
		m_Subpasses.push_back(VulkanSubpassBuilder::Get().SetSubpassBindPoint(bindPoint).AddAttachments(colorAttachments).AddAttachment(depthStencilAttachment).Build());
	}

	VulkanRenderPassBuilder& VulkanRenderPassBuilder::AddDependency(uint32_t srcSubpass, uint32_t dstSubpass)
	{
		m_Dependencies.push_back(VulkanSubpassDependency::CreateSubpassDependency(srcSubpass, dstSubpass));
	}

	VulkanRenderPass& VulkanRenderPassBuilder::Build()
	{
		VulkanRenderPass renderPass;
		std::vector<VkSubpassDescription> subpassDescriptions;
		std::vector<VkSubpassDependency> dependencies;
		std::vector<VkAttachmentDescription> attachments;
		for (auto& subpass : m_Subpasses)
		{
			subpassDescriptions.push_back(subpass.GetSubpassDescription());
			for (auto& attachment : subpass.GetAttachmentDescriptions())
			{
				attachments.push_back(attachment);
			}
		}
		for (auto& dependency : m_Dependencies)
		{
			dependencies.push_back(dependency.GetSubpassDependency());
		}
		
		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = static_cast<uint32_t>(subpassDescriptions.size());
		renderPassInfo.pSubpasses = subpassDescriptions.data();
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();

		if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass.GetRenderPass()) != VK_SUCCESS)
		{
			EG_CORE_ERROR("Failed to create render pass!");
		}

		renderPass.InitPipelineBuilder(device);

		return renderPass;
	}

}
