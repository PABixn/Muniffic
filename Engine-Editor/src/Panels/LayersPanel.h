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
			std::vector<UUID> entitiesUUID;
			LayerInfo() {
				this->index = LayersPanel::Layers.size();
				this->name = "New Layer " + std::to_string(this->index);
				this->isVisible = TRUE;
			};
			void AddEntity(UUID uuid) {
				this->entitiesUUID.push_back(uuid);
			};
			void RemoveEntity(UUID uuid) {
				for (int i = 0; i < this->entitiesUUID.size(); i++) {
					if (entitiesUUID[i] == uuid) {
						this->entitiesUUID.erase(entitiesUUID.begin() + i);
					}
				}
			};
			void DeleteAllEntities() {
				for (int i = 0; i < entitiesUUID.size(); i++) {
					if (m_Context->GetEntityByUUID(entitiesUUID[i]).GetAnyChildren().size() > 0) {
						for (auto& child : m_Context->GetEntityByUUID(entitiesUUID[i]).GetAnyChildren()) {
							LayersPanel::Layers[this->index]->RemoveEntity(child.GetUUID());
						};
						m_Context->GetEntityByUUID(entitiesUUID[i]).RemoveAnyChildren();
					};
					m_Context->DestroyEntity(m_Context->GetEntityByUUID(entitiesUUID[i]));
				}
			};
			
		};
		static std::vector<LayerInfo*> Layers;
		void OnImGuiRender();
		static void AddLayer();
		void DeleteLayer(int selectedLayer);
		void RepairIndexes();
		//static int EntityLayerIndex(UUID uuid);
		std::string GetSelectedLayerName();
		void RenameLayer(std::string name);
		void SetContext(const Ref<Scene>& scene);
		void AssignLayerForExcistingEntities();
		static int GetSelectedLayer() {
			return selectedLayer;
		};
	private:
		static int selectedLayer;
		static Ref<Scene> m_Context;
	};

}