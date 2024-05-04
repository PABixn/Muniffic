#include "egpch.h"
#include "VulkanPipelineMultisampleStateCreateInfo.h"

namespace eg {
	VulkanPipelineMultisampleStateCreateInfo::VulkanPipelineMultisampleStateCreateInfo()
	{
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.sampleShadingEnable = VK_FALSE;
		createInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		createInfo.minSampleShading = 1.0f;
		createInfo.pSampleMask = nullptr;
		createInfo.alphaToCoverageEnable = VK_FALSE;
		createInfo.alphaToOneEnable = VK_FALSE;
	}

	VulkanPipelineMultisampleStateCreateInfo::VulkanPipelineMultisampleStateCreateInfo(const VkPipelineMultisampleStateCreateInfo& info)
	{
		createInfo = info;
	}

	VulkanPipelineMultisampleStateCreateInfo::VulkanPipelineMultisampleStateCreateInfo(VkSampleCountFlagBits msaaSample)
	{
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.sampleShadingEnable = VK_FALSE;
		createInfo.rasterizationSamples = msaaSample;
		createInfo.minSampleShading = 1.0f;
		createInfo.pSampleMask = nullptr;
		createInfo.alphaToCoverageEnable = VK_FALSE;
		createInfo.alphaToOneEnable = VK_FALSE;
	}

	VulkanPipelineMultisampleStateCreateInfo::VulkanPipelineMultisampleStateCreateInfo(VkBool32 sampleShadingEnable, VkSampleCountFlagBits rasterizationSamples, float minSampleShading, const VkSampleMask* pSampleMask, VkBool32 alphaToCoverageEnable, VkBool32 alphaToOneEnable)
	{
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.flags = 0;
		createInfo.sampleShadingEnable = sampleShadingEnable;
		createInfo.rasterizationSamples = rasterizationSamples;
		createInfo.minSampleShading = minSampleShading;
		createInfo.pSampleMask = pSampleMask;
		createInfo.alphaToCoverageEnable = alphaToCoverageEnable;
		createInfo.alphaToOneEnable = alphaToOneEnable;
	}
}
