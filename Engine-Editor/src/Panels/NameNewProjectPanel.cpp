#include "NameNewProjectPanel.h"
#include "Engine/Resources/ResourceDatabase.h"
#include "../../Engine-Editor/src/Commands/Commands.h"

namespace eg
{
	NameNewProjectPanel::NameNewProjectPanel()
		: m_Show(false), m_ProjectUUID(0)
	{ }

	void NameNewProjectPanel::ShowWindow(UUID directoryUUID)
	{
		m_ProjectUUID = directoryUUID;
		m_Show = true;
	}

	void NameNewProjectPanel::OnImGuiRender()
	{
		ImGui::SetNextWindowPos(ImVec2(Application::Get().GetWindow().GetWidth() / 2, Application::Get().GetWindow().GetHeight() / 2));

		ImGui::Begin("Name project", &m_Show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::Text("Enter new project name");
		static char buffer[256];
		ImGui::InputText("##ProjectName", buffer, 256);
		if (ImGui::Button("Save"))
		{
			projectName = buffer;
			m_GiveProjectName = false;
			
			
			//Commands::ExecuteRenameDirectoryCommand(m_ProjectUUID, buffer);
			//ResourceDatabase::RenameDirectory(m_Path, buffer);
			m_Show = false;
		}
		ImGui::End();
	}
}