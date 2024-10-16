#pragma once
#include "Engine/Core/Core.h"
#include <string>
#include <filesystem>

namespace eg {
	

	const 


	struct ProjectConfig
	{
		std::string Name = "Untitiled";
		std::filesystem::path StartScene = "Untitled.egscene";
		std::filesystem::path AssetDirectory = "Assets";
		std::filesystem::path SceneDirectory = "Scenes";
        std::filesystem::path ScriptModulePath = "Scripts/Binaries";
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

		static void SetAssetDirectory(const std::filesystem::path& newDirectory) {
			EG_CORE_ASSERT(s_ActiveProject, "No active project");
			s_ActiveProject->m_Config.AssetDirectory = newDirectory;
		}

		static const std::filesystem::path& GetProjectDirectory()
		{
			EG_CORE_ASSERT(s_ActiveProject, "No active project");
			return s_ActiveProject->m_ProjectDirectory; 
		}

		static void SetProjectDirectory(const std::filesystem::path& newDirectory) {
			EG_CORE_ASSERT(s_ActiveProject, "No active project");
			s_ActiveProject->m_ProjectDirectory = newDirectory;
		}

		static const std::string GetProjectName()
		{
			//return "Hello";
			/*if (s_ActiveProject)
				return s_ActiveProject->m_Config.Name;
			else return "Hello";*/
			EG_CORE_ASSERT(s_ActiveProject, "No active project");
			return s_ActiveProject->m_Config.Name;
			
		}

		inline static const std::filesystem::path GetProjectPath(){
            EG_CORE_ASSERT(s_ActiveProject, "No active project");
            return s_ActiveProject->m_ProjectDirectory / (s_ActiveProject->m_Config.Name + ".mnproj");
        }

		static void SetProjectName(const std::string& newName) {
			EG_CORE_ASSERT(s_ActiveProject, "No active project");
			s_ActiveProject->m_Config.Name = newName;
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

		static void SetScenesDirectory(const std::filesystem::path& newDirectory) {
			EG_CORE_ASSERT(s_ActiveProject, "No active project");
			s_ActiveProject->m_Config.SceneDirectory = newDirectory;
		}

		static const std::filesystem::path GetScriptModulePath() {
			EG_CORE_ASSERT(s_ActiveProject, "No active project");
			return  s_ActiveProject->m_Config.ScriptModulePath /
				#ifdef EG_DEBUG
                "Debug"
				#elif EG_RELEASE
				"Release"
				#elif EG_DIST
				"Dist"
				#elif EG_RELWITHDEBINFO
				"RelWithDebInfo"
				#else
				"Debug"
				#endif
				/ (s_ActiveProject->m_Config.Name + ".dll");
		}

		static void SetScriptModulePath(const std::filesystem::path& newDirectory) {
			EG_CORE_ASSERT(s_ActiveProject, "No active project");
			s_ActiveProject->m_Config.ScriptModulePath = newDirectory;
		}

		static const std::filesystem::path& GetStartScene() {
			EG_CORE_ASSERT(s_ActiveProject, "No active project");
			return s_ActiveProject->m_Config.StartScene; 
		}

		static const std::filesystem::path GetResourcesPath()
		{
			EG_CORE_ASSERT(s_ActiveProject, "No active project");
			return s_ActiveProject->m_ProjectDirectory.parent_path();
		}

		
		inline static bool IsLoaded() { return s_ActiveProject != nullptr; }

	private:
		ProjectConfig m_Config;
		std::filesystem::path m_ProjectDirectory;

		inline static Ref<Project> s_ActiveProject;
	};

}