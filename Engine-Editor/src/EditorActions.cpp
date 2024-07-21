#include "EditorActions.h"

namespace eg
{
	Ref<Scene> EditorActions::m_Scene = nullptr;
	Entity* EditorActions::m_SelectedEntity = nullptr;

	std::string EditorActions::ExecuteEntityAction(const std::string& actionName, std::vector<std::string> params)
	{
		if (actionName == "GetUUID")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);
			return std::to_string(entity.GetUUID());
		}
		else if (actionName == "GetAllEntities")
		{
			auto view = m_Scene->GetEntitiesWith<TransformComponent>();
			std::string entities = "";
			for (auto entity : view)
			{
				entities += (Entity(entity, m_Scene.get()).GetName()) + ",";
			}
			return entities;
		}
		else if (actionName == "ChangeName")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			std::string name = tag;
			name = params[1];

			Commands::ExecuteRawValueCommand(&tag, name, "Assistant-TagComponent-Tag");

			return "Success";
		}
		else if (actionName == "CreateEntity")
		{
			Commands::ExecuteCommand<Commands::CreateEntityCommand>(Commands::CommandArgs(params[0], {}, m_Scene, *m_SelectedEntity));

			return "Success";
		}
		else if (actionName == "DeleteEntity")
		{
			Commands::ExecuteCommand<Commands::DeleteEntityCommand>(Commands::CommandArgs(params[0], {}, m_Scene, *m_SelectedEntity));

			return "Success";
		}
	}
}