#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanMultisampling.h"
#include "VulkanDescriptorSetLayout.h"
#include "VulkanDescriptorSet.h"
#include "VulkanDescriptorPool.h"
#include "VulkanBuffer.h"
#include <GLFW/glfw3.h>
#include <filesystem>

namespace eg {

	class VulkanGraphicsPipeline {
	public:
		//VulkanGraphicsPipeline(VkDevice logicalDevice, VkRenderPass renderPass,  VulkanDescriptorSetLayout layout, VulkanMultisampling multisampling, std::filesystem::path shaderPath);
		VulkanGraphicsPipeline() = default;
		VulkanGraphicsPipeline(const VkPipelineLayout& layout, const VkPipeline& pipeline, const VkDevice& device) : m_PipelineLayout(layout), m_GraphicsPipeline(pipeline), m_LogicalDevice(device) {}
		~VulkanGraphicsPipeline() = default;

		void Cleanup();

		void Bind(VkCommandBuffer commandBuffer);
		const VkPipelineLayout& GetPipelineLayout() const { return m_PipelineLayout; }
		const VkPipeline& GetPipeline() const { return m_GraphicsPipeline; }

	private:
		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_GraphicsPipeline;
		VkDevice m_LogicalDevice;
	};

}