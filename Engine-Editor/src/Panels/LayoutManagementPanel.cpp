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
		for (auto& element : LayoutManagementPanel::Huds) {
			if (ImGui::Button(element->name.c_str())) {

			};
		};
		ImGui::End();
	}

	


}