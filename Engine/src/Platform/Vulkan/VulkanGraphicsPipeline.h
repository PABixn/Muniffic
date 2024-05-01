#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanMultisampling.h"
#include "VulkanDescriptorSetLayout.h"
#include <GLFW/glfw3.h>
#include <filesystem>

namespace eg {

	class VulkanGraphicsPipeline {
	public:
		VulkanGraphicsPipeline(VkDevice logicalDevice, VkRenderPass renderPass,  VulkanDescriptorSetLayout layout, VulkanMultisampling multisampling, std::filesystem::path shaderPath);
		~VulkanGraphicsPipeline();

		void createGraphicsPipeline();
		void destroyGraphicsPipeline();
	private:
		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_GraphicsPipeline;
	};

}