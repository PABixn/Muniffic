#pragma once
#include "Engine/Renderer/GraphicsContext.h"
#include "VulkanHandler.h"

namespace eg {
	class VulkanContext : public GraphicsContext{
	public:
		VulkanContext(GLFWwindow* windowHandle);
		virtual void Init() override;
		virtual void StartFrame() override;
		virtual void SwapBuffers() override;

		VulkanHandler& GetVulkanHandler() { return m_VulkanHandler; }
	private:
		GLFWwindow* m_WindowHandle;
		VulkanHandler m_VulkanHandler;
	};
}