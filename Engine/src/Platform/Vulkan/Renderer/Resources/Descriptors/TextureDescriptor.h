#pragma once
#pragma once
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif // !RENDERER_INCLUDE_GLFW_VULKAN
namespace eg {
	class VulkanSamplerArray;
	class TextureDescriptorHelper
	{
	public:
		void init();
		void cleanup();
		void bindSamplerArray(VulkanSamplerArray* samplerarray);
		VkDescriptorSetLayout& getDescriptorSetLayout() { return m_DescriptorSetLayout; }
		VkDescriptorSet m_DescriptorSet;
	private:
		VkDescriptorPool m_DescriptorPool;
		VkDescriptorSetLayout m_DescriptorSetLayout;
		uint32_t m_DescriptorSetCount = 1;

	};
}