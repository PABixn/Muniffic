#include "RenameResourcePanel.h"
#include "Engine/Resources/ResourceDatabase.h"

namespace eg
{
	RenameResourcePanel::RenameResourcePanel()
		: m_Show(false), m_UUID(0)
	{ }

	void RenameResourcePanel::ShowWindow(UUID uuid)
	{
		m_UUID = uuid;
		m_Show = true;
	}

	void RenameResourcePanel::OnImGuiRender()
	{
		ImGui::SetNextWindowPos(ImVec2(Application::Get().GetWindow().GetWidth() / 2, Application::Get().GetWindow().GetHeight() / 2));

		ImGui::Begin("Rename resource", &m_Show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::Text("Enter new resource name");
		static char buffer[256];
		ImGui::InputText("##ResourceName", buffer, 256);
		if (ImGui::Button("Rename"))
		{
			ResourceDatabase::RenameResource(m_UUID, buffer);
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