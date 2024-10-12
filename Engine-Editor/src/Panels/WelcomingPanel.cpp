#include "WelcomingPanel.h"
#include <src/MarkdownRenderer/Markdown.h>


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
		m_DirectoryIcon = Texture2D::Create("resources/icons/contentBrowser/DirectoryIcon.png");
		m_NewProjectIcon = Texture2D::Create("resources/icons/contentBrowser/NewProjectIcon.png");
		m_DeleteProjectIcon = Texture2D::Create("resources/icons/contentBrowser/DeleteProjectIcon.png");
	}


	void WelcomingPanel::OnImGuiRender()
	{
		ImGui::Begin("WelcomingPanel", &m_Show , ImGuiWindowFlags_NoTitleBar| ImGuiViewportFlags_NoDecoration);
		ImGui::SetWindowFontScale(2.2);
		ImGui::Text("Muniffic Physics Engine");

		ImGui::SetWindowFontScale(1.0);

		ImGui::Begin("Menu", &m_Show, ImGuiWindowFlags_NoTitleBar| ImGuiViewportFlags_NoDecoration);
		ImGui::SetWindowFontScale(2.0);

		ImGui::Text("Menu");

		ImGui::SetWindowFontScale(1.0);

		Ref<Texture2D> directoryIcon = m_DirectoryIcon;
		Ref<Texture2D> newProjectIcon = m_NewProjectIcon;
		Ref<Texture2D> deleteProjectIcon = m_DeleteProjectIcon;
		
		if (ImGui::Button("Open an existing project", { ImGui::GetWindowWidth() - 20.0f, 32.0f}))
		{
			m_Show = false;
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX({ 8.0f });
		ImGui::Image((ImTextureID)directoryIcon->GetRendererID(), { 32.0f, 32.0f }, { 0, 1 }, { 1, 0 });
		
		if (ImGui::Button("Create a new project", { ImGui::GetWindowWidth() - 20.0f, 32.0f}))
		{
			m_Show = false;
			m_NewProjectCreated = true;
		}
		ImGui::SameLine();
		ImGui::SetCursorPosX({ 8.0f });
		ImGui::Image((ImTextureID)newProjectIcon->GetRendererID(), { 30.0f, 30.0f }, { 0, 1 }, { 1, 0 });

		ImGui::End();

		ImGui::Begin("Recent Projects", &m_Show, ImGuiWindowFlags_NoTitleBar| ImGuiViewportFlags_NoDecoration);
		ImGui::SetWindowFontScale(2.0);
		ImGui::Text("Recently opened projects");
		ImGui::SetWindowFontScale(1.0);
		int i = 0;
		if (m_ProjectList.size()) {
			for (auto project : m_ProjectList)
			{
				if (project != "")
				{
					if (ImGui::Button(project.c_str(), { ImGui::CalcTextSize(project.c_str())[0] + 80.0f, 32.0f }))
					{
						m_Show = false;
						m_SelectedProject = project;
					}
					
					ImGui::SameLine();

					ImGui::SetWindowFontScale(0.1f);

					if (ImGui::Button(std::to_string(i).c_str(), {32.0f, 32.0f}))
					{
						auto it = find(m_ProjectList.begin(), m_ProjectList.end(), project);
						int projectId = it - m_ProjectList.begin();
						m_Rps.DeleteProject(projectId);
						m_ProjectList.erase(m_ProjectList.begin() + projectId);

					}

					ImGui::SetWindowFontScale(1.0f);


					ImGui::SameLine();
					float cursorPos = ImGui::GetCursorPosX();
					ImGui::SetCursorPosX({ cursorPos - 39.0f });
					ImGui::Image((ImTextureID)deleteProjectIcon->GetRendererID(), { 30.0f, 30.0f }, { 0, 1 }, { 1, 0 });

					
					ImGui::SameLine();
					ImGui::SetCursorPosX({ 8.0f });
					ImGui::Image((ImTextureID)directoryIcon->GetRendererID(), { 32.0f, 32.0f }, { 0, 1 }, { 1, 0 });
				}
				i++;
			}
		}
		else
		{
			ImGui::Text("You haven't opened any project yet");
		}
		ImGui::End();


		ImGui::End();
	}
}