#pragma once
#include "glm/glm.hpp"
#include "Engine/Renderer/Buffer.h"
namespace eg
{
	enum class ShaderType : uint32_t
	{
		NONE = 0, VERTEX, FRAGMENT, COMPUTE

		
	};
	class Shader
	{
	public:
		virtual~Shader() = default;

		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat(const std::string& name, float value) = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& name, BufferLayout layout = {});
		static Ref<Shader> Create(const std::string& filepath, BufferLayout layout = {});
	};

	class ShaderLibrary
	{
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& filepath);
		Ref<Shader> Load(const std::string& name, const std::string& filepath);
		Ref<Shader> Load(const std::string& vertexFilepath, const std::string& fragmentFilepath, const std::string& name);

		Ref<Shader> Get(const std::string& name);

		bool Exists(const std::string& name) const;
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}