#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanQueue.h"
#include "VulkanImageView.h"
#include "VulkanImage.h"

#include <GLFW/glfw3.h>

namespace eg {
	struct VulkanSwapchainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;

		static VulkanSwapchainSupportDetails querySwapChainSupportDetails(VkPhysicalDevice physicalDevice);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	};

	class VulkanSwapchain {
	public:
		VulkanSwapchain();
		~VulkanSwapchain();
		void CreateSwapchain(const VkDevice& device, const VkSurfaceKHR& surface, const VkPhysicalDevice& physicalDevice, const VkExtent2D& extent);
		void CreateImageViews(const VkDevice& device);
		void CreateColorResources(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VulkanQueue& queue, const VkCommandPool& commandPool);
		void CreateDepthResources(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VulkanQueue& queue, const VkCommandPool& commandPool);
		void CreateFramebuffers(const VkDevice& device, const VkRenderPass& renderPass);
		
		void RecreateSwapChain(const VkDevice& device, const VkSurfaceKHR& surface, const VkPhysicalDevice& physicalDevice, const VkExtent2D& extent);
		

		void Cleanup(const VkDevice& device);
		const VkSwapchainKHR& GetSwapchain() { return m_Swapchain; }
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
	};
}