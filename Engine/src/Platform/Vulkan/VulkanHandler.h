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
#include "VulkanRenderPass.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanCommandPool.h"

#include "VulkanDeviceMemoryManager.h"
#include "VulkanConstants.h"
#include <GLFW/glfw3.h>



namespace eg {
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
		const VulkanRenderPass& GetMainRenderPass() { return m_MainRenderPass; }
		const VulkanGraphicsPipeline& GetVulkanGraphicsPipeline() { return m_GraphicsPipeline; }
		const VulkanCommandPool& GetVulkanCommandPool() { return m_CommandPool; }

		GLFWwindow* GetWindow() { return m_Window; }
	private:
		

	private:
		VulkanInstance m_Instance;
		VulkanDeviceExtensions m_DeviceExtensions;
		VulkanSurface m_Surface;
		VulkanPhysicalDevice m_PhysicalDevice;
		VulkanLogicalDevice m_LogicalDevice;
		VulkanSwapChain m_SwapChain;
		VulkanRenderPass m_MainRenderPass;
		VulkanGraphicsPipeline m_GraphicsPipeline;
		VulkanCommandPool m_CommandPool;
		std::vector<VulkanCommandBuffer> m_commandBuffers;
		GLFWwindow* m_Window;
	};
}