#include "egpch.h"
#include "Shader.h"
#include <glad/glad.h>
#
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/Vulkan/VulkanShader.h"
#include "../Engine-Editor/src/Panels/ConsolePanel.h"

namespace eg {
	Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& name, BufferLayout layout)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: 
				EG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				ConsolePanel::Log("File: Shader.cpp - RendererAPI::None is currently not supported!", ConsolePanel::LogType::Error);
				return nullptr;
			case RendererAPI::API::OpenGL:
				ConsolePanel::Log("File: Shader.cpp - Successfully Created Shader", ConsolePanel::LogType::Info);
				return CreateRef<OpenGLShader>(vertexSrc, fragmentSrc, name);
			case RendererAPI::API::Vulkan:
				ConsolePanel::Log("File: Shader.cpp - Successfully Created Shader", ConsolePanel::LogType::Info);
				return CreateRef<VulkanShader>(vertexSrc, fragmentSrc, name, layout);
		}

		EG_CORE_ASSERT(false, "Unknown RendererAPI!");
		ConsolePanel::Log("File: Shader.cpp - Unknown RendererAPI!", ConsolePanel::LogType::Error);
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& filepath, BufferLayout layout)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: 
			EG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); 
			ConsolePanel::Log("File: Shader.cpp - RendererAPI::None is currently not supported!", ConsolePanel::LogType::Error);
			return nullptr;
		case RendererAPI::API::OpenGL: 
			ConsolePanel::Log("File: Shader.cpp - Successfully Created Shader", ConsolePanel::LogType::Info);
			return  CreateRef<OpenGLShader>(filepath);
		case RendererAPI::API::Vulkan:
			ConsolePanel::Log("File: Shader.cpp - Successfully Created Shader", ConsolePanel::LogType::Info);
			return CreateRef<VulkanShader>(filepath, layout);
		}

		EG_CORE_ASSERT(false, "Unknown RendererAPI!");
		ConsolePanel::Log("File: Shader.cpp - Unknown RendererAPI!", ConsolePanel::LogType::Error);
		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		EG_CORE_ASSERT(!Exists(name), "Shader already exists!");
		ConsolePanel::Log("File: Shader.cpp - Shader already exists!", ConsolePanel::LogType::Warning);
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		EG_CORE_ASSERT(!Exists(name), "Shader already exists!");
		ConsolePanel::Log("File: Shader.cpp - Shader already exists!", ConsolePanel::LogType::Warning);
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& vertexFilepath, const std::string& fragmentFilepath, const std::string& name)
	{
		auto shader = Shader::Create(vertexFilepath, fragmentFilepath, name);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		EG_CORE_ASSERT(Exists(name), "Shader not found!");
		ConsolePanel::Log("File: Shader.cpp - Shader do not exist!", ConsolePanel::LogType::Error);
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}