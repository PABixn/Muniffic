#pragma once

#include <filesystem>
#include "Engine/Core/Application.h"
#include "Imgui/imgui.h"
#include "../Commands/Commands.h"
#include "Engine/Renderer/Texture.h"


namespace eg
{
	class WelcomingPanel
	{
	public:
		WelcomingPanel();
		WelcomingPanel(std::vector<std::string>);
		void OnImGuiRender();
		void ShowWindow(UUID directoryUUID);
		bool IsShown() { return m_Show; }
		bool isNewProjectCreated() { return m_NewProjectCreated; }
		std::string& getSelectedProject() { return m_SelectedProject; }

	protected:
		Ref<Texture2D> m_DirectoryIcon;
		bool m_NewProjectCreated;
		bool m_Show;
		std::string m_SelectedProject;
		std::vector<std::string> m_ProjectList;
		UUID m_DirectoryUUID;
	};
}