#pragma once
#define GLFW_INCLUDE_VULKAN
#include "VulkanImage.h"
#include <GLFW/glfw3.h>

namespace eg {
	class VulkanImageCreator {
	public:
		VulkanImageCreator(VkDevice device);
		~VulkanImageCreator();

		Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(const UUID& uuid);
		static Ref<Texture2D> Create(const TextureSpecification& specification);
	private:
		VkDevice m_Device;
	};
}