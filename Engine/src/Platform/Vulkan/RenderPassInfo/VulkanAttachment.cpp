#include "egpch.h"
#include "VulkanAttachment.h"

namespace eg {

	VulkanAttachment& VulkanAttachment::CreateAttachment(VkFormat format, VkSampleCountFlagBits samples, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkImageLayout initialLayout, VkImageLayout finalLayout, uint32_t refAttachment, VulkanAttachmentType type, bool preserve)
	{
		VulkanAttachment attachment;
		auto& attachmetnDescription = attachment.m_AttachmentDescription;
		auto& attachmentReference = attachment.m_AttachmentReference;
		attachmetnDescription.format = format;
		attachmetnDescription.samples = samples;
		attachmetnDescription.loadOp = loadOp;
		attachmetnDescription.storeOp = storeOp;
		attachmetnDescription.stencilLoadOp = stencilLoadOp;
		attachmetnDescription.stencilStoreOp = stencilStoreOp;
		attachmetnDescription.initialLayout = initialLayout;
		attachmetnDescription.finalLayout = finalLayout;

		attachmentReference.attachment = refAttachment;
		attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		attachment.m_AttachmentType = type;

		attachment.m_Preserve = preserve;

		return attachment;
	}

	VulkanAttachment& VulkanAttachment::CreateColorAttachment(VkFormat format, VkSampleCountFlagBits samples, VkImageLayout initialLayout, VkImageLayout finalLayout, bool preserve, uint32_t refAttachment)
	{
		VulkanAttachment attachment;
		auto& attachmetnDescription = attachment.m_AttachmentDescription;
		auto& attachmentReference = attachment.m_AttachmentReference;

		attachmetnDescription.format = format;
		attachmetnDescription.samples = samples;
		attachmetnDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachmetnDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachmetnDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmetnDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmetnDescription.initialLayout = initialLayout;
		attachmetnDescription.finalLayout = finalLayout;

		attachmentReference.attachment = refAttachment;
		attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		attachment.m_AttachmentType = VulkanAttachmentType::COLOR;

		attachment.m_Preserve = preserve;

		return attachment;
	}

	VulkanAttachment& VulkanAttachment::CreateColorAttachment(VkFormat format, VkSampleCountFlagBits samples, bool preserve)
	{
		return CreateColorAttachment(format, samples, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, preserve);
	}

	VulkanAttachment& VulkanAttachment::CreateDepthAttachment(VkFormat format, VkSampleCountFlagBits samples, VkImageLayout initialLayout, VkImageLayout finalLayout, bool preserve, uint32_t refAttachment)
	{
		VulkanAttachment attachment;
		auto& attachmetnDescription = attachment.m_AttachmentDescription;
		auto& attachmentReference = attachment.m_AttachmentReference;

		attachmetnDescription.format = format;
		attachmetnDescription.samples = samples;
		attachmetnDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachmetnDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmetnDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmetnDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmetnDescription.initialLayout = initialLayout;
		attachmetnDescription.finalLayout = finalLayout;

		attachmentReference.attachment = refAttachment;
		attachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		attachment.m_AttachmentType = VulkanAttachmentType::DEPTH;

		attachment.m_Preserve = preserve;

		return attachment;
	}

	VulkanAttachment& VulkanAttachment::CreateDepthAttachment(VkFormat format, VkSampleCountFlagBits samples, bool preserve)
	{
		return CreateDepthAttachment(format, samples, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, preserve);
	}

	VulkanAttachment& VulkanAttachment::CreateResolveAttachment(VkFormat format, VkImageLayout initialLayout, VkImageLayout finalLayout, bool preserve, uint32_t refAttachment)
	{
		VulkanAttachment attachment;
		auto& attachmetnDescription = attachment.m_AttachmentDescription;
		auto& attachmentReference = attachment.m_AttachmentReference;

		attachmetnDescription.format = format;
		attachmetnDescription.samples = VK_SAMPLE_COUNT_1_BIT;
		attachmetnDescription.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmetnDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachmetnDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmetnDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmetnDescription.initialLayout = initialLayout;
		attachmetnDescription.finalLayout = finalLayout;

		attachmentReference.attachment = refAttachment;
		attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		attachment.m_AttachmentType = VulkanAttachmentType::RESOLVE;

		attachment.m_Preserve = preserve;

		return attachment;
	}

	VulkanAttachment& VulkanAttachment::CreateResolveAttachment(VkFormat format, bool preserve)
	{
		return CreateResolveAttachment(format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, preserve);
	}

	VulkanAttachment& VulkanAttachment::CreateInputAttachment(VkFormat format, VkSampleCountFlagBits samples, VkImageLayout initialLayout, VkImageLayout finalLayout, bool preserve, uint32_t refAttachment)
	{
		VulkanAttachment attachment;
		auto& attachmetnDescription = attachment.m_AttachmentDescription;
		auto& attachmentReference = attachment.m_AttachmentReference;

		attachmetnDescription.format = format;
		attachmetnDescription.samples = samples;
		attachmetnDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachmetnDescription.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmetnDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachmetnDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachmetnDescription.initialLayout = initialLayout;
		attachmetnDescription.finalLayout = finalLayout;

		attachmentReference.attachment = refAttachment;
		attachmentReference.layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		attachment.m_AttachmentType = VulkanAttachmentType::INPUT;

		attachment.m_Preserve = preserve;

		return attachment;
	}

	VulkanAttachment& VulkanAttachment::CreateInputAttachment(VkFormat format, VkSampleCountFlagBits samples, bool preserve)
	{
		return CreateInputAttachment(format, samples, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, preserve);
	}

}
