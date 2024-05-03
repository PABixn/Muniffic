#include "egpch.h"
#include "VulkanDescriptorSetLayout.h"
#include "VulkanUtils.h"
namespace eg {

	void VulkanDescriptorSetLayout::Init()
	{
		VulkanHandler* handler = GetVulkanHandler();
		VkDevice device = handler->GetVulkanLogicalDevice().GetDevice();
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_descriptorSetLayout) != VK_SUCCESS) {
			EG_CORE_ERROR("Failed to create descriptor set layout!");
		}
	}

	void VulkanDescriptorSetLayout::Cleanup()
	{
		VulkanHandler* handler = GetVulkanHandler();
		VkDevice device = handler->GetVulkanLogicalDevice().GetDevice();
		vkDestroyDescriptorSetLayout(device, m_descriptorSetLayout, nullptr);
	}

}