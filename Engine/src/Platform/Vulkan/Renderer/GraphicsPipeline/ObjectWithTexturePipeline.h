#pragma once
#include "Platform/Vulkan/Renderer/Shader/Shader.h"
#include "GraphicsPipeline.h"
namespace eg {
	class Device;
	class SwapChain;
	class Renderer;
	class ObjectWithTexturePipeline : public GraphicsPipeline
	{
	public:
		friend class SwapChain;
		friend class Renderer;
		void init() override;
		void cleanUp() override;
		void onViewportResize(VkCommandBuffer& cmdBfr, const VkExtent2D& extentArg) override;
		VkPipeline& getNativePipeline() override { return m_NativePipeline; };
		VkPipelineLayout& getPipelineLayout() override { return m_PipelineLayout; }
		GraphicsPipelineType getType() override { return GraphicsPipelineType::Type_BasicMeshWithTexture; }
	private:
		VkPipeline m_NativePipeline;
		VkPipelineLayout m_PipelineLayout;

		VulkanShader m_VertexShader;
		VulkanShader m_FragmentShader;

	};
}