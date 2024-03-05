#include "DeleteFilePanel.h"

namespace eg
{
	DeleteFilePanel::DeleteFilePanel()
		: m_Show(false), m_UUID(0), m_Type(ResourceType::None)
	{ }

	void DeleteFilePanel::ShowWindow(UUID uuid, ResourceType type)
	{
		m_Show = true;
		m_UUID = uuid;
		m_Type = type;
	}

	void DeleteFilePanel::OnImGuiRender()
	{
		ImGui::SetNextWindowPos(ImVec2(Application::Get().GetWindow().GetWidth() / 2, Application::Get().GetWindow().GetHeight() / 2));

		ImGui::Begin("Delete file?", &m_Show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::Text("Warning: Choose delete method");
		if(ImGui::Button("Delete file from project"))
		{
			Commands::ExecuteDeleteResourceCommand(m_UUID, m_Type, false);
			m_Show = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete file from disk"))
		{
			Commands::ExecuteDeleteResourceCommand(m_UUID, m_Type, true);
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