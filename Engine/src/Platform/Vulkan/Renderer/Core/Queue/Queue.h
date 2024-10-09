#pragma once
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif // !RENDERER_INCLUDE_GLFW_VULKAN
#include <optional>
#include <vector>
#include <memory>
#include <iostream>

namespace eg {
	class Renderer;
	void showQueueFamilies(VkPhysicalDevice physicalDevice);

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		bool isComplete() const {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
		const std::vector<uint32_t> getUniqueQueueFamilyIndices() {
			if (!isComplete())
			{
				throw std::runtime_error("Queue families are not set! :(");
			}
			if (graphicsFamily.value() == presentFamily.value())
			{
				return std::vector<uint32_t>({ graphicsFamily.value() });
			}
			return std::vector<uint32_t>({ graphicsFamily.value(), presentFamily.value() });
		}
	};

	class Queue
	{
		friend class Renderer;
	public:
		static QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
		static QueueFamilyIndices GetQueueFamiliesIndices(const VkPhysicalDevice& device, const VkSurfaceKHR& surface, const bool& shouldBeSaved = false);
		static std::vector<float>* PopulateQueuesCreationInfo(std::vector<VkDeviceQueueCreateInfo>& InfoToPopulate);

	private:
		static QueueFamilyIndices m_QueueFamilyIndices;
	public:
		Queue() = default;
		~Queue() = default;

		void submitToQueue(VkCommandBuffer& commandBuffer);
		std::shared_ptr <VkQueue> GetNativeQueue() { return m_VulkanQueue; }
	private:
		std::shared_ptr<VkQueue> m_VulkanQueue = std::make_shared<VkQueue>();
	};
}