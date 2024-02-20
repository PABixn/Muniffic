#include "egpch.h"
#include "VertexArray.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "../Engine-Editor/src/Panels/ConsolePanel.h"

namespace eg {
	ConsolePanel consolePanel;
	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: 
			EG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
			consolePanel.Log("RendererAPI::None is currently not supported!", ConsolePanel::LogType::Error);
			return nullptr;
		case RendererAPI::API::OpenGL: 
			consolePanel.Log("Successfully created VertexArray", ConsolePanel::LogType::Info);
			return std::make_shared<OpenGLVertexArray>();
		}

		EG_CORE_ASSERT(false, "Unknown RendererAPI!");
		consolePanel.Log("Unknown RendererAPI!", ConsolePanel::LogType::Error);
		return nullptr;
	}
}