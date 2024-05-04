#include "egpch.h"
#include "VulkanPipelineInputAssemblyStateCreateInfo.h"

namespace eg {

	VulkanPipelineInputAssemblyStateCreateInfo::VulkanPipelineInputAssemblyStateCreateInfo()
	{
		m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		m_createInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		m_createInfo.primitiveRestartEnable = VK_FALSE;
	}

	VulkanPipelineInputAssemblyStateCreateInfo::VulkanPipelineInputAssemblyStateCreateInfo(const VkPipelineInputAssemblyStateCreateInfo& info)
	{
		m_createInfo = info;
	}

	VulkanPipelineInputAssemblyStateCreateInfo::VulkanPipelineInputAssemblyStateCreateInfo(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable)
	{
		m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		m_createInfo.topology = topology;
		m_createInfo.primitiveRestartEnable = primitiveRestartEnable;
	}

}
