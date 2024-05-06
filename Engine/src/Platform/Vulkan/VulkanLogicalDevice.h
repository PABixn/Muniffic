#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include "VulkanQueue.h"
#include "VulkanDeviceExtensions.h"
#include <GLFW/glfw3.h>

namespace eg {

	class VulkanLogicalDevice {
	public:
		VulkanLogicalDevice() = default;
		~VulkanLogicalDevice();

		void Init(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, VulkanDeviceExtensions& extensions);
		void Cleanup();


		const VkDevice& GetDevice() const { return m_Device; }
		const VulkanQueue& GetVulkanGraphicsQueue() const  { return m_GraphicsQueue; }
		const VkQueue& GetGraphicsQueue() const { return m_GraphicsQueue.GetQueue(); }
		const VulkanQueue& GetVulkanPresentQueue() const { return m_PresentQueue; }
		const VkQueue& GetPresentQueue() const { return m_PresentQueue.GetQueue(); }
		
		operator VkDevice*() { return &m_Device; }
		operator VkDevice&() { return m_Device; }
		operator const VkDevice*() const { return &m_Device; }
		operator const VkDevice&() const { return m_Device; }
	private:
		VkDevice m_Device;
		VulkanQueue m_GraphicsQueue;
		VulkanQueue m_PresentQueue;
	};

}