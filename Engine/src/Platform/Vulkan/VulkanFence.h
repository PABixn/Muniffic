#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanFence{
	public:
		VulkanFence(VkDevice device, bool signaled = false);
		~VulkanFence();
		void reset();
		void wait();
		void wait(uint64_t timeout);
		VkFence getFence() const { return m_fence;}
	private:
		VkDevice m_device;
		VkFence m_fence;
	};
}