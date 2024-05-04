#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanDescriptorSetLayout {
	public:
		VulkanDescriptorSetLayout() = default;
		~VulkanDescriptorSetLayout() = default;

		//Write an abstract class for VulkanDescriptorSetLayout creation and cleanup
		void Create();
		void Cleanup();

		const VkDescriptorSetLayout& GetDescriptorSetLayout() const { return m_descriptorSetLayout; }
	public:
		VkDescriptorSetLayout m_descriptorSetLayout;
	};
}