#include "egpch.h"
#include "Shader.h"
#include <glad/glad.h>
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace eg {

	Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& name)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(vertexSrc, fragmentSrc, name);
		}

		EG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: EG_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return  std::make_shared<OpenGLShader>(filepath);
		}

		EG_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		EG_CORE_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		EG_CORE_ASSERT(!Exists(name), "Shader already exists!");
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
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}