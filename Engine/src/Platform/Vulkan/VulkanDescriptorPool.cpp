#include "egpch.h"
#include "VulkanDescriptorPool.h"
#include "VulkanConstants.h"
namespace eg {
	void VulkanDescriptorPool::Create(VkDevice logicalDevice)
	{
		std::array<VkDescriptorPoolSize, 2> poolSizes{};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(VulkanConstants::MAX_FRAMES_IN_FLIGHT);
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(VulkanConstants::MAX_FRAMES_IN_FLIGHT);

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(VulkanConstants::MAX_FRAMES_IN_FLIGHT);

		if (vkCreateDescriptorPool(logicalDevice, &poolInfo, nullptr, &m_Pool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}
	}

	void VulkanDescriptorPool::Cleanup(VkDevice logicalDevice)
	{
		vkDestroyDescriptorPool(logicalDevice, m_Pool, nullptr);
	}
}
