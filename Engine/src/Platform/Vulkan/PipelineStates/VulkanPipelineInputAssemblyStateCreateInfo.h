#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanPipelineInputAssemblyStateCreateInfo {
	public:
		VulkanPipelineInputAssemblyStateCreateInfo();
		VulkanPipelineInputAssemblyStateCreateInfo(const VkPipelineInputAssemblyStateCreateInfo& info);
		VulkanPipelineInputAssemblyStateCreateInfo(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable = VK_FALSE);
		~VulkanPipelineInputAssemblyStateCreateInfo() = default;

		VkPipelineInputAssemblyStateCreateInfo* getCreateInfo() { return &m_createInfo; }

		void setTopology(VkPrimitiveTopology topology) { m_createInfo.topology = topology; }
		void setPrimitiveRestartEnable(VkBool32 primitiveRestartEnable) { m_createInfo.primitiveRestartEnable = primitiveRestartEnable; }

		operator VkPipelineInputAssemblyStateCreateInfo& () { return m_createInfo; }
		operator VkPipelineInputAssemblyStateCreateInfo () { return m_createInfo; }
		operator VkPipelineInputAssemblyStateCreateInfo* () { return &m_createInfo; }
		operator const VkPipelineInputAssemblyStateCreateInfo& () const { return m_createInfo; }
		operator const VkPipelineInputAssemblyStateCreateInfo* () const { return &m_createInfo; }
		operator const VkPipelineInputAssemblyStateCreateInfo () const { return m_createInfo; }
	private:
		VkPipelineInputAssemblyStateCreateInfo m_createInfo = {};
	};
}