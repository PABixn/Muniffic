#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

namespace eg {

	class VulkanValidationLayers {
	public:
		VulkanValidationLayers();
		~VulkanValidationLayers();
		bool CheckValidationLayerSupport();
		const std::vector<const char*>& GetValidationLayers() { return validationLayers; }
		bool ValidationLayersEnabled() { return enableValidationLayers; }
	private:
		bool enableValidationLayers = true;
		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};
	};
}