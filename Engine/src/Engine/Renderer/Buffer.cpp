#include "egpch.h"
#include "RendererAPI.h"
#include "Renderer.h"
#include "Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "../Engine-Editor/src/Panels/ConsolePanel.h"
#include "Buffer.h"


namespace eg {
	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
        EG_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			ConsolePanel::Log("File: Buffer.cpp - RendererAPI::None is currently not supported!", ConsolePanel::LogType::Error);
			return nullptr;
		case RendererAPI::API::OpenGL:
			ConsolePanel::Log("File: Buffer.cpp - IndexBuffer created", ConsolePanel::LogType::Info);
			return CreateRef<OpenGLIndexBuffer>(indices, count);
		default:
			break;
		}
		return nullptr;
	}

	Ref<VertexBuffer>(VertexBuffer::Create(uint32_t size))
	{
        EG_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			ConsolePanel::Log("File: Buffer.cpp - RendererAPI::None is currently not supported!", ConsolePanel::LogType::Error);
			return nullptr;
		case RendererAPI::API::OpenGL:
			ConsolePanel::Log("File: Buffer.cpp - VertexBuffer created", ConsolePanel::LogType::Info);
			return CreateRef<OpenGLVertexBuffer>(size);
		default:
			break;
		}
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
        EG_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			ConsolePanel::Log("File: Buffer.cpp - RendererAPI::None is currently not supported!", ConsolePanel::LogType::Error);
			return nullptr;
		case RendererAPI::API::OpenGL:
			ConsolePanel::Log("File: Buffer.cpp - VertexBuffer created", ConsolePanel::LogType::Info);
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
		default:
			break;
		}
		return nullptr;
	}
}
