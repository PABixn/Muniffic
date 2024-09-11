#include "CreateDirectoryPanel.h"

namespace eg
{
	CreateDirectoryPanel::CreateDirectoryPanel()
		: m_Show(false), m_DirectoryUUID(0)
	{ }

	void CreateDirectoryPanel::ShowWindow(UUID parentUUID)
	{
		m_Show = true;
		m_DirectoryUUID = parentUUID;
	}

	void CreateDirectoryPanel::OnImGuiRender()
	{
		ImGui::SetNextWindowPos(ImVec2(Application::Get().GetWindow().GetWidth() / 2, Application::Get().GetWindow().GetHeight() / 2));

		ImGui::Begin("Create directory?", &m_Show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

		ImGui::Text("Enter new folder name");
		static char buffer[256] = "New Folder";
		ImGui::InputText("##FolderName", buffer, 256);

		if (ImGui::Button("Create directory"))
		{
			Commands::ExecuteCreateDirectoryCommand(buffer, m_DirectoryUUID);
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