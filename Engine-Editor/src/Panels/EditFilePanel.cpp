#include "EditFilePanel.h"

namespace eg {
	EditFilePanel::EditFilePanel()
		: m_Show(false), m_Path(std::filesystem::path()), m_Type(ResourceType::None)
	{ 
	}

	void EditFilePanel::ShowWindow(std::filesystem::path path, ResourceType type)
	{
		m_Show = true;
		m_Path = path;
		m_Type = type;
		m_SpriteSheet = eg::Texture2D::Create(m_Path.string());
		m_SpriteSize = { m_SpriteSheet->GetWidth(), m_SpriteSheet->GetHeight()};
		m_Sprites = {};
	}

	void EditFilePanel::OnImGuiRender()
	{
		constexpr float EDIT_PANEL_HEIGHT = 190.0f;
		constexpr float EDIT_PANEL_WIDTH = 210.0f;

		ImGui::SetNextWindowPos(ImVec2(((Application::Get().GetWindow().GetWidth() / 2) - EDIT_PANEL_WIDTH / 2), ((Application::Get().GetWindow().GetHeight() / 2)) - EDIT_PANEL_HEIGHT / 2));
		ImGui::SetNextWindowSize({EDIT_PANEL_WIDTH, EDIT_PANEL_HEIGHT});

		ImGui::Begin("Set sprite parameters", &m_Show, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::Text("Create sprites");
		ImGui::Text("Width(px): ");
		ImGui::InputFloat("##width",&m_SpriteSize[0], 1, 100);
		ImGui::Text("Height(px): ");
		ImGui::InputFloat("##height", &m_SpriteSize[1], 1, 100);
		if (ImGui::Button("Submit"))
		{
			if (m_SpriteSize[0] > 0 && m_SpriteSize[0] < m_SpriteSheet->GetWidth() && m_SpriteSize[1] > 0 && m_SpriteSize[1] < m_SpriteSheet->GetHeight()) {
				/*for (int i = 0; i < m_SpriteSheet->GetWidth(); i += m_SpriteSize[0])
				{
					for (int j = 0; j < m_SpriteSheet->GetHeight(); j += m_SpriteSize[1]) 
					{
						m_Sprites.push_back(SubTexture2D::CreateFromCoords(m_SpriteSheet, { i, j }, m_SpriteSize));
					}
				}*/
				m_Show = false;
			}
			else {
				m_InvalidDataPanel->ShowWindow();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			m_Show = false;
		}
		ImGui::End();
	}
}