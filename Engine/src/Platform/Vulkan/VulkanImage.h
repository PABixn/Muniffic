#pragma once
#define GLFW_INCLUDE_VULKAN
#include "VulkanImageView.h"
#include "VulkanDeviceMemory.h"
#include <GLFW/glfw3.h>

#include <vector>
namespace eg{
	class VulkanImage {
	public:
		VulkanImage() = default;
		~VulkanImage() = default;
		static VulkanImage& Create(VkDevice logicalDevice, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
		static VulkanImage& Create(VkDevice logicalDevice, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags);
		

		static VkFormat FindSupportedFormat(VkDevice device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		static VkFormat FindDepthFormat(VkDevice device);
		static bool HasStencilComponent(VkFormat format);
		void CreateImageView(VkDevice logicalDevice, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
		void TransitionImageLayout(VkDevice logicalDevice, VkCommandPool commandPool, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
		void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
		void CopyBufferToImage(VkBuffer buffer, uint32_t width, uint32_t height);
		void Cleanup(VkDevice logicalDevice);

		VulkanImageView GetVulkanImageView() const { return m_ImageView; }
		VulkanImageView& GetVulkanImageViewRef() { return m_ImageView; }
		VulkanDeviceMemory GetVulkanImageMemory() const { return m_ImageMemory; }
		VulkanDeviceMemory& GetVulkanImageMemoryRef() { return m_ImageMemory; }
		VkImage GetImage() const { return m_Image; }
		VkImage& GetImageRef() { return m_Image; }
	private:
		VkImage m_Image;
		VulkanDeviceMemory m_ImageMemory;
		VulkanImageView m_ImageView;
	};
}