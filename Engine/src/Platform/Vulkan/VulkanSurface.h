#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {

	class VulkanSurface {
	public:
		VulkanSurface(const VkInstance& instance);
		~VulkanSurface();
		void Cleanup(const VkInstance& instance);
		const VkSurfaceKHR& GetSurface() { return m_Surface; }
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	private:
		VkSurfaceKHR m_Surface;
	};

}