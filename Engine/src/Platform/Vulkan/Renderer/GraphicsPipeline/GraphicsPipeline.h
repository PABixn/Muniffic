#pragma once
#include "Platform/Vulkan/Renderer/Shader/Shader.h"
namespace eg {
	class Device;
	class SwapChain;
	class Renderer;
	class GraphicsPipeline
	{
	public:
		friend class SwapChain;
		friend class Renderer;
		void init();
		void cleanUp();
		VkPipeline& getNativePipeline() { return m_NativePipeline; };
		VkPipelineLayout& getPipelineLayout() { return m_PipelineLayout; }
	private:
		VkPipeline m_NativePipeline;
		VkPipelineLayout m_PipelineLayout;

		VulkanShader m_VertexShader;
		VulkanShader m_FragmentShader;
	};
}