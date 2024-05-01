#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanImageView.h"
#include "VulkanDeviceMemory.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanImage {
	public:
		VulkanImage(VkDevice logicalDevice,  uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
		~VulkanImage();

		void TransitionImageLayout(VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);

		VkImage GetImage() const { return m_Image; }
	private:
		VkImage m_Image;
		VulkanDeviceMemory m_ImageMemory;
		VulkanImageView m_ImageView;
	};
}