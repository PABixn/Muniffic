#include "egpch.h"
#include "RecentProjectSerializer.h"

#include <fstream>
#include <filesystem>
#include "yaml-cpp/yaml.h"
#include "../Engine-Editor/src/Panels/ConsolePanel.h"

namespace eg
{
	RecentProjectSerializer::RecentProjectSerializer()
	{
		EG_PROFILE_FUNCTION();
		Deserialize("recentProjectSerializer.txt");
	}

	void RecentProjectSerializer::DeleteProject(int id) {
		EG_PROFILE_FUNCTION();
		int cwel = m_ProjectList[id].find_last_of("\\");
		std::string toDelete = m_ProjectList[id].erase(cwel);
		std::filesystem::remove_all(toDelete);
		m_ProjectList.erase(m_ProjectList.begin() + id);
		Serialize("", "recentProjectSerializer.txt");
	}

	bool RecentProjectSerializer::Serialize(const std::string& projectName, const std::string& path)
	{
		EG_PROFILE_FUNCTION();
		bool present = false;
		if (projectName != "")
		{
			for (auto project : m_ProjectList) {
				if (project == projectName) {
					present = true;
					break;
				}
			}
		}
		if (!present)
		{
			if (projectName != "")
			{
				m_ProjectList.push_back(projectName);
				if (m_ProjectList.size() > 4) m_ProjectList.erase(m_ProjectList.begin());
			}
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
						out << YAML::Key << "Projects0" << YAML::Value << projectName;
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
		EG_PROFILE_FUNCTION();
		YAML::Node data;
		try
		{
			std::filesystem::path abs = std::filesystem::absolute(path);

			if (!std::filesystem::exists(abs))
			{
				//std::filesystem::create_(path);
 				std::ofstream file(path);

				if (file.is_open()) {
					file << "RecentProjects:" << std::endl;
					file.close();
					std::cout << "File created and written successfully." << std::endl;
				}
				else {
					std::cerr << "Error creating or opening the file." << std::endl;
				}

			}
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
			if (projectNode[name].as<std::string>() != "") {
				i++;

				m_ProjectList.push_back(projectNode[name].as<std::string>());
				if (m_ProjectList.size() > 4) m_ProjectList.erase(m_ProjectList.begin());
			}
		}

		ConsolePanel::Log("File: RecentProjectSerializer.cpp - Successfully loaded: " + path, ConsolePanel::LogType::Info);
		return true;
	}

}
