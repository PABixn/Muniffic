#include "egpch.h"
#include "VulkanInstance.h"
#include "Engine/Project/Project.h"

namespace eg {

	VulkanInstance::~VulkanInstance()
	{
		Cleanup();
	}

	void VulkanInstance::Init(VulkanDeviceExtensions& extensions)
	{
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = Project::GetProjectNameCStr();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_2;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		extensions.GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.GetDeviceExtensions().data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		if (VulkanValidationLayers::GetInstance().ValidationLayersEnabled()) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanValidationLayers::GetInstance().GetValidationLayers().size());
			createInfo.ppEnabledLayerNames = VulkanValidationLayers::GetInstance().GetValidationLayers().data();

			m_DebugMessenger.PopulateDebugMessengerCreateInfo(debugCreateInfo);
			createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		}
		else {
			createInfo.enabledLayerCount = 0;
			createInfo.pNext = nullptr;
		}

		VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
		if (result != VK_SUCCESS)
		{
			EG_CORE_ERROR("Failed to create Vulkan instance!");
		}

		if (VulkanValidationLayers::GetInstance().ValidationLayersEnabled()) {
			m_DebugMessenger.Init(m_Instance);
		}
	}

	void VulkanInstance::Cleanup()
	{
		if (VulkanValidationLayers::GetInstance().ValidationLayersEnabled()) {
			m_DebugMessenger.Cleanup(m_Instance);
		}

		vkDestroyInstance(m_Instance, nullptr);
	}

}