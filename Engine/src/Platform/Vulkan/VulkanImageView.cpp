#include "egpch.h"
#include "VulkanImageView.h"

namespace eg {
	VulkanImageView& VulkanImageView::Create(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = image;
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = format;
		createInfo.subresourceRange.aspectMask = aspectFlags;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = mipLevels;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		if (vkCreateImageView(device, &createInfo, nullptr, &imageView) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create texture image view!");
		}

		VulkanImageView imageViewObj = VulkanImageView();
		imageViewObj.m_ImageView = imageView;
		return imageViewObj;
	}

	void VulkanImageView::Destroy(VkDevice device)
	{
		vkDestroyImageView(device, m_ImageView, nullptr);
	}
}
