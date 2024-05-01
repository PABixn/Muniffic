#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanRenderPass {
	public:
		VulkanRenderPass(VkDevice device, VkFormat swapChainImageFormat, VkSampleCountFlagBits msaaSamples);
		~VulkanRenderPass();

		void cleanup(VkDevice device);

		VkRenderPass getRenderPass() const { return renderPass; }

	private:
		VkRenderPass renderPass;
	};
}
