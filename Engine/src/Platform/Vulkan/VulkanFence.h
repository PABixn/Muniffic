#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanFence{
	public:
		VulkanFence() = default;
		~VulkanFence() = default;
		void Create(VkDevice device, bool signaled);
		void Reset();
		void Wait();
		void Wait(uint64_t timeout);
		VkFence getFence() const { return m_Fence;}
	private:
		VkDevice m_Device;
		VkFence m_Fence;
	};
}