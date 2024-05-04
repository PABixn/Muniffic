#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanImageView {
	public:
		VulkanImageView() = default;
		~VulkanImageView() = default;

		VkImageView GetImageView() const { return m_ImageView; }
		friend class VulkanImageFactory;
		friend class VulkanImage;
	private:
		VulkanImageView& Create(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
		void Destroy(VkDevice device);
	private:
		VkImageView m_ImageView;
		
	};
}