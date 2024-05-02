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

		void generateMipMaps(VkImage image, VkFormat format, int32_t texWidth, int32_t texHeight, int32_t mipLevels);
		
		VkSampleCountFlagBits GetMaxUsableSampleCount();

		void Cleanup();
		void CleanupSwapChain();

		const VkSwapchainKHR& GetSwapChain() { return m_Swapchain; }
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