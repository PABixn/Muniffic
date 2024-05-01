#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanCommandPool {
	public:
		VulkanCommandPool(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);
		~VulkanCommandPool();

		VkCommandPool getCommandPool() const { return m_commandPool; }
	private:
		VkCommandPool m_commandPool;
	};
}