#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanSemaphore {
	public:
		VulkanSemaphore(VkDevice device);
		~VulkanSemaphore();

		VkSemaphore getSemaphore() const { return m_Semaphore; }
	private:
		VkSemaphore m_Semaphore;
		VkDevice m_Device;
	};
}