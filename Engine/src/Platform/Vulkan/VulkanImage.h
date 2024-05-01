#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Renderer/Texture.h"
#include <GLFW/glfw3.h>

namespace eg{
	class VulkanImage: public Texture2D {
	public:
		VulkanImage();
		~VulkanImage();

		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(const UUID& uuid);
		static Ref<Texture2D> Create(const TextureSpecification& specification);

		void createImage();
		void createImageView(VkFormat format, VkImageAspectFlags aspectFlags);
		void createSampler();
		void transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
		void copyBufferToImage(VkBuffer buffer, uint32_t width, uint32_t height);
		void cleanup();
		VkImage getImage();
		VkImageView getImageView();
		VkSampler getSampler();

	private:
		UUID m_Id;
		VkImage image;
		VkDeviceMemory imageMemory;
		VkImageView imageView;
		TextureSpecification m_Specification;
		uint32_t m_Width, m_Height;
		
	};
}