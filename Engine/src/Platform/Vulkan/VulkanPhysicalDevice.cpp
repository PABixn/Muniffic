#include "egpch.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanQueue.h"
namespace eg {
	VulkanPhysicalDevice::~VulkanPhysicalDevice()
	{
		Cleanup();
	}

	void VulkanPhysicalDevice::Init(const VkInstance& instance, const VkSurfaceKHR& surface, VulkanDeviceExtensions& extensions)
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
		
		if (deviceCount == 0) {
			EG_ASSERT("Failed to find GPUs with Vulkan support!");
		}
		
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
		
		int bestDeviceIndex = -1;
		int bestScore = 0;
		for (int i = 0; i < deviceCount; i++) {
			int score = RateDeviceSuitability(devices[i], surface);
			if (score > bestScore && IsDeviceSuitable(devices[i], surface, extensions)) {
				bestScore = score;
				bestDeviceIndex = i;
			}
		}

		if(bestDeviceIndex == -1) {
			EG_ASSERT("Failed to find a suitable GPU!");
		}

		m_PhysicalDevice = devices[bestDeviceIndex];
	}

	void VulkanPhysicalDevice::Cleanup()
	{
	}

	bool VulkanPhysicalDevice::IsDeviceSuitable(VkPhysicalDevice device, const VkSurfaceKHR& surface, VulkanDeviceExtensions& extensions)
	{
		VulkanQueueFamilyIndices indices = VulkanQueueFamilyIndices::FindQueueFamilies(device, surface);

		bool extensionsSupported = extensions.CheckDeviceExtensionSupport(device);

		bool swapChainAdaquete = false;
		if (extensionsSupported) {
			VulkanSwapChainSupportDetails swapChainSupport = VulkanSwapChainSupportDetails::QuerySwapChainSupportDetails(device, surface);
			swapChainAdaquete = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		VkPhysicalDeviceFeatures supportedFeatures;
		vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

		return indices.IsComplete() && extensionsSupported && swapChainAdaquete && supportedFeatures.samplerAnisotropy;
	}

	int VulkanPhysicalDevice::RateDeviceSuitability(VkPhysicalDevice device, const VkSurfaceKHR& surface)
	{
		return 1;
	}


}
