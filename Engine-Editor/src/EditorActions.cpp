#include "EditorActions.h"
#include "EditorActionsHelper.h"

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
		else if (actionName == "GetAnyChildren")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);
			auto children = entity.GetAnyChildren();
			std::string childrenNames = "";
			for (Entity e : children)
			{
				childrenNames += e.GetName() + ",";
			}
			return childrenNames;
		}
		else if (actionName == "GetChildren")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);
			auto children = entity.GetChildren();
			std::string childrenNames = "";
			for (Entity e : children)
			{
				childrenNames += e.GetName() + ",";
			}
			return childrenNames;
		}
		else if (actionName == "GetParent")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);
			std::optional<Entity> parent = entity.GetParent();

			if (parent.has_value())
				return parent.value().GetName();
			else
				return "NULL";
		}
		else if (actionName == "SetParent")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);
			Entity parent = m_Scene->FindEntityByName(params[1]);
			
			Commands::ExecuteChangeParentCommand(entity, parent, m_Scene);
				
			return "Success";
		}
		else if (actionName == "IsChild")
		{
			Entity child = m_Scene->FindEntityByName(params[0]);
			Entity entity = m_Scene->FindEntityByName(params[1]);
			if(entity.IsChild(child))
				return "true";
			else
				return "false";
		}
		else if(actionName == "IsChildOfAny")
		{
			Entity child = m_Scene->FindEntityByName(params[0]);
			Entity entity = m_Scene->FindEntityByName(params[1]);
			if(entity.IsChildOfAny(child))
				return "true";
			else
				return "false";
		}
		else if (actionName == "AddComponent")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if(ComponentHelper::AddComponent(params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if(actionName == "RemoveComponent")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if(ComponentHelper::RemoveComponent(params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "HasComponent")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if(ComponentHelper::HasComponent(params[1], entity, m_Scene))
				return "true";
			else
				return "false";
		}
		else if (actionName == "InheritComponentInChildren")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if(ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::InheritComponentInChildren, params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "InheritComponentFromParent")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if(ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::InheritComponentFromParent, params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "CopyComponentToChildren")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if(ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::CopyComponentToChildren, params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "CopyComponentValuesToChildren")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if(ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::CopyComponentValuesToChildren, params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "CopyComponentWithValuesToChildren")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::CopyComponentWithValuesToChildren, params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "IsInheritedFromParent")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::IsInheritedFromParent, params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "IsInheritedInChildren")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::IsInheritedInChildren, params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
	}
}