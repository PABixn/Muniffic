#include "egpch.h"
#include "VulkanDebugMessenger.h"

namespace eg {

	void VulkanDebugMessenger::Init(const VkInstance& instance)
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		PopulateDebugMessengerCreateInfo(createInfo);
		CreateDebugUtilsMessengerEXT(instance, &createInfo);
	
	}

	void VulkanDebugMessenger::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
	{
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = DebugCallback;
	}

	void VulkanDebugMessenger::Cleanup(const VkInstance& instance)
	{
		DestroyDebugUtilsMessengerEXT(instance);
	}

	void VulkanDebugMessenger::CreateDebugUtilsMessengerEXT(const VkInstance& instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			if (func(instance, pCreateInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS)
			{
				EG_ASSERT(false, "Failed to set up debug messenger!");
			}
		}
		else
		{
			EG_ASSERT(false, "Failed to set up debug messenger!");
		}
	}

	void VulkanDebugMessenger::DestroyDebugUtilsMessengerEXT(const VkInstance& instance)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, m_DebugMessenger, nullptr);
		}
		else
		{
			EG_ASSERT(false, "Failed to destroy debug messenger!");
		}
	}

	VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugMessenger::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			EG_CORE_ERROR("Validation layer: {0}", pCallbackData->pMessage);
		}
		else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			EG_CORE_WARN("Validation layer: {0}", pCallbackData->pMessage);
		}
		else
		{
			EG_CORE_INFO("Validation layer: {0}", pCallbackData->pMessage);
		}

		return VK_FALSE;
	}


}