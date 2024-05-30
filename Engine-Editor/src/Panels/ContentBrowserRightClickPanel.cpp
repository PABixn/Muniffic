#include "ContentBrowserRightClickPanel.h"
#include "Imgui/imgui.h"
#include "../EditorLayer.h"

namespace eg {
	bool ContentBrowserRightClickPanel::OpenContentBrowserRightClickPanel(UUID key, float posX, float posY) {
		ShowRightClickPanel(true);
		m_key = key;
		m_posX = posX;
		m_posY = posY;
		return true;
	}

	void ContentBrowserRightClickPanel::OnImGuiRender() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
		ImGui::Begin("RightClickPanel", nullptr, ImGuiWindowFlags_NoDecoration);
		ImGui::SetWindowSize(ImVec2(200, 50));
		ImGui::SetWindowPos(ImVec2(m_posX,m_posY));

		ImGui::GetStyle().Colors[ImGuiCol_Button] = ImVec4(0, 0, 0, 0);
		ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = ImVec4(0, 0, 0, 0);
		ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = ImVec4(0, 0, 0, 0);
		ImGui::GetStyle().Colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);

		EditorLayer* e = (EditorLayer*)Application::Get().GetFirstLayer();
		if (ImGui::Button("Open animation editor panel")) {
			e->GetAnimationEditorPanel()->OpenAnimationEditorPanel(m_key);
		}
		ImGui::End();
		ImGui::PopStyleVar();
		e->GetAnimationEditorPanel()->OnImGuiRender();
	}

}