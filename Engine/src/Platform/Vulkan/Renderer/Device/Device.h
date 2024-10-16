#pragma once

#include "Platform/Vulkan/Renderer/Queue/Queue.h"
#include <vector>
#include <memory>
#include <iostream>
#include <optional>

#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif // !RENDERER_INCLUDE_GLFW_VULKAN

namespace eg{
#ifndef NDEBUG
	const bool enableValidationLayers = true;
#else 
	const bool enableValidationLayers = false;
#endif

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

uint32_t RateDevice(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

class Device
{
	friend class SwapChain;
	friend class Shader;
	friend class Renderer;
public:
	Device() : m_VulkanInstance(), m_Surface(), m_PhysicalDevice(), m_LogicalDevice() {};
	~Device() = default;
	void init(GLFWwindow* glfwWindow);
	void cleanUp();
	VkInstance* getVulkanInstance() { return &m_VulkanInstance; }
	VkDevice& getNativeDevice() { return m_LogicalDevice; };
private:
	void createVulkanInstance();
	void setupDebugMessanger();
	bool checkValidationLayerSupport() const;
	std::vector<const char*> getRequiredExtensions() const;
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateSurface(GLFWwindow* nativeWindow);

public:
	const std::vector<const char*> m_ValidationLayers = { 
		"VK_LAYER_KHRONOS_validation"
	};
	std::unique_ptr<VkDebugUtilsMessengerEXT> m_DebugMessenger = std::make_unique<VkDebugUtilsMessengerEXT>();
	VkInstance m_VulkanInstance;
	VkPhysicalDevice m_PhysicalDevice;
	VkDevice m_LogicalDevice;
	VkSurfaceKHR m_Surface;

	Queue m_GraphicsQueue;
	Queue m_PresentQueue;
private:
private:
	VKAPI_ATTR static VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
		EG_TRACE("Vulkan validation layer: {0}", pCallbackData->pMessage);
		//std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	};

};
}