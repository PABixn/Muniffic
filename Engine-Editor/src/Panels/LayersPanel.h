#pragma once

#include "Engine/Core/Application.h"
#include "../EditorLayer.h"
#include "Imgui/imgui.h"
#include "Engine/Project/Project.h"
#include "egpch.h"
#include <vector>
#include "ConsolePanel.h"

namespace eg {

	class LayersPanel {
	public:
		class LayerInfo {
		public:
			std::string name;
			int index;
			bool isVisible;
			LayerInfo(){
				this->index = LayersPanel::Layers.size();
				this->name = "New Layer " + std::to_string(this->index);
				this->isVisible = TRUE;
			}
		};
		static std::vector<LayerInfo*> Layers;
		void OnImGuiRender();
		void AddLayer();
	};

}