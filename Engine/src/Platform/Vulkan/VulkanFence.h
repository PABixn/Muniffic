#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanFence{
	public:
		VulkanFence() = default;
		VulkanFence(VkDevice device, bool signaled = false);
		~VulkanFence() = default;
		void Reset();
		void Wait();
		void Wait(uint64_t timeout);
		VkFence& GetFence() { return m_Fence;}

		operator VkFence&() { return m_Fence; }
		operator const VkFence&() const { return m_Fence; }
		operator VkFence*() { return &m_Fence; }
		operator const VkFence*() const { return &m_Fence; }
	private:
		VkDevice m_Device;
		VkFence m_Fence;
	};
}