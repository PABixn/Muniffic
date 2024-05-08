#include "egpch.h"
#include "RendererAPI.h"
#include "Platform/Vulkan/VulkanRendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
namespace eg {
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::Vulkan;
	Ref<RendererAPI> RendererAPI::Create()
	{
		switch (RendererAPI::s_API)
		{
		case RendererAPI::API::Vulkan:
			return CreateRef<VulkanRendererAPI>();
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLRendererAPI>();
		default:
			EG_CORE_ASSERT(false, "Unknown RendererAPI!");
			return nullptr;
		}
	}
}