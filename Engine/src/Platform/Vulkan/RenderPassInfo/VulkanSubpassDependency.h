#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanSubpassDependency {
	public:
		VulkanSubpassDependency() = default;
		~VulkanSubpassDependency() = default;

		static VulkanSubpassDependency& CreateSubpassDependency(uint32_t srcSubpass, uint32_t dstSubpass);
		static VulkanSubpassDependency& CreateSubpassDependency(uint32_t srcSubpass, uint32_t dstSubpass, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkDependencyFlags dependencyFlags);
		static VulkanSubpassDependency& CreateSubpassDependency(uint32_t srcSubpass, uint32_t dstSubpass, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkDependencyFlags dependencyFlags, uint32_t srcStageMask, uint32_t dstStageMask);

		VkSubpassDependency& GetSubpassDependency() { return m_SubpassDependency; }
		const VkSubpassDependency& GetSubpassDependency() const { return m_SubpassDependency; }

		operator VkSubpassDependency& () { return m_SubpassDependency; }
		operator const VkSubpassDependency& () const { return m_SubpassDependency; }
		operator VkSubpassDependency* () { return &m_SubpassDependency; }
		operator const VkSubpassDependency* () const { return &m_SubpassDependency; }
	private:
		VkSubpassDependency m_SubpassDependency;
	};
}