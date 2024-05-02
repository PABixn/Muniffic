#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {

	class VulkanValidationLayers {
	public:
		~VulkanValidationLayers() = default;
		static VulkanValidationLayers& GetInstance() {
			static VulkanValidationLayers instance;
			return instance;
		}
		bool CheckValidationLayerSupport();
		const std::vector<const char*>& GetValidationLayers() { return m_ValidationLayers; }
		bool ValidationLayersEnabled() { return enableValidationLayers; }
		size_t size() { return m_ValidationLayers.size(); }
	private:
		VulkanValidationLayers();
	private:
		bool enableValidationLayers = true;
		const std::vector<const char*> m_ValidationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
	};
}