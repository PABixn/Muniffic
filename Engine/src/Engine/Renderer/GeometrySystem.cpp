#include "egpch.h"
#include "GeometrySystem.h"
#include "RendererAPI.h"
#include "Platform/Vulkan/GeometrySystem/GeometrySystem.h"

namespace eg {
    Ref<GeometrySystem> GeometrySystem::Create()
    {
        if(RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
			return CreateRef<GeometrySystem>();
		else if(RendererAPI::GetAPI() == RendererAPI::API::Vulkan)
			return CreateRef<VulkanGeometrySystem>();
    }
}
