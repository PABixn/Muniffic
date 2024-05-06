#include "egpch.h"
#include "RendererAPI.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
namespace eg {
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::Vulkan;
	Ref<RendererAPI> RendererAPI::Create()
	{
		return Ref<RendererAPI>();
	}
}