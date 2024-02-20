#include "egpch.h"
#include "FrameBuffer.h"
#include "../Engine-Editor/src/Panels/ConsolePanel.h"
#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace eg {
	ConsolePanel consolePanel;
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:
			consolePanel.Log("FrameBuffer Created", ConsolePanel::LogType::Info);
			return CreateRef<OpenGLFrameBuffer>(spec);
		}

		EG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}