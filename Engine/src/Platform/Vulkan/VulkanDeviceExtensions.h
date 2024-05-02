#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanValidationLayers.h"
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanDeviceExtensions {
	public:
		VulkanDeviceExtensions() = default;
		VulkanDeviceExtensions(const std::vector<const char*>& deviceExtensions)
			: deviceExtensions(deviceExtensions) { };
		~VulkanDeviceExtensions() = default;

		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		
		std::vector<const char*> GetRequiredExtensions();

		const std::vector<const char*>& GetDeviceExtensions() const {
			return deviceExtensions;
		}


		size_t size() const {
			return deviceExtensions.size();
		}
	private:
		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};
	};
}