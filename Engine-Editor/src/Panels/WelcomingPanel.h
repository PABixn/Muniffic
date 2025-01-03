#pragma once

#include <filesystem>
#include "Engine/Core/Application.h"
#include "Imgui/imgui.h"
#include "../Commands/Commands.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Project/RecentProjectSerializer.h"


namespace eg
{
	class WelcomingPanel
	{
	public:
		WelcomingPanel();
		WelcomingPanel(std::vector<std::string>, RecentProjectSerializer);
		void OnImGuiRender();
		void ShowWindow(UUID directoryUUID);
		bool IsShown() { return m_Show; }
		bool isNewProjectCreated() { return m_NewProjectCreated; }
		std::string& getSelectedProject() { return m_SelectedProject; }
		void InitWelcomingPanel();

	protected:
		RecentProjectSerializer m_Rps;
		bool m_NewProjectCreated;
		bool m_Show;
		std::string m_SelectedProject;
		std::vector<std::string> m_ProjectList;
		UUID m_DirectoryUUID;
	};
}