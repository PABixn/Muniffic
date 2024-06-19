#include "WelcomingPanel.h"



namespace eg

	
{
	static float thumbnailSize = 128.0f;

	WelcomingPanel::WelcomingPanel() {}
	WelcomingPanel::WelcomingPanel(std::vector<std::string> list)
		: m_Show(true), m_DirectoryUUID(0), m_ProjectList(list)
	{
		m_DirectoryIcon = Texture2D::Create("resources/icons/contentBrowser/FileIcon.png");
		m_NewProjectCreated = false;
		m_SelectedProject = "";
	}

	void WelcomingPanel::ShowWindow(UUID directoryUUID)
	{
		m_Show = true;
		m_DirectoryUUID = directoryUUID;
	}


	void WelcomingPanel::OnImGuiRender()
	{
		ImGui::SetNextWindowPos(ImVec2(Application::Get().GetWindow().GetWidth() / 2, Application::Get().GetWindow().GetHeight() / 2));

		ImGui::Begin("WelcomingPanel", &m_Show , ImGuiWindowFlags_NoTitleBar| ImGuiViewportFlags_NoDecoration);
		ImGui::SetWindowFontScale(2.2);
		ImGui::Text("Muniffic Physics Engine");

		ImGui::SetWindowFontScale(1.0);

		ImGui::Begin("Menu", &m_Show, ImGuiWindowFlags_NoTitleBar| ImGuiViewportFlags_NoDecoration);
		ImGui::SetWindowFontScale(2.0);

		ImGui::Text("Menu");

		ImGui::SetWindowFontScale(1.0);

		Ref<Texture2D> icon = m_DirectoryIcon;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
		if (ImGui::Button("Open an existing project"))
		{
			m_Show = false;
		}
		//ImGui::SameLine();
		if (ImGui::Button("Create a new project"))
		{
			m_Show = false;
			m_NewProjectCreated = true;
		}
		ImGui::End();

		ImGui::Begin("Recent Projects", &m_Show, ImGuiWindowFlags_NoTitleBar| ImGuiViewportFlags_NoDecoration);
		ImGui::SetWindowFontScale(2.0);
		ImGui::Text("Recently opened projects");
		ImGui::SetWindowFontScale(1.0);
		if (m_ProjectList.size()) {
			for (auto project : m_ProjectList) {
				
				if (ImGui::Button(project.c_str()))
				{
					m_Show = false;
					m_SelectedProject = project;
				}
			}
		}
		else {
			ImGui::Text("You haven't opened any project yet");
		}
		ImGui::End();


		ImGui::End();
	}
}