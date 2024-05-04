#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace eg {
	class VulkanPipelineDynamicStateCreateInfo {
	public:
		VulkanPipelineDynamicStateCreateInfo();
		VulkanPipelineDynamicStateCreateInfo(const VkPipelineDynamicStateCreateInfo& info);
		VulkanPipelineDynamicStateCreateInfo(const std::vector<VkDynamicState>& dynamicStates);
		~VulkanPipelineDynamicStateCreateInfo() = default;

		VkPipelineDynamicStateCreateInfo* GetDynamicStateCreateInfo() { return &m_CreateInfo; }

		void AddDynamicState(VkDynamicState dynamicState) { m_DynamicStates.push_back(dynamicState); }

		operator VkPipelineDynamicStateCreateInfo* () { return &m_CreateInfo; }
		operator VkPipelineDynamicStateCreateInfo() { return m_CreateInfo; }
		operator VkPipelineDynamicStateCreateInfo&() { return m_CreateInfo; }
		operator const VkPipelineDynamicStateCreateInfo* () const { return &m_CreateInfo; }
		operator const VkPipelineDynamicStateCreateInfo() const { return m_CreateInfo; }
		operator const VkPipelineDynamicStateCreateInfo&() const { return m_CreateInfo; }
	private:
		VkPipelineDynamicStateCreateInfo m_CreateInfo;
		std::vector<VkDynamicState> m_DynamicStates;
	};
}