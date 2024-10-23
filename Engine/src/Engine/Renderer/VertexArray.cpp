#include "egpch.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "../Engine-Editor/src/Panels/ConsolePanel.h"

namespace eg {
	Ref<VertexArray> VertexArray::Create()
	{
        EG_PROFILE_FUNCTION();
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			EG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			ConsolePanel::Log("File: VertexArray.cpp - RendererAPI::None is currently not supported!", ConsolePanel::LogType::Error);
			return nullptr;
		case RendererAPI::API::OpenGL:
			ConsolePanel::Log("File: VertexArray.cpp - Successfully created VertexArray", ConsolePanel::LogType::Info);
			return std::make_shared<OpenGLVertexArray>();
		}

		EG_CORE_ASSERT(false, "Unknown RendererAPI!");
		ConsolePanel::Log("File: VertexArray.cpp - Unknown RendererAPI!", ConsolePanel::LogType::Error);
		return nullptr;
	}
}
