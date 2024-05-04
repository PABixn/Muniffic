#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanPipelineMultisampleStateCreateInfo {
	public:
		VulkanPipelineMultisampleStateCreateInfo();
		VulkanPipelineMultisampleStateCreateInfo(const VkPipelineMultisampleStateCreateInfo& info);
		VulkanPipelineMultisampleStateCreateInfo(VkSampleCountFlagBits msaaSample);
		VulkanPipelineMultisampleStateCreateInfo(VkBool32 sampleShadingEnable, VkSampleCountFlagBits rasterizationSamples, float minSampleShading, const VkSampleMask* pSampleMask, VkBool32 alphaToCoverageEnable, VkBool32 alphaToOneEnable);
		~VulkanPipelineMultisampleStateCreateInfo() = default;

		VkPipelineMultisampleStateCreateInfo* getCreateInfo() { return &createInfo; }

		void setSampleShadingEnable(VkBool32 sampleShadingEnable) { createInfo.sampleShadingEnable = sampleShadingEnable; }
		void setRasterizationSamples(VkSampleCountFlagBits rasterizationSamples) { createInfo.rasterizationSamples = rasterizationSamples; }
		void setMinSampleShading(float minSampleShading) { createInfo.minSampleShading = minSampleShading; }
		void setPSampleMask(const VkSampleMask* pSampleMask) { createInfo.pSampleMask = pSampleMask; }
		void setAlphaToCoverageEnable(VkBool32 alphaToCoverageEnable) { createInfo.alphaToCoverageEnable = alphaToCoverageEnable; }
		void setAlphaToOneEnable(VkBool32 alphaToOneEnable) { createInfo.alphaToOneEnable = alphaToOneEnable; }

		operator VkPipelineMultisampleStateCreateInfo* () { return &createInfo; }
		operator VkPipelineMultisampleStateCreateInfo() { return createInfo; }
		operator VkPipelineMultisampleStateCreateInfo&() { return createInfo; }
		operator const VkPipelineMultisampleStateCreateInfo* () const { return &createInfo; }
		operator const VkPipelineMultisampleStateCreateInfo& () const { return createInfo; }
		operator const VkPipelineMultisampleStateCreateInfo () const { return createInfo; }
	private:
		VkPipelineMultisampleStateCreateInfo createInfo = {};
	};
}