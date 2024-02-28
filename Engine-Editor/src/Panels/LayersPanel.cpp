#include "LayersPanel.h"

namespace eg {
	
    std::vector<LayersPanel::LayerInfo*> LayersPanel::Layers;
    int selectedLayer;


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
        }
        if (ImGui::Button("New Layer")) {
            this->AddLayer();
        };
        ImGui::SameLine();
        if (ImGui::Button("Rename Layer")) {
            this->RenameLayer("Renamed");
        };
        ImGui::SameLine();
        if (ImGui::Button("Delete Layer")) {
            this->DeleteLayer(this->selectedLayer);
        };
        

        ImGui::End();  
    };


    void LayersPanel::AddLayer() {
        LayersPanel::Layers.push_back(new LayersPanel::LayerInfo());
        ConsolePanel::Log("File: LayersPanel.cpp - Layer Created", ConsolePanel::LogType::Info);
    };

    void LayersPanel::DeleteLayer(int selectedLayer) {
        if (LayersPanel::Layers.size() > 1) {
            LayersPanel::Layers.erase(LayersPanel::Layers.begin() + selectedLayer);
            this->RepairIndexes();
            ConsolePanel::Log("File: LayersPanel.cpp - Layer Deleted", ConsolePanel::LogType::Info);
        }
        else {
            ConsolePanel::Log("File: LayersPanel.cpp - Can't delete last layer", ConsolePanel::LogType::Error);
        };
	};

    
    
	
}