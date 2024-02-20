#include "egpch.h"
#include "Project.h"
#include "../Engine-Editor/src/Panels/ConsolePanel.h"
#include "ProjectSerializer.h"

namespace eg {
	Project::Project(ProjectConfig projectConfig)
		: m_Config(projectConfig)
	{
	}

	Ref<Project> Project::New()
	{
		s_ActiveProject = CreateRef<Project>();
		ConsolePanel::Log("Project created successfully", ConsolePanel::LogType::Info);
		return s_ActiveProject;
	}

	bool Project::Save(const std::filesystem::path& path)
	{
		ProjectSerializer serializer(s_ActiveProject);
		if (serializer.Serialize(path))
		{
			s_ActiveProject->m_ProjectDirectory = path.parent_path();
			ConsolePanel::Log("Project saved successfully", ConsolePanel::LogType::Info);
			return true;
		}
		ConsolePanel::Log("Error during project saving", ConsolePanel::LogType::Error);
		return false;
	}

	Ref<Project> Project::Load(const std::filesystem::path& path)
	{
		Ref<Project> project = CreateRef<Project>();
		ProjectSerializer serializer(project);
		if (serializer.Deserialize(path))
		{
			project->m_ProjectDirectory = path.parent_path();
			s_ActiveProject = project;
			ConsolePanel::Log("Project loaded successfully", ConsolePanel::LogType::Info);
			return s_ActiveProject;
		}
		ConsolePanel::Log("Error during project loading", ConsolePanel::LogType::Error);
		return nullptr;
	}
}