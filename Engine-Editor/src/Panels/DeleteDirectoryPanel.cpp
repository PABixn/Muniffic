#include "DeleteDirectoryPanel.h"


namespace eg
{
	DeleteDirectoryPanel::DeleteDirectoryPanel()
	: m_Show(false), m_DirectoryUUID(0)
	{
        EG_PROFILE_FUNCTION();
    }

	void DeleteDirectoryPanel::ShowWindow(UUID directoryUUID)
	{
        EG_PROFILE_FUNCTION();
		m_Show = true;
		m_DirectoryUUID = directoryUUID;
	}

	void DeleteDirectoryPanel::OnImGuiRender()
	{
        EG_PROFILE_FUNCTION();
		ImGui::SetNextWindowPos(ImVec2(Application::Get().GetWindow().GetWidth() / 2, Application::Get().GetWindow().GetHeight() / 2));

		ImGui::Begin("Delete directory?", &m_Show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::Text("Warning: You will not be able to restore files");

		if (ImGui::Button("Delete directory AND ALL FILES from disk"))
		{
			Commands::ExecuteDeleteDirectoryCommand(m_DirectoryUUID);
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
