#include "LayoutManagementPanel.h"

namespace eg {

	std::vector<Hud*> LayoutManagementPanel::Huds;

	void LayoutManagementPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
	}

	void LayoutManagementPanel::CreateHUD() {
		LayoutManagementPanel::Huds.push_back(new Hud());
	}

	void LayoutManagementPanel::OnImGuiRender() {
		ImGui::Begin("Layout");
		ImGui::Text("Layout Management");
		if (ImGui::Button("Create Layout")) {
			CreateHUD();
		};
		if (m_Context->GetActiveHud() == nullptr) {
			for (auto& element : LayoutManagementPanel::Huds) {
				if (ImGui::Button(element->name.c_str())) {
					m_Context->SetActiveHud(element);
				};
			};
		}
		else {
			ImGui::Text("Active Layout: %s", m_Context->GetActiveHud()->name.c_str());
			ImGui::SameLine();
			if (ImGui::Button("Close Layout")) {
				m_Context->SetActiveHud(nullptr);
			};
			if (ImGui::Button("Add Element")) {
				m_Context->GetActiveHud()->CreateElement();
			};
			for (auto& element : m_Context->GetActiveHud()->elements) {
				ImGui::Text("Element: %s", element->name.c_str());
				//ImGui::SliderFloat("X", &element->GetX(), 0.0f, 1920.0f);
				//ImGui::SliderFloat("Y", &element->GetY(), 0.0f, 1080.0f);
				//ImGui::SliderFloat("Width", &element->GetWidth(), 0.0f, 1920.0f);
				//ImGui::SliderFloat("Height", &element->GetHeight(), 0.0f, 1080.0f);
				//ImGui::ColorEdit4("Color", glm::value_ptr(element->GetColor()));
			};
		};
		ImGui::End();
	}

	


}