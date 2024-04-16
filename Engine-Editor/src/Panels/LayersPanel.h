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
#include "Engine/Scene/Scene.h"
#include "Engine/Renderer/Renderer2D.h"
#include "LayerDeletingPanel.h"

namespace eg {

	class LayersPanel {
	public:
		friend class LayerDeletingPanel;
		void OnImGuiRender();
		void RenameLayer(std::string name);
		void SetContext(const Ref<Scene>& scene);
		static int GetSelectedLayer() {
			return selectedLayer;
		};
		static void SetSelectedLayer(int layer) {
			selectedLayer = layer;
		};
		LayerDeletingPanel* m_LayerDeletingPanel;
		void SetLayerDeletingPanel(LayerDeletingPanel* layerDeletingPanel) {
			m_LayerDeletingPanel = layerDeletingPanel;
		};
		static void CreateLayer();
		static void DeleteLayer(int index);
		static void DeleteLayer(int index, int moveTo);
		static void MoveEntityToLayer(UUID entityUUID, int layerIndex);
		static void MoveEntitiesToAnotherLayer(int originalLayer, int newLayer = 0);
		static void DestroyAllEntitiesInLayer(int layerIndex);
		static void RepairEntityLayerIndexes(int layer);
	private:
		static int selectedLayer;
		friend class Renderer2D;
		static Ref<Scene> m_Context;
	};

}