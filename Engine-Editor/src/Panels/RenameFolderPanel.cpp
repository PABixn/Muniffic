#include "RenameFolderPanel.h"
#include "Engine/Resources/ResourceDatabase.h"
#include "../../Engine-Editor/src/Commands/Commands.h"

namespace eg
{
	RenameFolderPanel::RenameFolderPanel()
		: m_Show(false), m_Path(std::filesystem::path())
	{ }

	void RenameFolderPanel::ShowWindow(std::filesystem::path path)
	{
		m_Path = path;
		m_Show = true;
	}

	void RenameFolderPanel::OnImGuiRender()
	{
		ImGui::SetNextWindowPos(ImVec2(Application::Get().GetWindow().GetWidth() / 2, Application::Get().GetWindow().GetHeight() / 2));

		ImGui::Begin("Rename folder", &m_Show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::Text("Enter new folder name");
		char buffer[256];
		ImGui::InputText("##FolderName", buffer, 256);
		if (ImGui::Button("Rename"))
		{
			Commands::ExecuteRenameDirectoryCommand(m_Path, buffer);
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