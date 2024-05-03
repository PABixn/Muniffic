#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {
	//RenderPass maybe should contain VulkanSwapChain
	class VulkanRenderPass {
	public:
		VulkanRenderPass() = default;
		~VulkanRenderPass() = default;

		void Init();
		void Cleanup(VkDevice device);

		VkRenderPass GetRenderPass() const { return renderPass; }

	private:
		VkRenderPass renderPass;
	};
}
