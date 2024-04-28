#pragma once
#define GLFW_INCLUDE_VULKAN
#include "Engine/Core/Core.h"

#include <GLFW/glfw3.h>

#include "vector"

namespace eg{

	class VulkanInstance {
	public:
		VulkanInstance();
		void Init();
		std::vector<const char*> getRequiredExtensions();
		void Cleanup();
		~VulkanInstance();

		const VkInstance& GetInstance() { return m_Instance; }
	private:
		
	private:
		VkInstance m_Instance;

	};
}