#pragma once
#include "VulkanContext.h"
#include "Engine/Core/Application.h"

namespace eg {
	inline VulkanHandler* GetVulkanHandler() {
		return &dynamic_cast<VulkanContext*>(Application::Get().GetWindow().GetContext())->GetVulkanHandler();
	}
}