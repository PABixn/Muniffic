#include "egpch.h"
#include "Engine/Core/Log.h"
#include "OpenGLContext.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"

namespace eg
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		EG_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		EG_PROFILE_FUNCTION();
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		EG_CORE_ASSERT(status, "Failed to initialize Glad!");
		//std::string vendor = (char*)glGetString(GL_VENDOR);
		EG_CORE_INFO("OpenGL Info:");
		EG_CORE_INFO("OpenGL Renderer: {0}", (char*)glGetString(GL_RENDERER));
		EG_CORE_INFO("Vendor: {0}", (char*)glGetString(GL_VENDOR));
		EG_CORE_INFO("Version: {0}", (char*)glGetString(GL_VERSION));

#ifdef EG_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		EG_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Muniffic requires at least OpenGL version 4.5!");
#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		EG_PROFILE_FUNCTION();
		glfwSwapBuffers(m_WindowHandle);
	}
}