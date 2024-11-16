#pragma once
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif // !RENDERER_INCLUDE_GLFW_VULKAN
#include "Platform/Vulkan/Renderer/Resources/Buffer/SSBuffer.h"
namespace eg {
	class MatrixBufferDescriptorHelper
	{
	public:
		void init();
		void cleanup();
		void bindSSBO(VulkanShaderStorageBuffer* SSBO);
		VkDescriptorSetLayout& getDescriptorSetLayout() { return m_DescriptorSetLayout; }
		VkDescriptorSet m_DescriptorSet;
	private:
		VkDescriptorPool m_DescriptorPool;
		VkDescriptorSetLayout m_DescriptorSetLayout;
		uint32_t m_DescriptorSetCount = 1;

	};
}