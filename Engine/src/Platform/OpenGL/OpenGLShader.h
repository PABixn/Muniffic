#pragma once

#include <string>
#include <glm/glm.hpp>
#include "Engine/Renderer/Shader.h"
#include <unordered_map>
typedef unsigned int GLenum;
namespace eg
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& name);
		OpenGLShader(const std::string& filepath);
		virtual~OpenGLShader();

		void Bind() const;
		void Unbind() const;

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat(const std::string& name, float value) override;

		virtual const std::string& GetName() const override { return m_Name; }

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);

		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat(const std::string& name, float value);

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
	private:
		std::string Readfile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> Preprocess(const std::string& source);
		void CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources);
		void CompileOrGetOpenGLBinaries();
		void CreateProgram();
		void Reflect(GLenum stage, const std::vector<uint32_t>& shaderData);
	private:
		uint32_t m_RendererID;
		std::string m_FilePath;
		std::string m_Name;
		std::unordered_map<GLenum, std::vector<uint32_t>> m_VulkanSPIRV;
		std::unordered_map<GLenum, std::vector<uint32_t>> m_OpenGLSPIRV;

		std::unordered_map<GLenum, std::string> m_OpenGLSourceCode;
	};
}