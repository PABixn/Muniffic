#include "LayersPanel.h"

namespace eg {
	
    std::vector<LayersPanel::LayerInfo*> LayersPanel::Layers;
    
    void LayersPanel::OnImGuiRender() {
        ImGui::Begin("Layers");
        for (auto& layer : LayersPanel::Layers) {
            ImGui::Text(layer->name.c_str());
        }
        if (ImGui::Button("New Layer")) {
            this->AddLayer();
        };
        ImGui::End();  
    };

    void LayersPanel::AddLayer() {
        LayersPanel::Layers.push_back(new LayersPanel::LayerInfo());
        ConsolePanel::Log("File: LayersPanel.cpp - Layer Created", ConsolePanel::LogType::Info);
    }
	
}