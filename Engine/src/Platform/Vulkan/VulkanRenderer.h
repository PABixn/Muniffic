#pragma once
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif // !RENDERER_INCLUDE_GLFW_VULKAN
#include "Platform/Vulkan/Renderer/Device/Device.h"
#include "Platform/Vulkan/Renderer/SwapChain/SwapChain.h"
#include "Platform/Vulkan/Renderer/Resources/ResourceManager.h"
#include "Platform/Vulkan/Renderer/GraphicsPipeline/GraphicsPipeline.h"
#include "entt.hpp"
#include "Platform/Vulkan/Renderer/Resources/Scene/SceneRenderData.h"
namespace eg {
	class Scene;
	class VulkanRenderer
	{
		friend class VRenderer;
		// TO-DO Vulkan
		static VulkanRenderer ren;
	public:
		static VulkanRenderer& get() { return ren; };
	public:
		void init();
		void cleanUp();
		void render();
		Device& getDevice() { return m_Device; }
		VkDevice& getNativeDevice() { return m_Device.m_LogicalDevice; }
		VkRenderPass& getEditorRenderPass() { return m_EditorRenderPass; }
		ResourceManager& getResourceManager() { return m_ResourceManager; }
		SwapChain& getSwapChain() { return m_SwapChain; }
		GraphicsPipeline& getGraphicsPipeline() { return m_GraphicsPipeline; }
		VkDescriptorPool& getImGuiDescriptorPool() { return m_ImGuiDescriptorPool; }
		const VkFormat& getFrameBufferImageFormat() { return m_SwapChain.m_ImageFormat; }
		SceneRenderData& getSceneRenderData() { return m_CurrentSceneRenderData; }
		void LoadSceneData(Scene* scene);
	private:
		Device m_Device;
		SwapChain m_SwapChain;
		VkRenderPass m_EditorRenderPass;
		ResourceManager m_ResourceManager;
		VkCommandPool m_PoolForOneTimeOperations;
		GraphicsPipeline m_GraphicsPipeline;
	private:
		VkDescriptorPool m_ImGuiDescriptorPool;
	private:
		void createCommandPoolForOneTimeOperations();
		void createRenderPass();
		void initImGui(GLFWwindow* window);
		SceneRenderData m_CurrentSceneRenderData;
	public:
		VkCommandBuffer BeginSingleTimeCommands(); // Graphics queue family
		void EndSingleTimeCommands(VkCommandBuffer& cb); // Submit to graphics queue
	};
	class VRen {
	private:
		static VulkanRenderer vren;
	public:
		static VulkanRenderer& get() { return vren; }
	};
}