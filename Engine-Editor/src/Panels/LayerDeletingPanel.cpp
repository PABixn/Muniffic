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
			for (auto& layer : LayersPanel::Layers) {
				if (layer->index != LayersPanel::GetSelectedLayer()) {
					if (ImGui::Button(layer->name.c_str())) {
						LayersPanel::DeleteLayer(LayersPanel::GetSelectedLayer(), layer->index);
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