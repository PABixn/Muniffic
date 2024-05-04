#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanPipelineLayoutCreateInfo {
	public:
		VulkanPipelineLayoutCreateInfo();
		VulkanPipelineLayoutCreateInfo(const VkPipelineLayoutCreateInfo& info);
		VulkanPipelineLayoutCreateInfo(VkDescriptorSetLayout* setLayouts, uint32_t setLayoutCount);
		VulkanPipelineLayoutCreateInfo(VkPushConstantRange* pushConstantRanges, uint32_t pushConstantRangeCount);
		VulkanPipelineLayoutCreateInfo(VkDescriptorSetLayout* setLayouts, uint32_t setLayoutCount, VkPushConstantRange* pushConstantRanges, uint32_t pushConstantRangeCount);
		~VulkanPipelineLayoutCreateInfo() = default;
		VkPipelineLayoutCreateInfo* GetCreateInfo() { return &m_createInfo; }

		void SetFlags(VkPipelineLayoutCreateFlags flags) { m_createInfo.flags = flags; }
		void SetSetLayouts(VkDescriptorSetLayout* setLayouts, uint32_t setLayoutCount) { m_createInfo.setLayoutCount = setLayoutCount; m_createInfo.pSetLayouts = setLayouts; }
		void SetPushConstantRanges(VkPushConstantRange* pushConstantRanges, uint32_t pushConstantRangeCount) { m_createInfo.pushConstantRangeCount = pushConstantRangeCount; m_createInfo.pPushConstantRanges = pushConstantRanges; }

		operator VkPipelineLayoutCreateInfo* () { return &m_createInfo; }
		operator VkPipelineLayoutCreateInfo() { return m_createInfo; }
		operator VkPipelineLayoutCreateInfo&() { return m_createInfo; }
		operator const VkPipelineLayoutCreateInfo* () const { return &m_createInfo; }
		operator const VkPipelineLayoutCreateInfo& () const { return m_createInfo; }
		operator const VkPipelineLayoutCreateInfo () const { return m_createInfo; }
	private:
		VkPipelineLayoutCreateInfo m_createInfo;
		
	};
}