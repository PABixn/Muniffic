#include "InvalidDataPanel.h"

namespace eg {
	InvalidDataPanel::InvalidDataPanel()
		: m_Show(false)
	{ }

	void InvalidDataPanel::ShowWindow()
	{
		m_Show = true;
	}

	void InvalidDataPanel::OnImGuiRender()
	{

		ImGui::SetNextWindowPos(ImVec2(Application::Get().GetWindow().GetWidth() / 2, Application::Get().GetWindow().GetHeight() / 2));

		ImGui::Begin("InvalidData", &m_Show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::Text("Invalid Data!");
		
		if (ImGui::Button("OK"))
		{
			m_Show = false;
		}
		ImGui::End();
	}
}