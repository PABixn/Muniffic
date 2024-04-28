#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanQueue.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanSwapchain {
	public:
		VulkanSwapchain();
		~VulkanSwapchain();
		void CreateSwapchain(const VkDevice& device, const VkSurfaceKHR& surface, const VkPhysicalDevice& physicalDevice, const VkExtent2D& extent);
		void Cleanup(const VkDevice& device);
		const VkSwapchainKHR& GetSwapchain() { return m_Swapchain; }
	private:
		VkSwapchainKHR m_Swapchain;
	};
}