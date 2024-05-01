#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanImageView {
	public:
		VulkanImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
		~VulkanImageView();

		VkImageView GetImageView() const { return imageView; }
	private:
		VkImageView imageView;
	};
}