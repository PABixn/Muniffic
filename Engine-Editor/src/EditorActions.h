#pragma once
#include <Engine.h>
#include "Commands/Commands.h"

namespace eg
{
	class EditorActions
	{
	public:
		static std::string ExecuteEntityAction(const std::string& actionName, std::vector<std::string> params);
		static std::string ExecuteComponentAction(const std::string& actionName, std::vector<std::string> params);

		static void SetScene(const Ref<Scene>& scene) { m_Scene = scene; }
		static void SetSelectionContext(Entity* entity) { m_SelectedEntity = entity; }

	private:
		static Ref<Scene> m_Scene;
		static Entity* m_SelectedEntity;
	};
}