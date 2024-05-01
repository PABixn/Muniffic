#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {

	class VulkanCommandBuffer {
	public:
		VulkanCommandBuffer BeginSingleTimeCommands(VkDevice logicalDevice,  VkCommandPool commandPool);
		void EndSingleTimeCommands(VkDevice logicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool);

		VkCommandBuffer getCommandBuffer() const { return m_commandBuffer; }
	private:
		VkCommandBuffer m_commandBuffer;
	};

}