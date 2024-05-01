#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanDeviceExtensions {
	public:
		VulkanDeviceExtensions() = default;
		VulkanDeviceExtensions(const std::vector<const char*>& deviceExtensions)
			: deviceExtensions(deviceExtensions) { };
		~VulkanDeviceExtensions() = default;

		bool checkDeviceExtensionSupport(VkPhysicalDevice device);

		const std::vector<const char*>& getDeviceExtensions() const {
			return deviceExtensions;
		}
	private:
		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
	};
}