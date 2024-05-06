#include "egpch.h"
#include "VulkanShader.h"
#include "Engine/Core/Timer.h"
#include "GraphicsPipelineFactory.h"
#include "VulkanUtils.h"
#include "VulkanCommandManager.h"
#include "PipelineStates/VulkanPipelineShaderStageCreateInfo.h"
#include <fstream>
#include <filesystem>
#include <glm/gtc/type_ptr.hpp>

#include "spirv_cross/spirv_cross.hpp"
#include "spirv_cross/spirv_glsl.hpp"
#include "shaderc/shaderc.hpp"
#include "VulkanUtils.h"




namespace eg
{

	namespace Utils {

		static ShaderType ShaderTypeFromString(const std::string& type)
		{
			if (type == "vertex")
				return ShaderType::VERTEX;
			if (type == "fragment" || type == "pixel")
				return ShaderType::FRAGMENT;

			EG_CORE_ASSERT(false, "Unknown shader type!");
			return ShaderType::NONE;
		}

		static shaderc_shader_kind GLShaderStageToShaderC(ShaderType stage)
		{
			switch (stage)
			{
			case ShaderType::VERTEX:   return shaderc_glsl_vertex_shader;
			case ShaderType::FRAGMENT: return shaderc_glsl_fragment_shader;
			}
			EG_CORE_ASSERT(false);
			return (shaderc_shader_kind)0;
		}

		static const char* GLShaderStageToString(ShaderType stage)
		{
			switch (stage)
			{
			case ShaderType::VERTEX:   return "ShaderType::VERTEX";
			case ShaderType::FRAGMENT: return "ShaderType::FRAGMENT";
			}
			EG_CORE_ASSERT(false);
			return nullptr;
		}

		static const char* GetCacheDirectory()
		{
			// TODO: make sure the assets directory is valid
			return "assets/cache/shader/vulkan";
		}

		static void CreateCacheDirectoryIfNeeded()
		{
			std::string cacheDirectory = GetCacheDirectory();
			if (!std::filesystem::exists(cacheDirectory))
				std::filesystem::create_directories(cacheDirectory);
		}

		static const char* GLShaderStageCachedOpenGLFileExtension(ShaderType stage)
		{
			switch (stage)
			{
			case ShaderType::VERTEX:    return ".cached_opengl.vert";
			case ShaderType::FRAGMENT:  return ".cached_opengl.frag";
			}
			EG_CORE_ASSERT(false);
			return "";
		}

		static const char* GLShaderStageCachedVulkanFileExtension(ShaderType stage)
		{
			switch (stage)
			{
			case ShaderType::VERTEX:    return ".cached_vulkan.vert";
			case ShaderType::FRAGMENT:  return ".cached_vulkan.frag";
			}
			EG_CORE_ASSERT(false);
			return "";
		}
	}

	VulkanShader::VulkanShader(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& name, BufferLayout layout)
		: m_RendererID(0), m_Name(name), m_Layout(layout)
	{
		EG_PROFILE_FUNCTION();
		VulkanHandler* handler = GetVulkanHandler();
		Ref<VulkanGraphicsPipelineBuilder> factory = handler->GetMainRenderPass().GetPipelineBuilder();
		std::unordered_map<ShaderType, std::vector<uint32_t>> shaderSources;
		std::vector<uint32_t> vertData(vertexSrc.begin(), vertexSrc.end());
		std::vector<uint32_t> fragData(fragmentSrc.begin(), fragmentSrc.end());
		shaderSources[ShaderType::VERTEX] = vertData;
		shaderSources[ShaderType::FRAGMENT] = fragData;
		m_VulkanShaderModules = CreateShaderModules(shaderSources);
		m_DescriptorSetLayout.Create();
		m_GraphicsPipeline = factory->setShader(this).SetBaseInfo().build();
		m_DescriptorPool.Create(handler->GetVulkanLogicalDevice().GetDevice());
		m_DescriptorSets.Create(handler->GetVulkanLogicalDevice().GetDevice(), m_DescriptorPool.getPool(), m_DescriptorSetLayout.GetDescriptorSetLayout(), m_UniformBuffers);
	}

	VulkanShader::VulkanShader(const std::string& filepath, BufferLayout layout)
		: m_FilePath(filepath), m_RendererID(0), m_Layout(layout)
	{
		EG_PROFILE_FUNCTION();
		Utils::CreateCacheDirectoryIfNeeded();
		m_VulkanShaderModules = CreateShaderModules(filepath);

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	VulkanShader::~VulkanShader()
	{
		vkDestroyShaderModule(GetVulkanHandler()->GetVulkanLogicalDevice().GetDevice(), m_VulkanShaderModules[ShaderType::VERTEX], nullptr);
		vkDestroyShaderModule(GetVulkanHandler()->GetVulkanLogicalDevice().GetDevice(), m_VulkanShaderModules[ShaderType::FRAGMENT], nullptr);
	}

	const std::vector<VkPipelineShaderStageCreateInfo>& VulkanShader::GetShaderStages() const
	{
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		for(auto&& [stage, module] : m_VulkanShaderModules)
		{
			shaderStages.push_back(VulkanPipelineShaderStageCreateInfo(stage, module));
		}
	}

	std::string VulkanShader::Readfile(const std::string& filepath)
	{
		EG_PROFILE_FUNCTION();
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);

		if (in)
		{

			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
			}
			else
			{
				EG_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			EG_CORE_ERROR("Could not open file '{0}'", filepath);
		}
		return result;
	}

	std::unordered_map<ShaderType, std::string> VulkanShader::Preprocess(const std::string& source)
	{
		EG_PROFILE_FUNCTION();

		std::unordered_map<ShaderType, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			EG_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			EG_CORE_ASSERT(static_cast<uint32_t>(Utils::ShaderTypeFromString(type)), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			EG_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}

		return shaderSources;
	}

	void VulkanShader::CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
		const bool optimize = true;
		if (optimize)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cacheDirectory = Utils::GetCacheDirectory();

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();
		for (auto&& [stage, source] : shaderSources)
		{
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedPath = cacheDirectory / (shaderFilePath.filename().string() + Utils::GLShaderStageCachedVulkanFileExtension(stage));

			std::ifstream in(cachedPath, std::ios::in | std::ios::binary);
			if (in.is_open())
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);
				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else
			{
				shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success)
				{
					EG_CORE_ERROR(module.GetErrorMessage());
					EG_CORE_ASSERT(false);
				}

				shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

				std::ofstream out(cachedPath, std::ios::out | std::ios::binary);
				if (out.is_open())
				{
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
		}

		for (auto&& [stage, data] : shaderData)
			Reflect(stage, data);
	}

	VkShaderModule VulkanShader::CreateShaderModule(std::filesystem::path shaderPath)
	{
		
	}

	VkShaderModule VulkanShader::CreateShaderModule(const std::vector<uint32_t>& code)
	{
		VulkanHandler* handler = GetVulkanHandler();

		VkShaderModule shaderModule;
		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = code.data();

		if (vkCreateShaderModule(handler->GetVulkanLogicalDevice().GetDevice(), &createInfo, nullptr, &shaderModule))
		{
			throw std::runtime_error("failed to create shader module!");
		}

		return shaderModule;
	}

	std::unordered_map<ShaderType, VkShaderModule> VulkanShader::CreateShaderModules(std::filesystem::path shadersPath)
	{
		std::unordered_map<ShaderType, VkShaderModule> shaderModules;
		std::string shaderSource = Readfile(shadersPath.string());
		auto shaderSources = Preprocess(shaderSource);
		CompileOrGetVulkanBinaries(shaderSources);
		shaderModules[ShaderType::VERTEX] = CreateShaderModule(m_VulkanSPIRV[ShaderType::VERTEX]);
		shaderModules[ShaderType::FRAGMENT] = CreateShaderModule(m_VulkanSPIRV[ShaderType::FRAGMENT]);
	}

	std::unordered_map<ShaderType, VkShaderModule> VulkanShader::CreateShaderModules(const std::unordered_map<ShaderType, std::vector<uint32_t>>& shaderData)
	{
		std::unordered_map<ShaderType, VkShaderModule> shaderModules;
		for (auto&& [stage, data] : shaderData)
			shaderModules[stage] = CreateShaderModule(data);
		return shaderModules;
	}

	void VulkanShader::Reflect(ShaderType stage, const std::vector<uint32_t>& shaderData)
	{
		spirv_cross::Compiler compiler(shaderData);
		spirv_cross::ShaderResources resources = compiler.get_shader_resources();

		EG_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", Utils::GLShaderStageToString(stage), m_FilePath);
		EG_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
		EG_CORE_TRACE("    {0} resources", resources.sampled_images.size());

		EG_CORE_TRACE("Uniform buffers:");
		for (const auto& resource : resources.uniform_buffers)
		{
			const auto& bufferType = compiler.get_type(resource.base_type_id);
			uint32_t bufferSize = compiler.get_declared_struct_size(bufferType);
			uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
			int memberCount = bufferType.member_types.size();

			EG_CORE_TRACE("  {0}", resource.name);
			EG_CORE_TRACE("    Size = {0}", bufferSize);
			EG_CORE_TRACE("    Binding = {0}", binding);
			EG_CORE_TRACE("    Members = {0}", memberCount);
		}
	}

	void VulkanShader::Bind()
	{
		EG_PROFILE_FUNCTION();
		VulkanHandler* handler = GetVulkanHandler();
		m_GraphicsPipeline->Bind(handler->GetCurrentCommandBuffer());
		m_DescriptorSets.Bind(handler->GetCurrentCommandBuffer(), m_GraphicsPipeline->GetPipelineLayout(), 0/*should be current frame*/);
	}
}