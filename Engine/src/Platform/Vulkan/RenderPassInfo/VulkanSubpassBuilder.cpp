#include "egpch.h"
#include "VulkanSubpassBuilder.h"

namespace eg {

	VulkanSubpassBuilder& VulkanSubpassBuilder::AddColorAttachment(VkFormat format, VkSampleCountFlagBits samples, VkImageLayout initialLayout, VkImageLayout finalLayout, bool preserve)
	{
		m_Attachments.push_back(VulkanAttachment::CreateColorAttachment(format, samples, initialLayout, finalLayout, preserve));
		return *this;
	}

	VulkanSubpassBuilder& VulkanSubpassBuilder::SetDepthStencilAttachment(VkFormat format, VkSampleCountFlagBits samples, VkImageLayout initialLayout, VkImageLayout finalLayout, bool preserve)
	{
		m_Attachments.push_back(VulkanAttachment::CreateDepthAttachment(format, samples, initialLayout, finalLayout, preserve));
		return *this;
	}

	VulkanSubpassBuilder& VulkanSubpassBuilder::AddInputAttachment(VkFormat format, VkSampleCountFlagBits samples, VkImageLayout initialLayout, VkImageLayout finalLayout, bool preserve)
	{
		m_Attachments.push_back(VulkanAttachment::CreateInputAttachment(format, samples, initialLayout, finalLayout, preserve));
		return *this;
	}

	VulkanSubpassBuilder& VulkanSubpassBuilder::AddResolveAttachment(VkFormat format, VkImageLayout initialLayout, VkImageLayout finalLayout, bool preserve)
	{
		m_Attachments.push_back(VulkanAttachment::CreateResolveAttachment(format, initialLayout, finalLayout, preserve));
		return *this;
	}

	VulkanSubpassBuilder& VulkanSubpassBuilder::AddAttachment(VulkanAttachment& attachment)
	{
		m_Attachments.push_back(attachment);
		return *this;
	}

	VulkanSubpassBuilder& VulkanSubpassBuilder::AddAttachments(std::vector<VulkanAttachment>& attachments)
	{
		m_Attachments.insert(m_Attachments.end(), attachments.begin(), attachments.end());
	}

	VulkanSubpass& VulkanSubpassBuilder::Build()
	{
		VulkanSubpass subpass;
		std::vector<VkAttachmentReference> colorAttachments;
		VkAttachmentReference depthStencilAttachment = {};
		std::vector<VkAttachmentReference> inputAttachments;
		std::vector<VkAttachmentReference> resolveAttachments;
		std::vector<uint32_t> preserveAttachments;
		int i = 0;
		for (auto& attachment : m_Attachments)
		{
			attachment.m_AttachmentReference.attachment = i;
			switch (attachment.m_AttachmentType)
			{
			case VulkanAttachmentType::COLOR:
				colorAttachments.push_back(attachment.m_AttachmentReference);
				break;
			case VulkanAttachmentType::DEPTH:
				depthStencilAttachment = attachment.m_AttachmentReference;
				break;
			case VulkanAttachmentType::INPUT:
				inputAttachments.push_back(attachment.m_AttachmentReference);
				break;
			case VulkanAttachmentType::RESOLVE:
				resolveAttachments.push_back(attachment.m_AttachmentReference);
				break;
			}
			if (attachment.m_Preserve)
				preserveAttachments.push_back(i);
			subpass.m_AttachmentDescriptions.push_back(attachment.m_AttachmentDescription);
			i++;
		}

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = m_BindPoint;
		subpassDescription.colorAttachmentCount = colorAttachments.size();
		subpassDescription.pColorAttachments = colorAttachments.data();
		subpassDescription.pDepthStencilAttachment = &depthStencilAttachment;
		subpassDescription.inputAttachmentCount = inputAttachments.size();
		subpassDescription.pInputAttachments = inputAttachments.data();
		subpassDescription.preserveAttachmentCount = preserveAttachments.size();
		subpassDescription.pPreserveAttachments = preserveAttachments.data();
		subpassDescription.pResolveAttachments = resolveAttachments.data();

		subpass.m_SubpassDescription = subpassDescription;

		return subpass;
	}

}
