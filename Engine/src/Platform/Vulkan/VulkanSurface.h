#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {

	class VulkanSurface {
	public:
		VulkanSurface();
		~VulkanSurface();
		void CreateSurface(const VkInstance& instance);
		void Cleanup(const VkInstance& instance);
		const VkSurfaceKHR& GetSurface() { return m_Surface; }
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	private:
		VkSurfaceKHR m_Surface;
	};

}