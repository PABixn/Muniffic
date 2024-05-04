#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanPipelineTesselationStateCreateInfo {
	public:
		VulkanPipelineTesselationStateCreateInfo();
		VulkanPipelineTesselationStateCreateInfo(VkPipelineTessellationStateCreateFlags flags, uint32_t patchControlPoints);
		~VulkanPipelineTesselationStateCreateInfo() = default;

		VkPipelineTessellationStateCreateInfo* GetCreateInfo() { return &m_createInfo; }

		void SetFlags(VkPipelineTessellationStateCreateFlags flags) { m_createInfo.flags = flags; }
		void SetPatchControlPoints(uint32_t patchControlPoints) { m_createInfo.patchControlPoints = patchControlPoints; }

		operator VkPipelineTessellationStateCreateInfo* () { return &m_createInfo; }
		operator VkPipelineTessellationStateCreateInfo() { return m_createInfo; }
		operator VkPipelineTessellationStateCreateInfo&() { return m_createInfo; }
		operator const VkPipelineTessellationStateCreateInfo* () const { return &m_createInfo; }
		operator const VkPipelineTessellationStateCreateInfo() const { return m_createInfo; }
		operator const VkPipelineTessellationStateCreateInfo& () const { return m_createInfo; }
	private:
		VkPipelineTessellationStateCreateInfo m_createInfo = {};
	};
}