#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanAttachment.h"
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanSubpass {
	public:
		VulkanSubpass() = default;
		~VulkanSubpass() = default;

		VkSubpassDescription& GetSubpassDescription() { return m_SubpassDescription; }
		const VkSubpassDescription& GetSubpassDescription() const { return m_SubpassDescription; }
		std::vector<VkAttachmentDescription>& GetAttachmentDescriptions() { return m_AttachmentDescriptions; }
		const std::vector<VkAttachmentDescription>& GetAttachmentDescriptions() const { return m_AttachmentDescriptions; }

		friend class VulkanSubpassBuilder;
	private:
		VkSubpassDescription m_SubpassDescription;
		std::vector<VkAttachmentDescription> m_AttachmentDescriptions;
	};
}