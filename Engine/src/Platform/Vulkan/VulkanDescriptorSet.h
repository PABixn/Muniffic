#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {

	class VulkanDescriptorSet {
	public:
		VulkanDescriptorSet(VkDevice device, VkDescriptorSetLayout layout, VkDescriptorPool pool);
		~VulkanDescriptorSet();

		void updateDescriptorSet(VkDevice device, VkBuffer uniformBuffer, VkImageView imageView, VkSampler sampler, VkDeviceSize bufferSize);
		VkDescriptorSet getDescriptorSet() const { return m_descriptorSet; }

	private:
		VkDescriptorSet m_descriptorSet;
	};
}