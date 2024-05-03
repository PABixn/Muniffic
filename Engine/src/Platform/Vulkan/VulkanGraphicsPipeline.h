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
		~VulkanGraphicsPipeline() = default;

		void Create(VkDevice logicalDevice, VkRenderPass renderPass, const std::filesystem::path& shaderPath, VulkanVertexInputLayout layout);
		void Cleanup();

		void Bind(VkCommandBuffer commandBuffer);
	private:
		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_GraphicsPipeline;
		VulkanDescriptorSetLayout m_DescriptorSetLayout;
		VulkanDescriptorSets m_DescriptorSets;
		VulkanDescriptorPool m_DescriptorPool;
		std::vector<VulkanBuffer> m_UniformBuffers;
	};

}