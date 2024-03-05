#include "LayersPanel.h"

namespace eg {
	
    std::vector<LayersPanel::LayerInfo*> LayersPanel::Layers;
    int selectedLayer;

    /*void LayersPanel::SetContext(const Ref<Scene>& scene) {
        this->m_Context = scene;
    };*/

    /*LayersPanel::LayersPanel(const Ref<Scene>& scene) {
        SetContext(scene);
	};*/

    //LayersPanel::LayersPanel() {};

    void LayersPanel::RepairIndexes() {
        for (int i = 0; i < LayersPanel::Layers.size(); i++) {
            LayersPanel::Layers[i]->index = i;
        }
    };
    void LayersPanel::RenameLayer(std::string name) {
        LayersPanel::Layers[selectedLayer]->name = name;
    };

    std::string LayersPanel::GetSelectedLayerName() {
        return LayersPanel::Layers[selectedLayer]->name;
    }

    void LayersPanel::OnImGuiRender() {
        ImGui::Begin("Layers");
        if (LayersPanel::Layers.size() == 0) {
			this->AddLayer();
		}
        for (auto& layer : LayersPanel::Layers) {
            if (ImGui::Button(layer->name.c_str())) {
                selectedLayer = layer->index;
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
            std::strncpy(buffer, LayersPanel::Layers[selectedLayer]->name.c_str(), sizeof(buffer));
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				LayersPanel::Layers[selectedLayer]->name = std::string(buffer);
				std::cout << LayersPanel::Layers[selectedLayer]->name << std::endl;
			}
            if (ImGui::Button("Rename")) {
				ImGui::CloseCurrentPopup();
			};
			ImGui::EndPopup();
		};
        ImGui::SameLine();
        if (ImGui::Button("Delete Layer")) {
            this->DeleteLayer(this->selectedLayer);
        };

        if (ImGui::Button("Up")) {
            if (selectedLayer > 0) {
                std::swap(LayersPanel::Layers[selectedLayer], LayersPanel::Layers[selectedLayer - 1]);
                selectedLayer--;
                this->RepairIndexes();
            };
		};
        ImGui::SameLine();
        if (ImGui::Button("Down")) {
            if (selectedLayer < LayersPanel::Layers.size() - 1) {
                std::swap(LayersPanel::Layers[selectedLayer], LayersPanel::Layers[selectedLayer + 1]);
                selectedLayer++;
                this->RepairIndexes();
            };
        };
        ImGui::Text("Selected Layer: %s", LayersPanel::Layers[selectedLayer]->name.c_str());
        ImGui::Text(("Selected Layer Index: " + std::to_string(LayersPanel::Layers[selectedLayer]->index)).c_str());
        

        ImGui::End();  
    };


    void LayersPanel::AddLayer() {
        LayersPanel::Layers.push_back(new LayersPanel::LayerInfo());
        ConsolePanel::Log("File: LayersPanel.cpp - Layer Created", ConsolePanel::LogType::Info);
    };

    void LayersPanel::DeleteLayer(int selectedLayer) {
        if (LayersPanel::Layers.size() > 1) {
            //window to choose what to do with entities
            /*for (auto& pair : m_Context->GetEntityInfoMap()) {
                if (pair.second->m_Layer == selectedLayer) {
                    m_Context->DestroyEntity(m_Context->GetEntityByUUID(pair.first));
				}
            }*/
            LayersPanel::Layers.erase(LayersPanel::Layers.begin() + selectedLayer);
            this->RepairIndexes();
            ConsolePanel::Log("File: LayersPanel.cpp - Layer Deleted", ConsolePanel::LogType::Info);
        }
        else {
            ConsolePanel::Log("File: LayersPanel.cpp - Can't delete last layer", ConsolePanel::LogType::Error);
        };
	};

    
    
	
}