#include "NameNewProjectPanel.h"
#include "Engine/Resources/ResourceDatabase.h"
#include "../../Engine-Editor/src/Commands/Commands.h"
#include <vector>

namespace eg
{
	NameNewProjectPanel::NameNewProjectPanel()
		: m_Show(false), m_ProjectUUID(0)
	{
	    EG_PROFILE_FUNCTION();
	}

	void NameNewProjectPanel::ShowWindow(UUID directoryUUID)
	{
		EG_PROFILE_FUNCTION();
		m_ProjectUUID = directoryUUID;
		m_Show = true;
	}

	char correctChar(char& s) {
        EG_PROFILE_FUNCTION();
		if (s == ' ') {
			return '_';
		}
		return s;
	}

	bool NameNewProjectPanel::isNameAllowed(char* name)
	{
        EG_PROFILE_FUNCTION();
		while (*name != '\0') {
			if (correctChar(*name) != *name)
			{
				return false;
			}
			name++;
		}
		return true;
	}

	std::vector<char> NameNewProjectPanel::makeNameAllowed(char* name)
	{
        EG_PROFILE_FUNCTION();
		std::vector<char> correctedName = std::vector<char>();
		while (*name != '\0') {
			correctedName.push_back(correctChar(*name));
			name++;
		}
		correctedName.push_back(correctChar(*name));
		return correctedName;
	}

	void NameNewProjectPanel::OnImGuiRender()
	{
        EG_PROFILE_FUNCTION();
		ImGui::SetNextWindowPos(ImVec2(Application::Get().GetWindow().GetWidth() / 2, Application::Get().GetWindow().GetHeight() / 2));

		ImGui::Begin("Name project", &m_Show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::Text("Enter new project name");
		static char buffer[256];
		ImGui::InputText("##ProjectName", buffer, 256);
		if (ImGui::Button("Save"))
		{
			if (isNameAllowed(buffer))
			{
				projectName = buffer;
				m_GiveProjectName = false;
				m_Show = false;
			}
			else
			{
				ImGui::OpenPopup("Project name not allowed");
			}
		}
		if (ImGui::BeginPopupModal("Project name not allowed"))
		{
			ImGui::Text("Project name contains forbidden characters. \nThe project will be saved as: ");
			std::vector<char> correctedName= makeNameAllowed(buffer);
			ImGui::Text(correctedName.data());
			if (ImGui::Button("ok")) {
				projectName = correctedName.data();
				m_GiveProjectName = false;
				m_Show = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::End();
	}
}