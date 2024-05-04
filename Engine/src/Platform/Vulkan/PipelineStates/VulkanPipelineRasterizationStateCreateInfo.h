#pragma once
#include "egpch.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanPipelineRasterizationStateCreateInfo {
	public:
		VulkanPipelineRasterizationStateCreateInfo();
		VulkanPipelineRasterizationStateCreateInfo(const VkPipelineRasterizationStateCreateInfo& info);
		VulkanPipelineRasterizationStateCreateInfo(VkBool32 depthClampEnable, VkBool32 rasterizerDiscardEnable, VkPolygonMode polygonMode, VkCullModeFlags cullMode, VkFrontFace frontFace, VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth);
		~VulkanPipelineRasterizationStateCreateInfo() = default;

		VkPipelineRasterizationStateCreateInfo* GetCreateInfo() { return &m_createInfo; }

		void SetDepthClampEnable(VkBool32 depthClampEnable) { m_createInfo.depthClampEnable = depthClampEnable; }
		void SetRasterizerDiscardEnable(VkBool32 rasterizerDiscardEnable) { m_createInfo.rasterizerDiscardEnable = rasterizerDiscardEnable; }
		void SetPolygonMode(VkPolygonMode polygonMode) { m_createInfo.polygonMode = polygonMode; }
		void SetCullMode(VkCullModeFlags cullMode) { m_createInfo.cullMode = cullMode; }
		void SetFrontFace(VkFrontFace frontFace) { m_createInfo.frontFace = frontFace; }
		void SetDepthBiasEnable(VkBool32 depthBiasEnable) { m_createInfo.depthBiasEnable = depthBiasEnable; }
		void SetDepthBiasConstantFactor(float depthBiasConstantFactor) { m_createInfo.depthBiasConstantFactor = depthBiasConstantFactor; }
		void SetDepthBiasClamp(float depthBiasClamp) { m_createInfo.depthBiasClamp = depthBiasClamp; }
		void SetDepthBiasSlopeFactor(float depthBiasSlopeFactor) { m_createInfo.depthBiasSlopeFactor = depthBiasSlopeFactor; }
		void SetLineWidth(float lineWidth) { m_createInfo.lineWidth = lineWidth; }

		operator const VkPipelineRasterizationStateCreateInfo() const { return m_createInfo; }
		operator const VkPipelineRasterizationStateCreateInfo&() const { return m_createInfo; }
		operator const VkPipelineRasterizationStateCreateInfo*() const { return &m_createInfo; }
		operator VkPipelineRasterizationStateCreateInfo&()  { return m_createInfo; }
		operator VkPipelineRasterizationStateCreateInfo()  { return m_createInfo; }
		operator VkPipelineRasterizationStateCreateInfo* () { return &m_createInfo; }
	private:
		VkPipelineRasterizationStateCreateInfo m_createInfo = {};
	};
}