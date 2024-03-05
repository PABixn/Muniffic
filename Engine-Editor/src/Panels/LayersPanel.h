#pragma once

#include "Engine/Core/Application.h"
#include "../EditorLayer.h"
#include "Imgui/imgui.h"
#include "Engine/Project/Project.h"
#include "egpch.h"
#include <vector>
#include "ConsolePanel.h"
#include "Engine/Core/UUID.h"
#include "Engine.h"

namespace eg {

	class LayersPanel {
	public:
		class LayerInfo {
		public:
			std::string name;
			int index;
			bool isVisible;
			LayerInfo() {
				this->index = LayersPanel::Layers.size();
				this->name = "New Layer " + std::to_string(this->index);
				this->isVisible = TRUE;
			};
		};
		static std::vector<LayerInfo*> Layers;
		void OnImGuiRender();
		void AddLayer();
		void DeleteLayer(int selectedLayer);
		void RepairIndexes();
		std::string GetSelectedLayerName();
		void RenameLayer(std::string name);

		LayersPanel() = default;
		//LayersPanel(const Ref<Scene>& scene);
		//void SetContext(const Ref<Scene>& scene);
	private:
		int selectedLayer = 0;
		//Ref<Scene> m_Context;
	};

}