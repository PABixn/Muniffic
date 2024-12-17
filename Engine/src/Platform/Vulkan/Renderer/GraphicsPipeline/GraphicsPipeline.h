#pragma once
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif
namespace eg {
	enum GraphicsPipelineType {
		Type_BasicMeshWithColor = 0,
		Type_BasicMeshWithTexture = 0
	};
	class GraphicsPipeline {
	public:
		virtual VkPipeline& getNativePipeline() = 0;
		virtual VkPipelineLayout& getPipelineLayout() = 0;
		virtual GraphicsPipelineType getType() = 0;
		virtual void init() = 0;
		virtual void cleanUp() = 0;
		virtual void onViewportResize(VkCommandBuffer& cmdBfr, const VkExtent2D& extentArg) = 0;
	};
	void defineInputAssembly(VkPipelineInputAssemblyStateCreateInfo& info);
	void defineRasterizer(VkPipelineRasterizationStateCreateInfo& info);
	void defineMultisampling(VkPipelineMultisampleStateCreateInfo& info);
	void defineColorBlend(VkPipelineColorBlendAttachmentState& info, VkPipelineColorBlendStateCreateInfo& info1);
	void defineViewportState(VkPipelineViewportStateCreateInfo& inf, VkViewport& viewport, VkRect2D& scissoro);
	void defineDynamicStates(VkPipelineDynamicStateCreateInfo& info);
}