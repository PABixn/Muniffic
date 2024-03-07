#include "LayersPanel.h"

namespace eg {
	
    Ref<Scene> LayersPanel::m_Context;
    std::vector<LayersPanel::LayerInfo*> LayersPanel::Layers;
    int LayersPanel::selectedLayer = 0;

    void LayersPanel::SetContext(const Ref<Scene>& scene) {
        LayersPanel::m_Context = scene;
    }

    void LayersPanel::RepairIndexes() {
        for (int i = 0; i < LayersPanel::Layers.size(); i++) {
            LayersPanel::Layers[i]->index = i;
            for (int j = 0; j < LayersPanel::Layers[i]->entitiesUUID.size(); j++) {
                m_Context->GetEntityByUUID(LayersPanel::Layers[i]->entitiesUUID[j]).GetEntityInfo()->m_Layer = i;
            }
        }
    };
    void LayersPanel::RenameLayer(std::string name) {
        LayersPanel::Layers[selectedLayer]->name = name;
    };

    std::string LayersPanel::GetSelectedLayerName() {
        return LayersPanel::Layers[selectedLayer]->name;
    };


    void LayersPanel::OnImGuiRender() {
        ImGui::Begin("Layers");
        if (LayersPanel::Layers.size() == 0) {
			this->AddLayer();
            this->AssignLayerForExcistingEntities();
		}
        for (auto& layer : LayersPanel::Layers) {
            if (ImGui::Button(layer->name.c_str())) {
                LayersPanel::selectedLayer = layer->index;
                ConsolePanel::Log("Selected Layer: " + layer->name, ConsolePanel::LogType::Info);
            };
            ImGui::SameLine();
            if (ImGui::Button("Visible")) {
				layer->isVisible = !layer->isVisible;
			};

        }
        if (ImGui::Button("New Layer")) {
            this->AddLayer();
        };
        ImGui::SameLine();
        if (ImGui::Button("Rename Layer")) {
			ImGui::OpenPopup("Rename Layer");
		};
        if (ImGui::BeginPopup("Rename Layer")) {
            static char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            std::strncpy(buffer, LayersPanel::Layers[LayersPanel::selectedLayer]->name.c_str(), sizeof(buffer));
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				LayersPanel::Layers[LayersPanel::selectedLayer]->name = std::string(buffer);
				std::cout << LayersPanel::Layers[LayersPanel::selectedLayer]->name << std::endl;
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
            if (LayersPanel::selectedLayer > 0) {
                std::swap(LayersPanel::Layers[LayersPanel::selectedLayer], LayersPanel::Layers[LayersPanel::selectedLayer - 1]);
                LayersPanel::selectedLayer--;
                LayersPanel::RepairIndexes();
            };
		};
        ImGui::SameLine();
        if (ImGui::Button("Down")) {
            if (LayersPanel::selectedLayer < LayersPanel::Layers.size() - 1) {
                std::swap(LayersPanel::Layers[LayersPanel::selectedLayer], LayersPanel::Layers[LayersPanel::selectedLayer + 1]);
                LayersPanel::selectedLayer++;
                LayersPanel::RepairIndexes();
            };
        };
        ImGui::Text("Selected Layer: %s", LayersPanel::Layers[LayersPanel::selectedLayer]->name.c_str());
        ImGui::Text(("Selected Layer Index: " + std::to_string(LayersPanel::Layers[LayersPanel::selectedLayer]->index)).c_str());
        

        ImGui::End();  
    };

    void LayersPanel::AssignLayerForExcistingEntities() {
        for (auto& pair : m_Context->GetEntityMap()) {
            LayersPanel::Layers[0]->AddEntity(pair.first);
        }
    }

    void LayersPanel::AddLayer() {
        LayersPanel::Layers.push_back(new LayersPanel::LayerInfo());
        ConsolePanel::Log("File: LayersPanel.cpp - Layer Created", ConsolePanel::LogType::Info);
    };

    void LayersPanel::DeleteLayer(int selectedLayer) {
        if (LayersPanel::Layers.size() > 1) {
            LayersPanel::selectedLayer = 0;
            LayersPanel::Layers[selectedLayer]->DeleteAllEntities();
            LayersPanel::Layers.erase(LayersPanel::Layers.begin() + selectedLayer);
            LayersPanel::RepairIndexes();
            ConsolePanel::Log("File: LayersPanel.cpp - Layer Deleted", ConsolePanel::LogType::Info);
        }
        else {
            ConsolePanel::Log("File: LayersPanel.cpp - Can't delete last layer", ConsolePanel::LogType::Error);
        };
	};

    void LayersPanel::DeleteLayer(int selectedLayer, int moveTo) {
        if (LayersPanel::Layers.size() > 1) {
            LayersPanel::selectedLayer = 0;
            LayersPanel::Layers[selectedLayer]->MoveAllEntitiesToLayer(moveTo);
            LayersPanel::Layers.erase(LayersPanel::Layers.begin() + selectedLayer);
            LayersPanel::RepairIndexes();
            ConsolePanel::Log("File: LayersPanel.cpp - Layer Deleted", ConsolePanel::LogType::Info);
        }
        else {
            ConsolePanel::Log("File: LayersPanel.cpp - Can't delete last layer", ConsolePanel::LogType::Error);
        };
    };

    
    
	
}