#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include "VulkanInstance.h"
#include "VulkanValidationLayers.h"
#include "VulkanDebugMessenger.h"
#include "VulkanSurface.h"
#include "VulkanPhysicalDevice.h"

#include <GLFW/glfw3.h>



namespace eg {

	class VulkanHandler {
	public:
		VulkanHandler();
		void Init();
		void DrawFrame();
		void Cleanup();

		VkSampleCountFlagBits GetMaxUsableSampleCount();
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	private:
		~VulkanHandler();
	private:
		VulkanInstance m_Instance;
		VulkanValidationLayers m_ValidationLayers;
		VulkanDebugMessenger m_DebugMessenger;
		VulkanSurface m_Surface;
		VulkanPhysicalDevice m_PhysicalDevice;
	};
}