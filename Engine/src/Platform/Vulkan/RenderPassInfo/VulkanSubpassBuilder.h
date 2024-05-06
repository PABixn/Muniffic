#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanSubpass.h"
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanSubpassBuilder {
	public:
		~VulkanSubpassBuilder() = default;
		
		void Init(VkDevice device) { m_Device = device; }
		void Cleanup() { m_Attachments.clear(); }

		static VulkanSubpassBuilder& Get() {
			static VulkanSubpassBuilder instance;
			return instance;
		}

		VulkanSubpassBuilder& AddColorAttachment(VkFormat format, VkSampleCountFlagBits samples, VkImageLayout initialLayout, VkImageLayout finalLayout, bool preserve);
		VulkanSubpassBuilder& SetDepthStencilAttachment(VkFormat format, VkSampleCountFlagBits samples, VkImageLayout initialLayout, VkImageLayout finalLayout, bool preserve);
		VulkanSubpassBuilder& AddInputAttachment(VkFormat format, VkSampleCountFlagBits samples, VkImageLayout initialLayout, VkImageLayout finalLayout, bool preserve);
		VulkanSubpassBuilder& AddResolveAttachment(VkFormat format, VkImageLayout initialLayout, VkImageLayout finalLayout, bool preserve);
		VulkanSubpassBuilder& AddAttachment(VulkanAttachment& attachment);
		VulkanSubpassBuilder& AddAttachments(std::vector<VulkanAttachment>& attachments);
		VulkanSubpassBuilder& SetSubpassDescription(VkPipelineBindPoint bindPoint) { m_BindPoint = bindPoint; return *this; }
		VulkanSubpassBuilder& SetSubpassBindPoint(VkPipelineBindPoint bindPoint) { m_BindPoint = bindPoint; return *this; }

		VulkanSubpass& Build();
	private:
		VulkanSubpassBuilder() = default;
	private:
		VkDevice m_Device;
		std::vector<VulkanAttachment> m_Attachments;
		VkPipelineBindPoint m_BindPoint;
	};
}