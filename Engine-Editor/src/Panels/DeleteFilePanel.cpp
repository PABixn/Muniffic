#include "DeleteFilePanel.h"

namespace eg
{
	DeleteFilePanel::DeleteFilePanel()
		: m_Result(FileDeleteMethod::Cancel), m_Show(false)
	{ }

	void DeleteFilePanel::OnImGuiRender()
	{
		ImGui::SetNextWindowPos(ImVec2(Application::Get().GetWindow().GetWidth() / 2, Application::Get().GetWindow().GetHeight() / 2));

		ImGui::Begin("Delete file?", &m_Show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::Text("Warning: Choose delete method");
		if(ImGui::Button("Delete file from project"))
		{
			m_Result = FileDeleteMethod::DeleteFromProject;
			m_Show = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete file from disk"))
		{
			m_Result = FileDeleteMethod::DeleteFromDisk;
			m_Show = false;
		}
		ImGui::SameLine();
		if(ImGui::Button("Cancel"))
		{
			m_Show = false;
		}
		ImGui::End();
	}
}