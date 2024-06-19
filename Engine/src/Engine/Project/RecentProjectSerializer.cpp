#include "egpch.h"
#include "RecentProjectSerializer.h"

#include <fstream>
#include "yaml-cpp/yaml.h"
#include "../Engine-Editor/src/Panels/ConsolePanel.h"

namespace eg
{
	RecentProjectSerializer::RecentProjectSerializer()
	{
		Deserialize("recentProjectSerializer.txt");
	}

	bool RecentProjectSerializer::Serialize(const std::string& test, const std::string& path)
	{
		bool present = false;
		for (auto project : m_ProjectList) {
			if (project == test) {
				present = true;
				break;
			}
		}
		if (!present) {
			m_ProjectList.push_back(test);
			if (m_ProjectList.size() > 4) m_ProjectList.erase(m_ProjectList.begin());

			YAML::Emitter out;
			{
				out << YAML::BeginMap; // Root
				out << YAML::Key << "RecentProjects";
				{
					out << YAML::BeginMap; // Project
					if (m_ProjectList.size()) {
						int i = 0;
						for (auto project : m_ProjectList) {
							std::string name = "Projects" + std::to_string(i);
							i++;
							out << YAML::Key << name << YAML::Value << project;
						}
					}
					else {
						out << YAML::Key << "Projects0" << YAML::Value << test;
					}
					out << YAML::EndMap; // Project
				}
				out << YAML::EndMap; // Root
			}

			std::ofstream fout(path);
			fout << out.c_str();
		}
		return true;
	}

	bool RecentProjectSerializer::Deserialize(const std::string& path)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path);
		}
		catch (const YAML::Exception& e)
		{
			EG_CORE_ERROR("Failed to load recent projects file '{0}': {1}", path, e.what());
			ConsolePanel::Log("File: RecentProjectSerializer.cpp - Failed to load recent projects file: " + path, ConsolePanel::LogType::Error);
			return false;
		}

		auto projectNode = data["RecentProjects"];
		if (!projectNode)
		{
			EG_CORE_ERROR("Invalid projects file '{0}'", path);
			ConsolePanel::Log("File: RecentProjectSerializer.cpp - Invalid projects file: " + path, ConsolePanel::LogType::Error);
			return false;
		}

		int i = 0;
		for (auto project : projectNode) {
			std::string name = "Projects" + std::to_string(i);
			i++;
			m_ProjectList.push_back(projectNode[name].as<std::string>());
			if (m_ProjectList.size() > 4) m_ProjectList.erase(m_ProjectList.begin());
		}


		//config.Name = projectNode["Name"].as<std::string>();
		/*config.StartScene = projectNode["StartScene"].as<std::string>();
		config.AssetDirectory = projectNode["AssetDirectory"].as<std::string>();
		config.SceneDirectory = projectNode["SceneDirectory"].as<std::string>();
		config.ScriptModulePath = projectNode["ScriptModulePath"].as<std::string>();*/
		ConsolePanel::Log("File: RecentProjectSerializer.cpp - Successfully loaded: " + path, ConsolePanel::LogType::Info);
		return true;
	}
}