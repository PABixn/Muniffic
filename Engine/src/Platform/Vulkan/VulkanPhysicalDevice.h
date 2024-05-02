#pragma once

#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanDeviceExtensions.h"
#include <GLFW/glfw3.h>

namespace eg {

	class VulkanPhysicalDevice {
	public:
		VulkanPhysicalDevice() = default;
		~VulkanPhysicalDevice();
		void Init(const VkInstance& instance, const VkSurfaceKHR& surface, VulkanDeviceExtensions& extensions);
		void Cleanup();

		bool IsDeviceSuitable(VkPhysicalDevice device, const VkSurfaceKHR& surface, VulkanDeviceExtensions& extensions);
		int RateDeviceSuitability(VkPhysicalDevice device, const VkSurfaceKHR& surface);
		VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
		const VkPhysicalDevice& GetPhysicalDevice() const { return m_PhysicalDevice; }
	private:
		VkPhysicalDevice m_PhysicalDevice;
	};

}