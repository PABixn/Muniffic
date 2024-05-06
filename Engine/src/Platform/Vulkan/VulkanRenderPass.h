#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "GraphicsPipelineFactory.h"
#include <GLFW/glfw3.h>

namespace eg {
	//RenderPass maybe should contain VulkanSwapChain
	class VulkanRenderPass {
	public:
		VulkanRenderPass() = default;
		~VulkanRenderPass() = default;

		void Init();
		void InitPipelineBuilder(VkDevice device);
		void Cleanup(VkDevice device);

		VkRenderPass& GetRenderPass()  { return renderPass; }
		const Ref<VulkanGraphicsPipelineBuilder>& GetPipelineBuilder() const { return pipelineBuilder; }

		operator VkRenderPass& () { return renderPass; }
		operator const VkRenderPass& () const { return renderPass; }
		operator Ref<VulkanGraphicsPipelineBuilder>& () { return pipelineBuilder; }
		operator const Ref<VulkanGraphicsPipelineBuilder>& () const { return pipelineBuilder; }
	private:
		VkRenderPass renderPass;
		Ref<VulkanGraphicsPipelineBuilder> pipelineBuilder;
	};
}
