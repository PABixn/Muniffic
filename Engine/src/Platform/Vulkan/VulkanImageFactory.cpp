#include "egpch.h"
#include "VulkanImageFactory.h"

namespace eg {
	void VulkanImageFactory::Init(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, uint32_t mipLevels)
	{
		s_Device = logicalDevice;
		s_PhysicalDevice = physicalDevice;
		s_CommandPool = commandPool;
		s_MipLevels = mipLevels;
	}

	VulkanImage& VulkanImageFactory::CreateImage(VkDevice logicalDevice, uint32_t width, uint32_t height, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint32_t mipLevels)
	{
		VulkanImage image;
		image.Create(logicalDevice, width, height, mipLevels, numSamples, format, tiling, usage, properties);
		return image;
	}

	VulkanImage& VulkanImageFactory::CreateImage(VkDevice logicalDevice, uint32_t width, uint32_t height, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
	{
		VulkanImage image;
		image.Create(logicalDevice, width, height, mipLevels, numSamples, format, tiling, usage, properties, aspectFlags);
		return image;
	}

	VulkanImage& VulkanImageFactory::CreateImage(const std::string& path, uint32_t mipLevels)
	{
		VulkanImage image;
		image.Load(s_Device, s_PhysicalDevice, s_CommandPool, path, mipLevels);
		return image;
	}

	VulkanImageView& eg::VulkanImageFactory::CreateImageView(VkDevice logicalDevice, VkImage& image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
	{
		VulkanImageView imageView;
		imageView.Create(logicalDevice, image, format, aspectFlags, mipLevels);
		return imageView;
	}

	void VulkanImageFactory::GenerateMipMaps(VkPhysicalDevice physicalDevice, VkImage& image, VkFormat format, int32_t texWidth, int32_t texHeight, uint32_t mipLevels)
	{
		VulkanImage::GenerateMipMaps(physicalDevice, image, format, texWidth, texHeight, mipLevels);
	}

	void VulkanImageFactory::DestroyImage(VulkanImage& image, VkDevice logicalDevice)
	{
		image.Destroy(logicalDevice);
	}

	void VulkanImageFactory::DestroyImageView(VulkanImageView& imageView, VkDevice logicalDevice)
	{
		imageView.Destroy(logicalDevice);
	}
}
