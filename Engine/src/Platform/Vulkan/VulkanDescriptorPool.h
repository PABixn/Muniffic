#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanDescriptorPool {
	public:
		VulkanDescriptorPool() = default;
		~VulkanDescriptorPool() = default;

		void Create(VkDevice logicalDevice);
		void Cleanup(VkDevice logicalDevice);

		VkDescriptorPool getPool() const { return m_Pool; }

	private:
		VkDescriptorPool m_Pool;
	};
}