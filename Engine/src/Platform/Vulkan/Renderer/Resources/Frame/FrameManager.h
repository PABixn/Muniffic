#pragma once
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif
#include "Renderer/info.h"

typedef void* ImTextureID;
namespace eg {
	class ResourceManager;
	class FrameManager
	{
	friend class Renderer;
	public:
		ImTextureID getSceneRendererID(){ return m_SceneRenderImageRendererID[m_CurrentFrameInFlightIndex]; };
		void init(ResourceManager* rm);
		void cleanUp();
		void cleanUpFramebuffer();
		void editorRecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		void sceneRecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		void CreateSyncObjects();
		void createCommandBuffers();
		void createSceneFramebuffer(uint32_t widthArg, uint32_t heightArg, bool ImguiInit = true);
		void createRenderPass();
		void drawEditorFrame();
		void drawSceneFrame(uint32_t imageIndex);
		void initForImGui();
		void resizeViewport(const std::pair<uint32_t, uint32_t>& viewportSize);
		ResourceManager* m_ResourceManagerRef;

		

		uint32_t m_CurrentFrameInFlightIndex = 0; // in <0, MAX_FRAMES_IN_FLIGHT)
		VkCommandBuffer m_EditorCommandBuffers[MAX_FRAMES_IN_FLIGHT];
		VkCommandBuffer m_SceneCommandBuffers[MAX_FRAMES_IN_FLIGHT];
		VkCommandPool m_CommandPool;
	private:
		VkSemaphore m_EditorImageAvailableSemaphores[MAX_FRAMES_IN_FLIGHT];
		VkSemaphore m_EditorRenderFinishedSemaphores[MAX_FRAMES_IN_FLIGHT];
		VkFence m_EditorInFlightFences[MAX_FRAMES_IN_FLIGHT];
	private: //ImGuiRender Image stuff
		ImTextureID m_SceneRenderImageRendererID[MAX_FRAMES_IN_FLIGHT + 1];
		VkSampler m_SceneRenderSampler[MAX_FRAMES_IN_FLIGHT + 1];
	private:
		VkRenderPass m_SceneRenderPass;
		VkImage m_SceneRenderImages[MAX_FRAMES_IN_FLIGHT + 1];
		VkImageView m_SceneRenderImageViews[MAX_FRAMES_IN_FLIGHT + 1];
		VkFramebuffer m_SceneRenderFramebuffers[MAX_FRAMES_IN_FLIGHT + 1];
		VkDeviceMemory m_SceneRenderImageMemory[MAX_FRAMES_IN_FLIGHT + 1];

		VkSemaphore m_SceneRenderFinishedSemaphores[MAX_FRAMES_IN_FLIGHT];
		VkFence m_SceneRenderImageSafeToDelete[MAX_FRAMES_IN_FLIGHT + 1];
	private:
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;
	};
}