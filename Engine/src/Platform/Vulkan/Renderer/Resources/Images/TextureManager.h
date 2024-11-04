#pragma once
#include "VulkanTexture.h"
namespace eg {
	class TextureManager {
	public:
		void init() {};
		void TransitionTextureLayout(VulkanTexture& texture, const VkFormat& format, const VkImageLayout& oldLayout, const VkImageLayout& newLayout);
		void TransitionTextureLayout(VkImage& texture, const VkFormat& format, const VkImageLayout& oldLayout, const VkImageLayout& newLayout);
		void CopyBufferToImage(VulkanBuffer& buffer, VulkanTexture& texture);
	};
}