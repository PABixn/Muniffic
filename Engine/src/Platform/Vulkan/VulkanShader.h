#pragma once
#define GLFW_INCLUDE_VULKAN
#include <string>
#include <glm/glm.hpp>
#include "Engine/Renderer/Shader.h"
#include "VulkanVertex.h"
#include "VulkanGraphicsPipeline.h"
#include <unordered_map>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
namespace eg {
	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& name, BufferLayout layout);
		VulkanShader(const std::string& filepath, BufferLayout layout);
		virtual~VulkanShader();

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

		//VkPipelineShaderStageCreateInfo GetVertexShaderStage() const { return m_VulkanShaderModules[ShaderType::VERTEX]; }
		//VkPipelineShaderStageCreateInfo GetFragmentShaderStage() const { return m_VulkanShaderModules[ShaderType::FRAGMENT]; }
		VkShaderModule GetVertexShaderModule() const { return m_VulkanShaderModules.at(ShaderType::FRAGMENT); }
		VkShaderModule GetFragmentShaderModule() const { return m_VulkanShaderModules.at(ShaderType::FRAGMENT); }
		VkShaderModule GetShaderModule(ShaderType type) const { return m_VulkanShaderModules.at(type); }

		const VulkanGraphicsPipeline& GetGraphicsPipeline() const { return m_GraphicsPipeline; }
		const VulkanVertexInputLayout& GetLayout() const { return m_Layout; }
		VulkanVertexInputLayout& GetLayout() { return m_Layout; }
	private:
		std::string Readfile(const std::string& filepath);
		std::unordered_map<ShaderType, std::string> Preprocess(const std::string& source);
		void CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& shaderSources);
		void CompileOrGetOpenGLBinaries();
		VkShaderModule CreateShaderModule(std::filesystem::path shaderPath);
		VkShaderModule CreateShaderModule(const std::vector<uint32_t>& code);
		std::unordered_map<ShaderType, VkShaderModule> CreateShaderModules(std::filesystem::path shadersPath);
		std::unordered_map<ShaderType, VkShaderModule> CreateShaderModules(const std::unordered_map<ShaderType, std::vector<uint32_t>>& shaderData);
		void Reflect(ShaderType stage, const std::vector<uint32_t>& shaderData);
	private:
		uint32_t m_RendererID;
		std::string m_FilePath;
		std::string m_Name;
		std::unordered_map<ShaderType, std::vector<uint32_t>> m_VulkanSPIRV;
		std::unordered_map<ShaderType, VkShaderModule> m_VulkanShaderModules;
		VulkanGraphicsPipeline m_GraphicsPipeline;
		VulkanVertexInputLayout m_Layout;
	};
}