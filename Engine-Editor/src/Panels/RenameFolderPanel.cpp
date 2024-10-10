#include "RenameFolderPanel.h"
#include "Engine/Resources/ResourceDatabase.h"
#include "../../Engine-Editor/src/Commands/Commands.h"

namespace eg
{
	RenameFolderPanel::RenameFolderPanel()
		: m_Show(false), m_DirectoryUUID(0)
	{
	    EG_PROFILE_FUNCTION();
	}

	void RenameFolderPanel::ShowWindow(UUID directoryUUID)
	{
		EG_PROFILE_FUNCTION();
		m_DirectoryUUID = directoryUUID;
		m_Show = true;
	}

	void RenameFolderPanel::OnImGuiRender()
	{
        EG_PROFILE_FUNCTION();
		ImGui::SetNextWindowPos(ImVec2(Application::Get().GetWindow().GetWidth() / 2, Application::Get().GetWindow().GetHeight() / 2));

		ImGui::Begin("Rename folder", &m_Show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::Text("Enter new folder name");
		static char buffer[256];
		ImGui::InputText("##FolderName", buffer, 256);
		if (ImGui::Button("Rename"))
		{
			Commands::ExecuteRenameDirectoryCommand(m_DirectoryUUID, buffer);
			//ResourceDatabase::RenameDirectory(m_Path, buffer);
			m_Show = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			m_Show = false;
		}
		ImGui::End();
	}
}