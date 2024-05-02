#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanDeviceExtensions.h"
#include "VulkanDebugMessenger.h"
#include <GLFW/glfw3.h>

#include "vector"

namespace eg{

	class VulkanInstance {
	public:
		VulkanInstance() = default;
		~VulkanInstance();

		void Init(VulkanDeviceExtensions& extensions);
		void Cleanup();
		
		

		const VkInstance& GetInstance() { return m_Instance; }
	private:
		
	private:
		VkInstance m_Instance;
		VulkanDebugMessenger m_DebugMessenger;
	};
}