#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanPipelineViewportStateCreateInfo {
	public:
		VulkanPipelineViewportStateCreateInfo();
		VulkanPipelineViewportStateCreateInfo(const VkPipelineViewportStateCreateInfo& info);
		VulkanPipelineViewportStateCreateInfo(uint32_t viewportCount, uint32_t scissorCount, VkViewport* viewports, VkRect2D* scissors);
		~VulkanPipelineViewportStateCreateInfo() = default;

		VkPipelineViewportStateCreateInfo* GetVulkanPipelineViewportStateCreateInfo() { return &m_CreateInfo; }

		void SetViewportCount(uint32_t viewportCount) { m_CreateInfo.viewportCount = viewportCount; }
		void SetScissorCount(uint32_t scissorCount) { m_CreateInfo.scissorCount = scissorCount; }
		void SetViewports(VkViewport* viewports) { m_CreateInfo.pViewports = viewports; }
		void SetScissors(VkRect2D* scissors) { m_CreateInfo.pScissors = scissors; }

		operator VkPipelineViewportStateCreateInfo* () { return &m_CreateInfo; }
		operator VkPipelineViewportStateCreateInfo& () { return m_CreateInfo; }
		operator VkPipelineViewportStateCreateInfo () { return m_CreateInfo; }
		operator const VkPipelineViewportStateCreateInfo* () const { return &m_CreateInfo; }
		operator const VkPipelineViewportStateCreateInfo& () const { return m_CreateInfo; }
		operator const VkPipelineViewportStateCreateInfo () const { return m_CreateInfo; }
	private:
		VkPipelineViewportStateCreateInfo m_CreateInfo = {};
	};
}