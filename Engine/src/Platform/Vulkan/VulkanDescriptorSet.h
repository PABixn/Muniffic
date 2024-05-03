#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanVertex.h"
#include "VulkanBuffer.h"
#include <GLFW/glfw3.h>

namespace eg {

	class VulkanDescriptorSets {
	public:
		VulkanDescriptorSets() = default;
		~VulkanDescriptorSets() = default;

		void Create(VkDevice logicalDevice, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, const std::vector<VulkanBuffer>& uniformBuffers);
		void Create(VkDevice logicalDevice, VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, VkSampler sampler, VkImageView imageView, const std::vector<VulkanBuffer>& uniformBuffers);
		//void updateDescriptorSet(VkDevice device, VkBuffer uniformBuffer, VkImageView imageView, VkSampler sampler, VkDeviceSize bufferSize);

		void Bind(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, uint32_t descriptorIndex);
		const std::vector<VkDescriptorSet>& getDescriptorSets() const { return m_descriptorSets; }

	private:
		std::vector<VkDescriptorSet> m_descriptorSets;
	};
}