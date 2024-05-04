#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanQueue.h"
#include "VulkanImageView.h"
#include "VulkanImage.h"
#include <GLFW/glfw3.h>

namespace eg {
	struct VulkanSwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;

		static VulkanSwapChainSupportDetails QuerySwapChainSupportDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	};

	class VulkanSwapChain {
	public:
		VulkanSwapChain() = default;
		~VulkanSwapChain() = default;
		void Init();
		void CreateImageViews();
		void CreateColorResources();
		void CreateDepthResources();
		void CreateFramebuffers();
		
		void RecreateSwapChain();
		
		VkSampleCountFlagBits GetMaxUsableSampleCount();

		void Cleanup();
		void CleanupSwapChain();

		const VkSwapchainKHR& GetSwapChain() { return m_Swapchain; }
		const std::vector<VkImage>& GetImages() const { return m_Images; }
		const VkFormat& GetImageFormat() const { return m_ImageFormat; }
		const VkExtent2D& GetExtent() const { return m_Extent; }
		const std::vector<VulkanImageView>& GetImageViews() const { return m_ImageViews; }
		const VulkanImage& GetColorImage() const { return m_ColorImage; }
		const VulkanImage& GetDepthImage() const { return m_DepthImage; }
		const VkSampleCountFlagBits& GetMsaaSamples() const { return m_MsaaSamples; }
		const uint32_t& GetMipLevels() const { return m_MipLevels; }
	private:
		void ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	private:
		VkSwapchainKHR m_Swapchain;
		std::vector<VkImage> m_Images;
		VkFormat m_ImageFormat;
		VkExtent2D m_Extent;
		std::vector<VulkanImageView> m_ImageViews;
		VulkanImage m_ColorImage;
		VulkanImage m_DepthImage;
		std::vector<VkFramebuffer> m_Framebuffers;
		VkSampleCountFlagBits m_MsaaSamples = VK_SAMPLE_COUNT_1_BIT;
		uint32_t m_MipLevels = 1;
	};
}