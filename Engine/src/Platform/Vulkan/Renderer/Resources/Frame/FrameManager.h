#pragma once
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif
#include "Renderer/info.h"
namespace eg {
	class ResourceManager;
	class FrameManager
	{
	friend class Renderer;
	public:
		void init(ResourceManager* rm);
		void cleanUp();
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		void CreateSyncObjects();
		void createCommandBuffer();
		void drawFrame();
		ResourceManager* m_ResourceManagerRef;

		uint32_t m_CurrentFrameInFlightIndex = 0; // in <0, MAX_FRAMES_IN_FLIGHT)
		VkCommandBuffer m_CommandBuffers[MAX_FRAMES_IN_FLIGHT];
		VkCommandPool m_CommandPool;
	private:
		VkSemaphore m_ImageAvailableSemaphores[MAX_FRAMES_IN_FLIGHT];
		VkSemaphore m_RenderFinishedSemaphores[MAX_FRAMES_IN_FLIGHT];
		VkFence m_InFlightFences[MAX_FRAMES_IN_FLIGHT];
		

	};
}