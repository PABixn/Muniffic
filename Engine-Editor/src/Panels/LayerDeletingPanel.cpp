#include "LayerDeletingPanel.h"

namespace eg {
	void LayerDeletingPanel::OnImGuiRender() {
		ImGui::SetNextWindowPos(ImVec2(Application::Get().GetWindow().GetWidth() / 2, Application::Get().GetWindow().GetHeight() / 2));
		ImGui::Begin("Layer Deleting", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

		ImGui::Text("Do you want to delete all entities with layer, or move them to another layer?");

		if (ImGui::Button("DELETE")) {
			LayersPanel::DeleteLayer(LayersPanel::GetSelectedLayer());
			m_Show = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("MOVE")) {
			ImGui::OpenPopup("Move");
		}
		if (ImGui::BeginPopup("Move")) {
			for (int i = 0; i< Renderer2D::GetRendererData().layers.size(); i++) {
				if (i != LayersPanel::GetSelectedLayer()) {
					if (ImGui::Button(Renderer2D::GetRendererData().layers[i].name.c_str())) {
						LayersPanel::DeleteLayer(LayersPanel::GetSelectedLayer(), i);
						ImGui::CloseCurrentPopup();
						m_Show = false;
					}
				}
			}
			ImGui::EndPopup();
		}
		ImGui::End();
	}
}