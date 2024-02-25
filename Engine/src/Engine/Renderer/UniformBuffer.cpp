#include "egpch.h"
#include "UniformBuffer.h"

#include "Engine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"
#include "../Engine-Editor/src/Panels/ConsolePanel.h"

namespace eg {
	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    
			EG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
			ConsolePanel::Log("File: UniformBuffer.cpp - RendererAPI::None is currently not supported!", ConsolePanel::LogType::Error);
			return nullptr;
		case RendererAPI::API::OpenGL:  
			ConsolePanel::Log("File: UniformBuffer.cpp - Successfully Created UniformBuffer", ConsolePanel::LogType::Info);
			return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		EG_CORE_ASSERT(false, "Unknown RendererAPI!");
		ConsolePanel::Log("File: UniformBuffer.cpp - Unknown RendererAPI!", ConsolePanel::LogType::Error);
		return nullptr;
	}

}