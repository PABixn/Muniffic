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
#include "VulkanSemaphore.h"
#include "VulkanFence.h"

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
		void StartFrame();
		void DrawFrame();
		void Init();
		void Cleanup();

		void CreateCommandBuffers();
		void CreateSyncObjects();
		
		void RecordCommandBuffer(VkCommandBuffer& commandBuffer, uint32_t imageIndex);
		VkCommandBuffer GetCurrentCommandBuffer() {return m_commandBuffers[m_CurrentFrame].getCommandBuffer(); }

		const VulkanInstance& GetVulkanInstance() { return m_Instance; }
		const VulkanDeviceExtensions& GetVulkanDeviceExtensions() { return m_DeviceExtensions; }
		const VulkanSurface& GetVulkanSurface() { return m_Surface; }
		const VulkanPhysicalDevice& GetVulkanPhysicalDevice() { return m_PhysicalDevice; }
		const VulkanLogicalDevice& GetVulkanLogicalDevice() { return m_LogicalDevice; }
		const VulkanSwapChain& GetVulkanSwapChain() { return m_SwapChain; }
		const std::vector<VulkanCommandBuffer>& GetVulkanCommandBuffers() { return m_commandBuffers; }
		const VulkanRenderPass& GetMainRenderPass() { return m_MainRenderPass; }
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
		VulkanCommandPool m_CommandPool;
		std::vector<VulkanCommandBuffer> m_commandBuffers;
		std::vector<VulkanSemaphore> m_ImageAvailableSemaphores;
		std::vector<VulkanSemaphore> m_RenderFinishedSemaphores;
		std::vector<VulkanFence> m_InFlightFences;
		GLFWwindow* m_Window;
		uint32_t m_CurrentFrame = 0;
		uint32_t m_NextImageIndex = 0;
	};
}