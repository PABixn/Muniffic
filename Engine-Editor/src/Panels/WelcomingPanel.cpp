#include "WelcomingPanel.h"
#include "../EditorLayer.h"

namespace eg

	

{	

	WelcomingPanel::WelcomingPanel() {}
	WelcomingPanel::WelcomingPanel(std::vector<std::string> list, RecentProjectSerializer m_RecentProjectSerializer)
		: m_Show(true), m_DirectoryUUID(0), m_ProjectList(list), m_Rps(m_RecentProjectSerializer)
	{
		m_NewProjectCreated = false;
		m_SelectedProject = "";
	}

	void WelcomingPanel::ShowWindow(UUID directoryUUID)
	{
		m_Show = true;
		m_DirectoryUUID = directoryUUID;
	}

	void WelcomingPanel::InitWelcomingPanel() {
		m_DirectoryIcon = Texture2D::Create("resources/icons/contentBrowser/FolderIcon.png");
		m_NewProjectIcon = Texture2D::Create("resources/icons/contentBrowser/NewProjectIcon.png");
		m_DeleteProjectIcon = Texture2D::Create("resources/icons/contentBrowser/DeleteProjectIcon.png");
	}


	void WelcomingPanel::OnImGuiRender()
	{
		ImGui::Begin("WelcomingPanel", &m_Show , ImGuiWindowFlags_NoTitleBar| ImGuiViewportFlags_NoDecoration);
		ImGui::PushFont(static_cast<EditorLayer*>(Application::Get().GetFirstLayer())->m_PoppinsExtraBoldFont);
		ImGui::Text("Muniffic Game Engine");
		ImGui::PopFont();

		ImGui::Dummy(ImVec2(0.0f, 30.0f));

		float windowWidth = ImGui::GetWindowWidth();
		ImGui::Columns(2, nullptr, false);
		ImGui::SetColumnWidth(0, windowWidth * 0.75f);

		//Begin recent project list
		ImGui::PushFont(static_cast<EditorLayer*>(Application::Get().GetFirstLayer())->m_PoppinsSemiBoldFontBig);
		ImGui::Text("Recently opened projects");
		ImGui::PopFont();
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
		int i = 0;
		if (m_ProjectList.size()) {
			for (auto project : m_ProjectList)
			{
				if (project != "")
				{
					if (ImGui::Button(project.c_str(), { ImGui::CalcTextSize(project.c_str())[0] + 80.0f, 34.0f }))
					{
						m_Show = false;
						m_SelectedProject = project;
					}
					
					ImGui::SameLine();

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
					if (ImGui::ImageButton((ImTextureID)m_DeleteProjectIcon->GetRendererID(), ImVec2(30.0f, 30.0f),ImVec2(0, 1), ImVec2(1, 0))) {
						auto it = find(m_ProjectList.begin(), m_ProjectList.end(), project);
						int projectId = it - m_ProjectList.begin();
						m_Rps.DeleteProject(projectId);
						m_ProjectList.erase(m_ProjectList.begin() + projectId);
					}
					ImGui::PopStyleColor(3);

					ImGui::SameLine();
					float cursorPos = ImGui::GetCursorPosX();
					ImGui::SetCursorPosX({ cursorPos - 39.0f });
					//ImGui::Image((ImTextureID)m_DeleteProjectIcon->GetRendererID(), { 28.0f, 28.0f }, { 0, 1 }, { 1, 0 });
					ImGui::SameLine();
					ImGui::SetCursorPosX({15.f});
					ImGui::Image((ImTextureID)m_DirectoryIcon->GetRendererID(), { 30.0f, 30.0f }, { 0, 1 }, { 1, 0 });
				}
				i++;
			}
		}
		else {
			ImGui::Text("You haven't opened any project yet");
		}

		ImGui::NextColumn();

		//Begin menu side
		ImGui::PushFont(static_cast<EditorLayer*>(Application::Get().GetFirstLayer())->m_PoppinsSemiBoldFontBig);
		ImGui::Text("Menu");
		ImGui::PopFont();
		float menuSize = windowWidth * 0.25f;

		ImGui::PushFont(static_cast<EditorLayer*>(Application::Get().GetFirstLayer())->m_PoppinsRegularFont);
		if (ImGui::Button("Open an existing project", { menuSize - 20.0f, 32.0f }))
		{
			m_Show = false;
		}
		ImGui::SameLine();
		float posX = ImGui::GetCursorPosX();
		posX -= (menuSize - 20.f);
		ImGui::SetCursorPosX(posX);
		ImGui::Image((ImTextureID)m_DirectoryIcon->GetRendererID(), { 30.0f, 30.0f }, { 0, 1 }, { 1, 0 });

		if (ImGui::Button("Create a new project", { menuSize - 20.0f, 32.0f }))
		{
			m_Show = false;
			m_NewProjectCreated = true;
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX(posX);
		ImGui::Image((ImTextureID)m_NewProjectIcon->GetRendererID(), { 32.0f, 32.0f }, { 0, 1 }, { 1, 0 });
		ImGui::PopFont();
		ImGui::PopStyleVar();

		ImGui::End();
	}
}