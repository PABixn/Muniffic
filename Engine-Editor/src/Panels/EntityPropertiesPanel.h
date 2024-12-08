#pragma once
#include "Engine.h"
#include "../IconLoader.h"
#include "Commands/Commands.h"

namespace eg {
	class EntityPropertiesPanel {
	public:
		void OnImGuiRender();
		void SetEntity(Entity entity) { m_Entity = entity; }
		void SetScene(Ref<Scene> scene) { m_Context = scene; }
	private:
		void DrawComponents(Entity entity);
		template<typename T, typename UIFunction>
		void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction, Ref<Scene>& context, Icons icon);
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);
		UUID DrawScriptListPopup();
	private:
		bool m_OpenFunctionPopupCall = false;
		Entity m_Entity;
		Ref<Scene> m_Context = nullptr;
		std::vector<Ref<Animation>> m_PreviewedAnimations;
	};

}