#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include <GLFW/glfw3.h>

namespace eg{
	enum class VulkanAttachmentType {
		COLOR,
		DEPTH,
		RESOLVE,
		INPUT,
		PRESERVE
	};

	class VulkanAttachment {
	public:
		VulkanAttachment() = default;
		~VulkanAttachment() = default;

		static VulkanAttachment& CreateAttachment(VkFormat format, VkSampleCountFlagBits samples, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkImageLayout initialLayout, VkImageLayout finalLayout, uint32_t refAttachment, VulkanAttachmentType type, bool preserve);
		static VulkanAttachment& CreateColorAttachment(VkFormat format, VkSampleCountFlagBits samples, VkImageLayout initialLayout, VkImageLayout finalLayout, bool preserve, uint32_t refAttachment = 0);
		static VulkanAttachment& CreateColorAttachment(VkFormat format, VkSampleCountFlagBits samples, bool preserve);
		static VulkanAttachment& CreateDepthAttachment(VkFormat format, VkSampleCountFlagBits samples, VkImageLayout initialLayout, VkImageLayout finalLayout, bool preserve, uint32_t refAttachment = 0);
		static VulkanAttachment& CreateDepthAttachment(VkFormat format, VkSampleCountFlagBits samples, bool preserve);
		static VulkanAttachment& CreateResolveAttachment(VkFormat format, VkImageLayout initialLayout, VkImageLayout finalLayout, bool preserve, uint32_t refAttachment = 0);
		static VulkanAttachment& CreateResolveAttachment(VkFormat format, bool preserve);
		static VulkanAttachment& CreateInputAttachment(VkFormat format, VkSampleCountFlagBits samples, VkImageLayout initialLayout, VkImageLayout finalLayout, bool preserve, uint32_t refAttachment = 0);
		static VulkanAttachment& CreateInputAttachment(VkFormat format, VkSampleCountFlagBits samples, bool preserve);

		const VkAttachmentDescription& GetAttachmentDescription() const { return m_AttachmentDescription; }
		const VkAttachmentReference& GetAttachmentReference() const { return m_AttachmentReference; }
		VulkanAttachmentType GetAttachmentType() const { return m_AttachmentType; }

		operator VkAttachmentDescription& () { return m_AttachmentDescription; }
		operator const VkAttachmentDescription& () const { return m_AttachmentDescription; }
		operator VkAttachmentReference& () { return m_AttachmentReference; }
		operator const VkAttachmentReference& () const { return m_AttachmentReference; }
		operator VkAttachmentDescription* () { return &m_AttachmentDescription; }
		operator const VkAttachmentDescription* () const { return &m_AttachmentDescription; }
		operator VkAttachmentReference* () { return &m_AttachmentReference; }
		operator const VkAttachmentReference* () const { return &m_AttachmentReference; }

		friend class VulkanSubpassBuilder;
		private:
			VkAttachmentDescription m_AttachmentDescription;
			VkAttachmentReference m_AttachmentReference;
			VulkanAttachmentType m_AttachmentType;
			bool m_Preserve;
	};
}