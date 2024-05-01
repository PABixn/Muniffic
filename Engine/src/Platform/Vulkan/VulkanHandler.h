#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include "VulkanInstance.h"
#include "VulkanValidationLayers.h"
#include "VulkanDebugMessenger.h"
#include "VulkanSurface.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanMultisampling.h"
#include "VulkanCommandBuffer.h"

#include <GLFW/glfw3.h>



namespace eg {
	namespace VulkanConstants {
		constexpr const uint32_t MAX_FRAMES_IN_FLIGHT = 2;
}
	class VulkanHandler {
	public:
		VulkanHandler();
		void Init();
		void DrawFrame();
		void Cleanup();

		VkSampleCountFlagBits GetMaxUsableSampleCount();
		
	private:
		void CreateSurface();
		~VulkanHandler();
	private:
		VulkanInstance m_Instance;
		VulkanValidationLayers m_ValidationLayers;
		VulkanDebugMessenger m_DebugMessenger;
		VulkanSurface m_Surface;
		VulkanPhysicalDevice m_PhysicalDevice;
		VulkanMultisampling m_Multisampling;
		std::vector<VulkanCommandBuffer> m_commandBuffers;
	};
}