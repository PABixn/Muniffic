#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanQueue.h"
#include <GLFW/glfw3.h>

namespace eg {

	class VulkanLogicalDevice {
	public:
		VulkanLogicalDevice();
		~VulkanLogicalDevice();
		void CreateLogicalDevice(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);
		void Cleanup();
		const VkDevice& GetDevice() { return m_Device; }
		const VulkanQueue& GetGraphicsQueue() { return m_GraphicsQueue; }
		const VulkanQueue& GetPresentQueue() { return m_PresentQueue; }
	private:
		VkDevice m_Device;
		VulkanQueue m_GraphicsQueue;
		VulkanQueue m_PresentQueue;
	};

}