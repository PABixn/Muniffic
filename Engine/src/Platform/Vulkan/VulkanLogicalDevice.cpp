#include "egpch.h"
#include "VulkanLogicalDevice.h"

#include "VulkanValidationLayers.h"
#include "set"
namespace eg {
	VulkanLogicalDevice::~VulkanLogicalDevice()
	{
		Cleanup();
	}

	void VulkanLogicalDevice::Init(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, VulkanDeviceExtensions& deviceExtensions)
	{
		VulkanQueueFamilyIndices indices = VulkanQueueFamilyIndices::FindQueueFamilies(physicalDevice, surface);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };
		
		float queuePriority = 1.0f;
		
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}
		
		VkPhysicalDeviceFeatures deviceFeatures{};
		deviceFeatures.samplerAnisotropy = VK_TRUE;
		
		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = queueCreateInfos.data();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		
		createInfo.pEnabledFeatures = &deviceFeatures;
		
		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.GetDeviceExtensions().data();
		
		if (VulkanValidationLayers::GetInstance().ValidationLayersEnabled())
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanValidationLayers::GetInstance().size());
			createInfo.ppEnabledLayerNames = VulkanValidationLayers::GetInstance().GetValidationLayers().data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}
		
		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_Device) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create logical device");
		}
		
		vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue.GetQueueRef());
		vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_PresentQueue.GetQueueRef());

	}

	void VulkanLogicalDevice::Cleanup()
	{
		vkDestroyDevice(m_Device, nullptr);
	}
}
