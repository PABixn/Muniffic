#include "RenameFolderPanel.h"
#include "Engine/Resources/ResourceDatabase.h"

namespace eg
{
	RenameFolderPanel::RenameFolderPanel()
		: m_Show(false), m_Path(std::filesystem::path())
	{ }

	void RenameFolderPanel::ShowWindow(std::filesystem::path path)
	{
		m_Show = true;
		m_Path = path;
	}

	void RenameFolderPanel::OnImGuiRender()
	{
		ImGui::SetNextWindowPos(ImVec2(Application::Get().GetWindow().GetWidth() / 2, Application::Get().GetWindow().GetHeight() / 2));

		ImGui::Begin("Rename folder", &m_Show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::Text("Enter new folder name");
		static char buffer[256];
		ImGui::InputText("##FolderName", buffer, 256);
		if (ImGui::Button("Rename"))
		{
			std::filesystem::path newPath = m_Path.parent_path() / buffer;
			ResourceDatabase::RenameResource(m_Path, newPath);
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