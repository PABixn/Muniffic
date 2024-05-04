#pragma once
#define GLFW_INCLUDE_VULKAN
#include "VulkanImage.h"
#include "VulkanTexture2D.h"
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanImageFactory {
	public:
		VulkanImageFactory() = default;
		~VulkanImageFactory() = default;
		static void Init(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkCommandPool commandPool, uint32_t mipLevels = 1);

		static VulkanImage& CreateImage(VkDevice logicalDevice = s_Device, uint32_t width, uint32_t height, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, uint32_t mipLevels = s_MipLevels);
		static VulkanImage& CreateImage(VkDevice logicalDevice = s_Device, uint32_t width, uint32_t height, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImageAspectFlags aspectFlags, uint32_t mipLevels = s_MipLevels);
		static VulkanImage& CreateImage(const std::string& path, uint32_t mipLevels = s_MipLevels);
		static VulkanImageView& CreateImageView(VkDevice logicalDevice = s_Device, VkImage& image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels = s_MipLevels);
		static void GenerateMipMaps(VkPhysicalDevice physicalDevice = s_PhysicalDevice, VkImage& image, VkFormat format, int32_t texWidth, int32_t texHeight, uint32_t mipLevels = s_MipLevels);

		static void DestroyImage(VulkanImage& image, VkDevice logicalDevice = s_Device);
		static void DestroyImageView(VulkanImageView& imageView, VkDevice logicalDevice = s_Device);

		Ref<Texture2D> Create(const std::string& path);
		Ref<Texture2D> Create(const UUID& uuid);
		Ref<Texture2D> Create(const TextureSpecification& specification);

		friend class VukanImage;
		friend class VulkanImageView;
	private:
		static VkDevice s_Device;
		static VkPhysicalDevice s_PhysicalDevice;
		static VkCommandPool s_CommandPool;
		static uint32_t s_MipLevels;

		
	};
}