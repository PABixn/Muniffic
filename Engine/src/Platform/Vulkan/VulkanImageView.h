#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanImageView {
	public:
		VulkanImageView() = default;
		static VulkanImageView& Create(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
		void Cleanup(VkDevice device);
		~VulkanImageView() = default;

		VkImageView GetImageView() const { return m_ImageView; }
	private:
		VkImageView m_ImageView;
	};
}