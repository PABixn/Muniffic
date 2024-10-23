#include "egpch.h"
#include "FrameBuffer.h"
#include "../Engine-Editor/src/Panels/ConsolePanel.h"
#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace eg {
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
        EG_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			ConsolePanel::Log("File: FrameBuffer.cpp - RendererAPI::None is currently not supported!", ConsolePanel::LogType::Error);
			EG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:
			ConsolePanel::Log("File: FrameBuffer.cpp - FrameBuffer Created", ConsolePanel::LogType::Info);
			return CreateRef<OpenGLFrameBuffer>(spec);
		case RendererAPI::API::Vulkan:
			return nullptr; //Framebuffer already created
		}

		EG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
