#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanPipelineDepthStencilStateCreateInfo {
	public:
		VulkanPipelineDepthStencilStateCreateInfo();
		VulkanPipelineDepthStencilStateCreateInfo(const VkPipelineDepthStencilStateCreateInfo& info);
		VulkanPipelineDepthStencilStateCreateInfo(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, VkCompareOp depthCompareOp, VkBool32 depthBoundsTestEnable, VkBool32 stencilTestEnable, VkStencilOpState front, VkStencilOpState back, float minDepthBounds, float maxDepthBounds);
		~VulkanPipelineDepthStencilStateCreateInfo() = default;

		VkPipelineDepthStencilStateCreateInfo* GetVulkanPipelineDepthStencilStateCreateInfo() { return &m_CreateInfo; }

		void SetDepthTestEnable(VkBool32 depthTestEnable) { m_CreateInfo.depthTestEnable = depthTestEnable; }
		void SetDepthWriteEnable(VkBool32 depthWriteEnable) { m_CreateInfo.depthWriteEnable = depthWriteEnable; }
		void SetDepthCompareOp(VkCompareOp depthCompareOp) { m_CreateInfo.depthCompareOp = depthCompareOp; }
		void SetDepthBoundsTestEnable(VkBool32 depthBoundsTestEnable) { m_CreateInfo.depthBoundsTestEnable = depthBoundsTestEnable; }
		void SetStencilTestEnable(VkBool32 stencilTestEnable) { m_CreateInfo.stencilTestEnable = stencilTestEnable; }
		void SetFront(VkStencilOpState front) { m_CreateInfo.front = front; }
		void SetBack(VkStencilOpState back) { m_CreateInfo.back = back; }
		void SetMinDepthBounds(float minDepthBounds) { m_CreateInfo.minDepthBounds = minDepthBounds; }
		void SetMaxDepthBounds(float maxDepthBounds) { m_CreateInfo.maxDepthBounds = maxDepthBounds; }

		operator VkPipelineDepthStencilStateCreateInfo* () { return &m_CreateInfo; }
		operator VkPipelineDepthStencilStateCreateInfo() { return m_CreateInfo; }
		operator VkPipelineDepthStencilStateCreateInfo&() { return m_CreateInfo; }
		operator const VkPipelineDepthStencilStateCreateInfo* () const { return &m_CreateInfo; }
		operator const VkPipelineDepthStencilStateCreateInfo() const { return m_CreateInfo; }
		operator const VkPipelineDepthStencilStateCreateInfo&() const { return m_CreateInfo; }
	private:
		VkPipelineDepthStencilStateCreateInfo m_CreateInfo = {};
	};
}