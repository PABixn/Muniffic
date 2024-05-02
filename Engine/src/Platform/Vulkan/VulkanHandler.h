#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include "VulkanInstance.h"
#include "VulkanValidationLayers.h"
#include "VulkanDeviceExtensions.h"
#include "VulkanDebugMessenger.h"
#include "VulkanSurface.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanLogicalDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanCommandBuffer.h"

#include "VulkanDeviceMemoryManager.h"

#include <GLFW/glfw3.h>



namespace eg {
	namespace VulkanConstants {
		constexpr const uint32_t MAX_FRAMES_IN_FLIGHT = 2;
}
	class VulkanHandler {
	public:
		VulkanHandler() = default;
		VulkanHandler(GLFWwindow* window);
		~VulkanHandler();
		static Ref<VulkanHandler> Create(GLFWwindow* window);
		void DrawFrame();
		void Init();
		void Cleanup();

		const VulkanInstance& GetVulkanInstance() { return m_Instance; }
		const VulkanDeviceExtensions& GetVulkanDeviceExtensions() { return m_DeviceExtensions; }
		const VulkanSurface& GetVulkanSurface() { return m_Surface; }
		const VulkanPhysicalDevice& GetVulkanPhysicalDevice() { return m_PhysicalDevice; }
		const VulkanLogicalDevice& GetVulkanLogicalDevice() { return m_LogicalDevice; }
		const VulkanSwapChain& GetVulkanSwapChain() { return m_SwapChain; }
		const std::vector<VulkanCommandBuffer>& GetVulkanCommandBuffers() { return m_commandBuffers; }

		GLFWwindow* GetWindow() { return m_Window; }
		//VkCommandPool& GetCommandPool() { return m_SwapChain.GetCommandPool(); }
	private:
		

	private:
		VulkanInstance m_Instance;
		VulkanDeviceExtensions m_DeviceExtensions;
		VulkanSurface m_Surface;
		VulkanPhysicalDevice m_PhysicalDevice;
		VulkanLogicalDevice m_LogicalDevice;
		VulkanSwapChain m_SwapChain;
		std::vector<VulkanCommandBuffer> m_commandBuffers;
		GLFWwindow* m_Window;
	};
}