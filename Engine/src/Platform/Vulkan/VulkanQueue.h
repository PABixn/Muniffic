#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	class VulkanQueue {
	public:
		VulkanQueue();
		~VulkanQueue();
		void CreateQueue(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);
		void Cleanup();
		const VkQueue& GetQueue() { return m_Queue; }

		static QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
	private:
		VkQueue m_Queue;
	};
}