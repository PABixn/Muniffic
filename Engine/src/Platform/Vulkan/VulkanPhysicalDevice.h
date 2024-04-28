#pragma once

#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {

	class VulkanPhysicalDevice {
	public:
		VulkanPhysicalDevice();
		~VulkanPhysicalDevice();
		void PickPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface);
		bool IsDeviceSuitable(VkPhysicalDevice device, const VkSurfaceKHR& surface);
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		int RateDeviceSuitability(VkPhysicalDevice device, const VkSurfaceKHR& surface);
		VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }

		
	private:
		VkPhysicalDevice m_PhysicalDevice;
	};

}