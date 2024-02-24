#pragma once
#include "Engine/Core/Core.h"
#include <string>
#include <filesystem>

namespace eg {
	
	struct ProjectConfig
	{
		std::string Name = "Untitiled";

		std::filesystem::path StartScene = "Untitled.scene";
		std::filesystem::path AssetDirectory = "assets";
		std::filesystem::path SceneDirectory = "scenes";
		std::filesystem::path ScriptModulePath = "scripts";
	};

	class Project
	{
	public:
		Project() = default;
		Project(ProjectConfig projectConfig);

		static Ref<Project> New();
		static bool Save(const std::filesystem::path& path);
		static Ref<Project> Load(const std::filesystem::path& path);

		void CreateProject(ProjectConfig projectConfig);

		static const std::filesystem::path& GetAssetDirectory()
		{
			EG_CORE_ASSERT(s_ActiveProject, "No active project");
			return s_ActiveProject->m_Config.AssetDirectory; 
		}

		static const std::filesystem::path& GetProjectDirectory()
		{
			EG_CORE_ASSERT(s_ActiveProject, "No active project");
			return s_ActiveProject->m_ProjectDirectory; 
		}

		static const std::string GetProjectName()
		{
			EG_CORE_ASSERT(s_ActiveProject, "No active project");
			return s_ActiveProject->m_Config.Name; 
		}

		//TODO: Move this to AssetManager
		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path) {
			return GetProjectDirectory() / GetAssetDirectory() / path;
		}

		static std::filesystem::path GetSceneFileSystemPath(const std::filesystem::path& path) {
			return GetProjectDirectory() / GetAssetDirectory() / GetScenesDirectory() / path;
		}

		ProjectConfig& GetConfig() { 
			return m_Config; 
		}

		static Ref<Project> GetActive() {
			return s_ActiveProject; 
		}

		static const std::filesystem::path& GetScenesDirectory() {
			EG_CORE_ASSERT(s_ActiveProject, "No active project");
			return s_ActiveProject->m_Config.SceneDirectory; 
		}

		static const std::filesystem::path& GetScriptModulePath() {
			EG_CORE_ASSERT(s_ActiveProject, "No active project");
			return s_ActiveProject->m_Config.ScriptModulePath; 
		}

		static const std::filesystem::path& GetStartScene() {
			EG_CORE_ASSERT(s_ActiveProject, "No active project");
			return s_ActiveProject->m_Config.StartScene; 
		}

	private:
		ProjectConfig m_Config;
		std::filesystem::path m_ProjectDirectory;

		inline static Ref<Project> s_ActiveProject;
	};

}