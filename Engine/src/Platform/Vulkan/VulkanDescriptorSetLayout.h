#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanDescriptorSetLayout {
	public:
		VulkanDescriptorSetLayout();
		~VulkanDescriptorSetLayout();

		void createDescriptorSetLayout(VkDevice device);
		void destroyDescriptorSetLayout(VkDevice device);

		VkDescriptorSetLayout getDescriptorSetLayout() const { return m_descriptorSetLayout; }
	public:
		VkDescriptorSetLayout m_descriptorSetLayout;
	};
}