#include "egpch.h"
#include "ProjectSerializer.h"

#include <fstream>
#include "yaml-cpp/yaml.h"
#include "../Engine-Editor/src/Panels/ConsolePanel.h"

namespace eg
{
	ProjectSerializer::ProjectSerializer(Ref<Project> project)
		: m_Project(project)
	{
		EG_PROFILE_FUNCTION();
	}

	bool ProjectSerializer::Serialize(const std::filesystem::path& path)
	{
		EG_PROFILE_FUNCTION();
		const auto& config = m_Project->GetConfig();

		YAML::Emitter out;
		{
			out << YAML::BeginMap; // Root
			out << YAML::Key << "Project";
			{
				out << YAML::BeginMap; // Project
				out << YAML::Key << "Name" << YAML::Value << config.Name;
				out << YAML::Key << "StartScene" << YAML::Value << config.StartScene.string();
				out << YAML::Key << "AssetDirectory" << YAML::Value << config.AssetDirectory.string();
				out << YAML::Key << "SceneDirectory" << YAML::Value << config.SceneDirectory.string();
				out << YAML::Key << "ScriptModulePath" << YAML::Value << config.ScriptModulePath.string();
				out << YAML::EndMap; // Project
			}
			out << YAML::EndMap; // Root
		}

		std::ofstream fout(path);
		fout << out.c_str();

		return true;
	}

	bool ProjectSerializer::Deserialize(const std::filesystem::path& path)
	{
		EG_PROFILE_FUNCTION();
		auto& config = m_Project->GetConfig();
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path.string());
		}
		catch (const YAML::Exception& e)
		{
			EG_CORE_ERROR("Failed to load project file '{0}': {1}", path.string(), e.what());
			ConsolePanel::Log("File: ProjectSerializer.cpp - Failed to load project file: " + path.string(), ConsolePanel::LogType::Error);
			return false;
		}

		auto projectNode = data["Project"];
		if (!projectNode)
		{
			EG_CORE_ERROR("Invalid project file '{0}'", path.string());
			ConsolePanel::Log("File: ProjectSerializer.cpp - Invalid project file: " + path.string(), ConsolePanel::LogType::Error);
			return false;
		}

		config.Name = projectNode["Name"].as<std::string>();
		config.StartScene = projectNode["StartScene"].as<std::string>();
		config.AssetDirectory = projectNode["AssetDirectory"].as<std::string>();
		config.SceneDirectory = projectNode["SceneDirectory"].as<std::string>();
		config.ScriptModulePath = projectNode["ScriptModulePath"].as<std::string>();
		ConsolePanel::Log("File: ProjectSerializer.cpp - Successfully loaded: " + path.string(), ConsolePanel::LogType::Info);
		return true;
	}
}
