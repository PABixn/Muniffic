#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {
	class VulkanMultisampling {
	public:
		VulkanMultisampling();
		~VulkanMultisampling();

		VkSampleCountFlagBits getMaxUsableSampleCount();
		VkSampleCountFlagBits getMsaaSamples() { return m_msaaSamples; }
	private:
		VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	};
}