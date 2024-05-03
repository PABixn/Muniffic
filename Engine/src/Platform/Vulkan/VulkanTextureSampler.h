#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"
#include <GLFW/glfw3.h>

namespace eg
{
	class VulkanTextureSampler {
	public:
		VulkanTextureSampler() = default;
		~VulkanTextureSampler() = default;

		void Create();
		void Cleanup();

		const VkSampler& GetSampler() const { return m_Sampler; }
	private:
		VkSampler m_Sampler;
	};
}