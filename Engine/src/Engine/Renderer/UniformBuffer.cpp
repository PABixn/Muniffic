#include "egpch.h"
#include "UniformBuffer.h"

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"
#include "../Engine-Editor/src/Panels/ConsolePanel.h"

namespace eg {
	ConsolePanel consolePanel;
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    
			EG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
			consolePanel.Log("RendererAPI::None is currently not supported!", ConsolePanel::LogType::Error);
			return nullptr;
		case RendererAPI::API::OpenGL:  
			consolePanel.Log("Successfully Created UniformBuffer", ConsolePanel::LogType::Info);
			return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		EG_CORE_ASSERT(false, "Unknown RendererAPI!");
		consolePanel.Log("Unknown RendererAPI!", ConsolePanel::LogType::Error);
		return nullptr;
	}

}