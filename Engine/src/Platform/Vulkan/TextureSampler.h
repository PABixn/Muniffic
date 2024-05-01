#pragma once
#define GLFW_INCLUDE_VULKAN
#include <Engine/Core/Core.h>
#include <GLFW/glfw3.h>

namespace eg {
	class TextureSampler {
	public:
		TextureSampler(VkDevice logicalDevice,VkPhysicalDevice physicalDevice, uint32_t mipLevels);
		~TextureSampler();

		VkSampler getSampler() const { return m_sampler; }
	private:
		VkSampler m_sampler;
	};
}