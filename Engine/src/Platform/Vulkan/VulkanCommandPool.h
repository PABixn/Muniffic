#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanCommandPool {
	public:
		VulkanCommandPool() = default;
		~VulkanCommandPool() = default;

		void Create(VkCommandPoolCreateFlags flags = 0);
		void Cleanup();

		VkCommandPool GetPool() const { return m_commandPool; }
	private:
		VkCommandPool m_commandPool;
	};
}