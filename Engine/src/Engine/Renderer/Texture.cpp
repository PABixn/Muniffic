#include "egpch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "../Engine-Editor/src/Panels/ConsolePanel.h"

namespace eg {
	ConsolePanel consolePanel;
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
		}

		EG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: 
				consolePanel.Log("2D Texture Created", ConsolePanel::LogType::Info);
				return CreateRef<OpenGLTexture2D>(specification);
		}

		EG_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}