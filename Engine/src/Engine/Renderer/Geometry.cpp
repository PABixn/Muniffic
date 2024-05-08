#include "egpch.h"
#include "Geometry.h"
#include "RendererAPI.h"
#include "Platform/Vulkan/GeometrySystem/VulkanGeometry.h"

namespace eg {
    Ref<Geometry> Geometry::Create(size_t vertexBufferSize, BufferLayout& layout, size_t indexBufferSize, std::string shaderPath)
    {
        if(RendererAPI::GetAPI() == RendererAPI::API::OpenGL)
			return CreateRef<Geometry>(vertexBufferSize, layout, indexBufferSize, shaderPath);
		else if(RendererAPI::GetAPI() == RendererAPI::API::Vulkan)
			return VulkanGeometry::Create(vertexBufferSize, layout, indexBufferSize, shaderPath);
    }
}