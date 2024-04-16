#include "LayersPanel.h"

namespace eg {
	
    Ref<Scene> LayersPanel::m_Context;
    int LayersPanel::selectedLayer = 0;

    void LayersPanel::SetContext(const Ref<Scene>& scene) {
        LayersPanel::m_Context = scene;
    }

    void LayersPanel::RenameLayer(std::string name) {
        Renderer2D::SetLayerName(selectedLayer, name);
    };

    void LayersPanel::OnImGuiRender() {
        ImGui::Begin("Layers");
        for (int i = 0; i < Renderer2D::GetRendererData().layers.size();i++) {
            if (ImGui::Button(Renderer2D::GetLayerName(i).c_str())) {
                LayersPanel::selectedLayer = i;
                ConsolePanel::Log("Selected Layer: " + i, ConsolePanel::LogType::Info);
            };
            ImGui::SameLine();
            if (ImGui::Button("Visible")) {
				Renderer2D::SwitchLayerVisibility(i);
			};

        }
        if (ImGui::Button("New Layer")) {
            Renderer2D::CreateLayer();
        };
        ImGui::SameLine();
        if (ImGui::Button("Rename Layer")) {
			ImGui::OpenPopup("Rename Layer");
		};
        if (ImGui::BeginPopup("Rename Layer")) {
            static char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            std::strncpy(buffer, Renderer2D::GetRendererData().layers[selectedLayer].name.c_str(), sizeof(buffer));
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				Renderer2D::SetLayerName(selectedLayer, std::string(buffer));
			}
            if (ImGui::Button("Rename")) {
				ImGui::CloseCurrentPopup();
			};
			ImGui::EndPopup();
		};
        ImGui::SameLine();
        if (ImGui::Button("Delete Layer")) {
            m_LayerDeletingPanel->Show();
        };

        if (ImGui::Button("Up")) {
            Renderer2D::MoveLayerUp(LayersPanel::selectedLayer);
            LayersPanel::selectedLayer--;
		};
        ImGui::SameLine();
        if (ImGui::Button("Down")) {
            Renderer2D::MoveLayerDown(LayersPanel::selectedLayer);
            LayersPanel::selectedLayer++;
        };
        ImGui::Text("Selected Layer: %s", Renderer2D::GetRendererData().layers[selectedLayer].name.c_str());
        

        ImGui::End();  
    };

    void LayersPanel::CreateLayer() {
		Renderer2D::CreateLayer();
		ConsolePanel::Log("File: LayersPanel.cpp - Layer Created", ConsolePanel::LogType::Info);
	};

    void LayersPanel::DeleteLayer(int selectedLayer) {
        if (Renderer2D::GetRendererData().layers.size() > 1) {
            LayersPanel::SetSelectedLayer(0);
            LayersPanel::DestroyAllEntitiesInLayer(selectedLayer);
            LayersPanel::RepairEntityLayerIndexes(selectedLayer);
            Renderer2D::DeleteLayer(selectedLayer);
            ConsolePanel::Log("File: LayersPanel.cpp - Layer Deleted", ConsolePanel::LogType::Info);
        }
        else {
            ConsolePanel::Log("File: LayersPanel.cpp - Can't delete last layer", ConsolePanel::LogType::Error);
        };
    };

    void LayersPanel::DeleteLayer(int selectedLayer, int moveTo) {
        if (Renderer2D::GetRendererData().layers.size() > 1) {
			LayersPanel::MoveEntitiesToAnotherLayer(selectedLayer, moveTo);
			LayersPanel::RepairEntityLayerIndexes(selectedLayer);
			Renderer2D::DeleteLayer(selectedLayer);
			ConsolePanel::Log("File: LayersPanel.cpp - Layer Deleted", ConsolePanel::LogType::Info);
		}
        else {
			ConsolePanel::Log("File: LayersPanel.cpp - Can't delete last layer", ConsolePanel::LogType::Error);
		};
	};
    
    void LayersPanel::MoveEntityToLayer(UUID entityUUID, int layerIndex) {
        m_Context->GetEntityByUUID(entityUUID).GetEntityInfo()->m_Layer = layerIndex;
    }

    void LayersPanel::MoveEntitiesToAnotherLayer(int originalLayer, int newLayer) {
        for (auto& entityInfo : m_Context->GetEntityInfoMap()) {
            if (entityInfo.second->m_Layer == originalLayer) {
				LayersPanel::MoveEntityToLayer(entityInfo.first, newLayer);
			}
		}
	}
    
    void LayersPanel::DestroyAllEntitiesInLayer(int layerIndex) {
        for (auto& entityInfo : m_Context->GetEntityInfoMap()) {
            if (entityInfo.second->m_Layer == layerIndex) {
				m_Context->DestroyEntity(m_Context->GetEntityByUUID(entityInfo.first));
			}
		}
	}

    void LayersPanel::RepairEntityLayerIndexes(int layer) {
        for (auto& entityInfo : m_Context->GetEntityInfoMap()) {
            if (entityInfo.second->m_Layer > layer) {
                entityInfo.second->m_Layer--;
            }
        }
    }
    
    
	
}