#pragma once
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif
#include "GraphicsPipeline.h"
#include "ObjectWithoutTexturePipeline.h"
namespace eg {

	class PipelineManager {
	public:
		void CreatePipeline(const GraphicsPipelineType& type);
		GraphicsPipeline* getPipeline(const GraphicsPipelineType& type);
		void cleanUpPipeline(const GraphicsPipelineType& type);
		void cleanUpAllPipelines();
	private:
		ObjectWithoutTexturePipeline m_PureColorPipeline;
	};
}