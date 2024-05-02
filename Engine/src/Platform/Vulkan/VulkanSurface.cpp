#include "egpch.h"
#include "VulkanSurface.h"

namespace eg {

	void VulkanSurface::Init(const VkInstance& instance, GLFWwindow* window)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, &m_Surface) != VK_SUCCESS)
		{
			EG_ASSERT(false, "Failed to create window surface!");
		}
	}

	void VulkanSurface::Cleanup(const VkInstance& instance)
	{
		vkDestroySurfaceKHR(instance, m_Surface, nullptr);
	}

	VkSurfaceFormatKHR VulkanSurface::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for(const auto& availableFormat : availableFormats)
		{
			if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return availableFormats[0];
	}
}
