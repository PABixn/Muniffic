#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanDescriptorPool {
	public:
		VulkanDescriptorPool();
		~VulkanDescriptorPool();

		void createPool(VkDevice device, uint32_t maxSets, const std::vector<VkDescriptorPoolSize>& poolSizes);
		void destroyPool(VkDevice device);

		VkDescriptorPool getPool() const { return m_Pool; }

	private:
		VkDescriptorPool m_Pool;
	};
}