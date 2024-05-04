#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanPipelineColorBlendAttachmentState.h"
namespace eg {
	class VulkanPipelineColorBlendCreateInfo {
	public:
		VulkanPipelineColorBlendCreateInfo();
		VulkanPipelineColorBlendCreateInfo(const VkPipelineColorBlendStateCreateInfo& info);
		VulkanPipelineColorBlendCreateInfo(const VulkanPipelineColorBlendAttachmentState& attachmentState);
		VulkanPipelineColorBlendCreateInfo(const VulkanPipelineColorBlendAttachmentState& attachmentState, VkBool32 logicOpEnable = VK_FALSE, VkLogicOp logicOp = VK_LOGIC_OP_COPY, float blendConstants[4] = {});
		~VulkanPipelineColorBlendCreateInfo() = default;

		VkPipelineColorBlendStateCreateInfo* GetCreateInfo() { return &m_createInfo; }

		void SetFlags(VkPipelineColorBlendStateCreateFlags flags) { m_createInfo.flags = flags; }
		void SetLogicOpEnable(VkBool32 logicOpEnable) { m_createInfo.logicOpEnable = logicOpEnable; }
		void SetLogicOp(VkLogicOp logicOp) { m_createInfo.logicOp = logicOp; }
		void SetAttachmentCount(uint32_t attachmentCount) { m_createInfo.attachmentCount = attachmentCount; }
		void SetAttachments(const VkPipelineColorBlendAttachmentState* attachments) { m_createInfo.pAttachments = attachments; }
		void SetBlendConstants(float blendConstants[4]) { m_createInfo.blendConstants[0] = blendConstants[0]; m_createInfo.blendConstants[1] = blendConstants[1]; m_createInfo.blendConstants[2] = blendConstants[2]; m_createInfo.blendConstants[3] = blendConstants[3]; }

		operator VkPipelineColorBlendStateCreateInfo* () { return &m_createInfo; }
		operator VkPipelineColorBlendStateCreateInfo() { return m_createInfo; }
		operator VkPipelineColorBlendStateCreateInfo&() { return m_createInfo; }
		operator const VkPipelineColorBlendStateCreateInfo* () const { return &m_createInfo; }
		operator const VkPipelineColorBlendStateCreateInfo() const { return m_createInfo; }
		operator const VkPipelineColorBlendStateCreateInfo&() const { return m_createInfo; }
	private:
		VkPipelineColorBlendStateCreateInfo m_createInfo;
	};
}