#include "egpch.h"
#include "Project.h"
#include "../Engine-Editor/src/Panels/ConsolePanel.h"
#include "ProjectSerializer.h"

namespace eg {
	Project::Project(ProjectConfig projectConfig)
		: m_Config(projectConfig)
	{
		EG_PROFILE_FUNCTION();
	}

	Ref<Project> Project::New()
	{
		EG_PROFILE_FUNCTION();
		s_ActiveProject = CreateRef<Project>();

		ConsolePanel::Log("File: Project.cpp - Project created successfully", ConsolePanel::LogType::Info);
		return s_ActiveProject;
	}

	bool Project::Save(const std::filesystem::path& path)
	{
		EG_PROFILE_FUNCTION();
		ProjectSerializer serializer(s_ActiveProject);
		if (serializer.Serialize(path))
		{
			s_ActiveProject->m_ProjectDirectory = path.parent_path();
			ConsolePanel::Log("File: Project.cpp - Project saved successfully", ConsolePanel::LogType::Info);
			return true;
		}
		ConsolePanel::Log("File: Project.cpp - Error during project saving", ConsolePanel::LogType::Error);
		return false;
	}

	Ref<Project> Project::Load(const std::filesystem::path& path)
	{
		EG_PROFILE_FUNCTION();
		Ref<Project> project = CreateRef<Project>();
		ProjectSerializer serializer(project);
		if (serializer.Deserialize(path))
		{
			project->m_ProjectDirectory = path.parent_path();
			s_ActiveProject = project;
			ConsolePanel::Log("File: Project.cpp - Project loaded successfully", ConsolePanel::LogType::Info);
			return s_ActiveProject;
		}
		ConsolePanel::Log("File: Project.cpp - Error during project loading", ConsolePanel::LogType::Error);
		return nullptr;
	}
}
