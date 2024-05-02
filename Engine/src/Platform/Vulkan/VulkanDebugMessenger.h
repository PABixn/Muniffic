#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {

	class VulkanDebugMessenger {
	public:
		VulkanDebugMessenger() = default;
		~VulkanDebugMessenger() = default;
		void Init(const VkInstance& instance);
		static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		void Cleanup(const VkInstance& instance);
	private:
		void CreateDebugUtilsMessengerEXT(const VkInstance& instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo);
		void DestroyDebugUtilsMessengerEXT(const VkInstance& instance);
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);
	private:
		VkDebugUtilsMessengerEXT m_DebugMessenger;
	};

}