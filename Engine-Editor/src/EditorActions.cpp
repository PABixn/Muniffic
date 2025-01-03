#include "EditorActions.h"
#include "EditorActionsHelper.h"
#include "EditorLayer.h"

namespace eg
{
	Ref<Scene> EditorActions::m_Scene = nullptr;
	Entity* EditorActions::m_SelectedEntity = nullptr;

	std::string EditorActions::ExecuteEntityAction(const std::string& actionName, const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();

		if (actionName == "GetUUID")
		{
			return GetUUID(params);
		}
		else if (actionName == "Exists")
		{
			return Exists(params);
		}
		else if (actionName == "GetAllEntities")
		{
			return GetAllEntities(params);
		}
		else if (actionName == "ChangeName")
		{
			return ChangeName(params);
		}
		else if (actionName == "CreateEntity")
		{
			return CreateEntity(params);
		}
		else if (actionName == "DeleteEntity")
		{
			return DeleteEntity(params);
		}
		else if (actionName == "GetAnyChildren")
		{
			return GetAnyChildren(params);
		}
		else if (actionName == "GetChildren")
		{
			return GetChildren(params);
		}
		else if (actionName == "GetParent")
		{
			return GetParent(params);
		}
		else if (actionName == "SetParent")
		{
			return SetParent(params);
		}
		else if (actionName == "IsChild")
		{
			return IsChild(params);
		}
		else if (actionName == "IsChildOfAny")
		{
			return IsChildOfAny(params);
		}
		else if (actionName == "AddComponent")
		{
			return AddComponent(params);
		}
		else if (actionName == "RemoveComponent")
		{
			return RemoveComponent(params);
		}
		else if (actionName == "HasComponent")
		{
			return HasComponent(params);
		}
		else if (actionName == "InheritComponentInChildren")
		{
			return InheritComponentInChildren(params);
		}
		else if (actionName == "InheritComponentFromParent")
		{
			return InheritComponentFromParent(params);
		}
		else if (actionName == "CopyComponentToChildren")
		{
			return CopyComponentToChildren(params);
		}
		else if (actionName == "CopyComponentValuesToChildren")
		{
			return CopyComponentValuesToChildren(params);
		}
		else if (actionName == "CopyComponentWithValuesToChildren")
		{
			return CopyComponentWithValuesToChildren(params);
		}
		else if (actionName == "IsInheritedFromParent")
		{
			return IsInheritedFromParent(params);
		}
		else if (actionName == "IsInheritedInChildren")
		{
			return IsInheritedInChildren(params);
		}
		else
		{
			return ExecuteComponentAction(actionName, params);
		}
	}

	std::string EditorActions::ExecuteComponentAction(const std::string& actionName, const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();

		if (actionName == "GetTransformComponent")
		{
			return GetTransformComponent(params);
		}
		else if (actionName == "SetTransformComponent")
		{
			return SetTransformComponent(params);
		}
		else if (actionName == "GetCameraComponent")
		{
			return GetCameraComponent(params);
		}
		else if (actionName == "SetCameraComponent")
		{
			return SetCameraComponent(params);
		}
		else if (actionName == "GetSpriteRendererComponent")
		{
			return GetSpriteRendererComponent(params);
		}
		else if (actionName == "SetSpriteRendererComponent")
		{
			return SetSpriteRendererComponent(params);
		}
		else if (actionName == "GetCircleRendererComponent")
		{
			return GetCircleRendererComponent(params);
		}
		else if (actionName == "SetCircleRendererComponent")
		{
			return SetCircleRendererComponent(params);
		}
		else if (actionName == "GetRigidBody2DComponent")
		{
			return GetRigidBody2DComponent(params);
		}
		else if (actionName == "SetRigidBody2DComponent")
		{
			return SetRigidBody2DComponent(params);
		}
		else if (actionName == "GetBoxCollider2DComponent")
		{
			return GetBoxCollider2DComponent(params);
		}
		else if (actionName == "SetBoxCollider2DComponent")
		{
			return SetBoxCollider2DComponent(params);
		}
		else if (actionName == "GetCircleCollider2DComponent")
		{
			return GetCircleCollider2DComponent(params);
		}
		else if (actionName == "SetCircleCollider2DComponent")
		{
			return SetCircleCollider2DComponent(params);
		}
		else if (actionName == "GetTextComponent")
		{
			return GetTextComponent(params);
		}
		else if (actionName == "SetTextComponent")
		{
			return SetTextComponent(params);
		}
		else
		{
			return "Invalid action name.";
		}
	}



	#pragma region EntityActions

	std::string EditorActions::GetUUID(const std::vector<std::string>& params)
	{
        EG_PROFILE_FUNCTION();

		if (params.size() < 1)
			return "No entity name provided.";

		Entity entity = m_Scene->FindEntityByName(params.at(0));

		if (entity == Entity())
			return "Entity with name " + params.at(0) + " not found";

		return std::to_string(entity.GetUUID());
	}

	std::string EditorActions::Exists(const std::vector<std::string>& params)
	{
        EG_PROFILE_FUNCTION();

		if (params.size() < 1)
			return "No entity name provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "false";
		else
			return "true";
	}

	std::string EditorActions::GetAllEntities(const std::vector<std::string>& params)
	{
        EG_PROFILE_FUNCTION();

		auto view = m_Scene->GetEntitiesWith<TransformComponent>();
		std::string entities = "";
		for (auto entity : view)
		{
			entities += Entity(entity, m_Scene.get()).GetName() + " (UUID:" +
				std::to_string(Entity(entity, m_Scene.get()).GetUUID()) + "), ";
		}
		return entities;
	}

	std::string EditorActions::ChangeName(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();

		if (params.size() < 1)
			return "No entity name provided.";
		else if (params.size() < 2)
			return "No new name provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!entity.HasComponent<TagComponent>())
			return "Entity does not have TagComponent.";

		std::string& tag = entity.GetComponent<TagComponent>().Tag;
		std::string name = tag;
		tag = params.at(1);

		Commands::ExecuteRawValueCommand(&tag, name, "Assistant-TagComponent-Tag");

		return "Success";
	}

	std::string EditorActions::CreateEntity(const std::vector<std::string>& params)
	{
        EG_PROFILE_FUNCTION();

		if (params.size() < 1)
			return "No entity name provided.";

		Commands::ExecuteCommand<Commands::CreateEntityCommand>(Commands::CommandArgs(params.at(0), {}, m_Scene, *m_SelectedEntity));

		UUID uuid = m_Scene->FindEntityByName(params.at(0)).GetUUID();

		if (uuid == 0)
			return "Failed to create entity.";

		static_cast<EditorLayer*>(Application::Get().GetFirstLayer())->GetSceneHierarchyPanel()->Search();

		return "Entity created with UUID: " + std::to_string(uuid);
	}

	std::string EditorActions::DeleteEntity(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		Commands::ExecuteCommand<Commands::DeleteEntityCommand>(Commands::CommandArgs("", entity, m_Scene, *m_SelectedEntity));

		static_cast<EditorLayer*>(Application::Get().GetFirstLayer())->GetSceneHierarchyPanel()->Search();

		return "Entity removed";
	}

	std::string EditorActions::GetAnyChildren(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		std::string childrenNames = "";
		Ref<std::vector<Entity>> children = entity.GetAnyChildren();
		for (Entity e : *children)
		{
			childrenNames += e.GetName() + ",";
		}
		return childrenNames;
	}

	std::string EditorActions::GetChildren(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		std::string childrenNames = "";
		Ref<std::vector<Entity>> children = entity.GetChildren();
		for (Entity e : *children)
		{
			childrenNames += e.GetName() + ",";
		}
		return childrenNames;
	}

	std::string EditorActions::GetParent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		std::optional<Entity> parent = entity.GetParent();

		if (parent.has_value())
			return parent.value().GetName();
		else
			return "NULL";
	}

	std::string EditorActions::SetParent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No parent UUID provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		std::optional<Entity> parent;

		if (params.at(1) == "" || params.at(1) == "NULL" || params.at(1) == "null" || params.at(1) == "0")
			parent = std::nullopt;
		else
		{
			if (!ComponentHelper::CanConvertToUUID(params.at(1)))
				return "Second parameter is not UUID.";

			parent = m_Scene->GetEntityByUUID(std::stoll(params.at(1)));

			if (parent == Entity())
				return "Entity with UUID " + params.at(1) + " not found";
		}

		Commands::ExecuteChangeParentCommand(entity, parent, m_Scene);

		return "Success";
	}

	std::string EditorActions::IsChild(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No child entity UUID provided.";
		else if (params.size() < 2)
			return "No parent entity UUID provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity child = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (child == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!ComponentHelper::CanConvertToUUID(params.at(1)))
			return "Second parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(1)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(1) + " not found";

		if (entity.IsChild(child))
			return "true";
		else
			return "false";
	}

	std::string EditorActions::IsChildOfAny(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No child entity UUID provided.";
		else if (params.size() < 2)
			return "No parent entity UUID provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity child = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (child == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!ComponentHelper::CanConvertToUUID(params.at(1)))
			return "Second parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(1)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(1) + " not found";

		if (entity.IsChildOfAny(child))
			return "true";
		else
			return "false";
	}

	std::string EditorActions::AddComponent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No component name provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (ComponentHelper::AddComponent(params.at(1), entity, m_Scene))
			return "Success";
		else
			return "Failed";
	}

	std::string EditorActions::RemoveComponent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No component name provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (ComponentHelper::RemoveComponent(params.at(1), entity, m_Scene))
			return "Success";
		else
			return "Failed";
	}

	std::string EditorActions::HasComponent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No component name provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (ComponentHelper::HasComponent(params.at(1), entity, m_Scene))
			return "true";
		else
			return "false";
	}

	std::string EditorActions::InheritComponentInChildren(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No component name provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::InheritComponentInChildren, params.at(1), entity, m_Scene))
			return "Success";
		else
			return "Failed";
	}

	std::string EditorActions::InheritComponentFromParent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No component name provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::InheritComponentFromParent, params.at(1), entity, m_Scene))
			return "Success";
		else
			return "Failed";
	}

	std::string EditorActions::CopyComponentToChildren(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No component name provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::CopyComponentToChildren, params.at(1), entity, m_Scene))
			return "Success";
		else
			return "Failed";
	}

	std::string EditorActions::CopyComponentValuesToChildren(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No component name provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::CopyComponentValuesToChildren, params.at(1), entity, m_Scene))
			return "Success";
		else
			return "Failed";
	}

	std::string EditorActions::CopyComponentWithValuesToChildren(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No component name provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::CopyComponentWithValuesToChildren, params.at(1), entity, m_Scene))
			return "Success";
		else
			return "Failed";
	}

	std::string EditorActions::IsInheritedFromParent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No component name provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::IsInheritedFromParent, params.at(1), entity, m_Scene))
			return "Success";
		else
			return "Failed";
	}

	std::string EditorActions::IsInheritedInChildren(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No component name provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::IsInheritedInChildren, params.at(1), entity, m_Scene))
			return "Success";
		else
			return "Failed";
	}

#pragma endregion


	#pragma region ComponentActions

	std::string EditorActions::GetTransformComponent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!entity.HasComponent<TransformComponent>())
			return "Entity does not have TransformComponent.";

		TransformComponent& transform = entity.GetComponent<TransformComponent>();

		std::string transformString = "Position: X: "
			+ std::to_string(transform.Translation.x) + " Y: "
			+ std::to_string(transform.Translation.y) + " Z: "
			+ std::to_string(transform.Translation.z) + " Rotation: X: "
			+ std::to_string(transform.Rotation.x) + " Y: "
			+ std::to_string(transform.Rotation.y) + " Z: "
			+ std::to_string(transform.Rotation.z) + " Scale: X: "
			+ std::to_string(transform.Scale.x) + " Y: "
			+ std::to_string(transform.Scale.y) + " Z: "
			+ std::to_string(transform.Scale.z);

		return transformString;
	}

	std::string EditorActions::SetTransformComponent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No property name provided.";
		else if (params.size() < 5)
			return "No values provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!entity.HasComponent<TransformComponent>())
			return "Entity does not have TransformComponent.";

		TransformComponent& transform = entity.GetComponent<TransformComponent>();

		if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
			return "Parameter is not float.";

		if (ComponentHelper::CanConvertToFloat(params.at(3)) == false)
			return "Parameter is not float.";

		if (ComponentHelper::CanConvertToFloat(params.at(4)) == false)
			return "Parameter is not float.";

		if (params.at(1) == "Position")
		{
			glm::vec3 position = transform.Translation;
			glm::vec3 newPosition = glm::vec3(std::stof(params.at(2)), std::stof(params.at(3)), std::stof(params.at(4)));
			transform.Translation = newPosition;
			Commands::ExecuteRawValueCommand<glm::vec3, TransformComponent>(&transform.Translation, position, entity, "Assistant-TransformComponent-Position");

			return "Success";
		}
		else if (params.at(1) == "Rotation")
		{
			glm::vec3 rotation = transform.Rotation;
			glm::vec3 newRotation = glm::vec3(std::stof(params.at(2)), std::stof(params.at(3)), std::stof(params.at(4)));
			transform.Rotation = newRotation;
			Commands::ExecuteRawValueCommand<glm::vec3, TransformComponent>(&transform.Rotation, rotation, entity, "Assistant-TransformComponent-Rotation");

			return "Success";
		}
		else if (params.at(1) == "Scale")
		{
			glm::vec3 scale = transform.Scale;
			glm::vec3 newScale = glm::vec3(std::stof(params.at(2)), std::stof(params.at(3)), std::stof(params.at(4)));
			transform.Scale = newScale;
			Commands::ExecuteRawValueCommand<glm::vec3, TransformComponent>(&transform.Scale, scale, entity, "Assistant-TransformComponent-Scale");

			return "Success";
		}
		else
			return "Invalid property.";

		return "None action was taken.";
	}

	std::string EditorActions::GetCameraComponent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!entity.HasComponent<CameraComponent>())
			return "Entity does not have CameraComponent.";

		CameraComponent& camera = entity.GetComponent<CameraComponent>();

		std::string cameraString = "Primary: " + std::to_string(camera.Primary) + " FixedAspectRatio: " + std::to_string(camera.FixedAspectRatio) + " AspectRatio: " + std::to_string(camera.FixedAspectRatio);

		if (camera.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
		{
			cameraString += " ProjectionType: Orthographic Size: " + std::to_string(camera.Camera.GetOrthographicSize()) + " NearClip: " + std::to_string(camera.Camera.GetOrthographicNearClip()) + " FarClip: " + std::to_string(camera.Camera.GetOrthographicFarClip());
		}
		else
		{
			cameraString += " ProjectionType: Perspective VerticalFOV: " + std::to_string(camera.Camera.GetPerspectiveVerticalFOV()) + " NearClip: " + std::to_string(camera.Camera.GetPerspectiveNearClip()) + " FarClip: " + std::to_string(camera.Camera.GetPerspectiveFarClip());
		}

		return cameraString;
	}

	std::string EditorActions::SetCameraComponent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No property name provided.";
		else if (params.size() < 3)
			return "No value provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!entity.HasComponent<CameraComponent>())
			return "Entity does not have CameraComponent.";

		CameraComponent& camera = entity.GetComponent<CameraComponent>();
		SceneCamera& sceneCamera = camera.Camera;

		if (params.at(1) == "Primary")
		{
			if (params.at(2) == "true" || params.at(2) == "True")
			{
				bool primary = camera.Primary;
				camera.Primary = true;
				Commands::ExecuteRawValueCommand(&camera.Primary, primary, "Assistant-CameraComponent-Primary");

				return "Success";
			}
			else if (params.at(2) == "false" || params.at(2) == "False")
			{
				bool primary = camera.Primary;
				camera.Primary = false;
				Commands::ExecuteRawValueCommand(&camera.Primary, primary, "Assistant-CameraComponent-Primary");

				return "Success";
			}
			else
				return "Invalid value.";
		}
		else if (params.at(1) == "FixedAspectRatio")
		{
			if (params.at(2) == "true" || params.at(2) == "True")
			{
				bool fixedAspectRatio = camera.FixedAspectRatio;
				camera.FixedAspectRatio = true;
				Commands::ExecuteRawValueCommand(&camera.FixedAspectRatio, fixedAspectRatio, "Assistant-CameraComponent-FixedAspectRatio");

				return "Success";
			}
			else if (params.at(2) == "false" || params.at(2) == "False")
			{
				bool fixedAspectRatio = camera.FixedAspectRatio;
				camera.FixedAspectRatio = false;
				Commands::ExecuteRawValueCommand(&camera.FixedAspectRatio, fixedAspectRatio, "Assistant-CameraComponent-FixedAspectRatio");

				return "Success";
			}
			else
				return "Invalid value.";
		}
		else if (params.at(1) == "ProjectionType")
		{
			if (params.at(2) == "Orthographic")
			{
				Commands::ExecuteValueCommand<SceneCamera::ProjectionType>([&sceneCamera](SceneCamera::ProjectionType projectionType)
					{
						sceneCamera.SetProjectionType(projectionType);
					}, SceneCamera::ProjectionType::Orthographic, sceneCamera.GetProjectionType(), "Assistant-CameraComponent-ProjectionType");

				return "Success";
			}
			else if (params.at(2) == "Perspective")
			{
				Commands::ExecuteValueCommand<SceneCamera::ProjectionType>([&sceneCamera](SceneCamera::ProjectionType projectionType)
					{
						sceneCamera.SetProjectionType(projectionType);
					}, SceneCamera::ProjectionType::Perspective, sceneCamera.GetProjectionType(), "Assistant-CameraComponent-ProjectionType");

				return "Success";
			}
			else
				return "Invalid value.";
		}
		else if (params.at(1) == "OrthographicSize")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) != false)
			{
				Commands::ExecuteValueCommand<float>([&sceneCamera](float size)
					{
						sceneCamera.SetOrthographicSize(size);
					}, std::stof(params.at(2)), sceneCamera.GetOrthographicSize(), "Assistant-CameraComponent-OrthographicSize");

				return "Success";
			}
			else
				return "Parameter is not float.";
		}
		else if (params.at(1) == "OrthographicNearClip")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) != false)
			{
				Commands::ExecuteValueCommand<float>([&sceneCamera](float nearClip)
					{
						sceneCamera.SetOrthographicNearClip(nearClip);
					}, std::stof(params.at(2)), sceneCamera.GetOrthographicNearClip(), "Assistant-CameraComponent-OrthographicNearClip");

				return "Success";
			}
			else
				return "Parameter is not float.";
		}
		else if (params.at(1) == "OrthographicFarClip")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) != false)
			{
				Commands::ExecuteValueCommand<float>([&sceneCamera](float farClip)
					{
						sceneCamera.SetOrthographicFarClip(farClip);
					}, std::stof(params.at(2)), sceneCamera.GetOrthographicFarClip(), "Assistant-CameraComponent-OrthographicFarClip");

				return "Success";
			}
			else
				return "Parameter is not float.";
		}
		else if (params.at(1) == "PerspectiveVerticalFOV")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) != false)
			{
				Commands::ExecuteValueCommand<float>([&sceneCamera](float verticalFOV)
					{
						sceneCamera.SetPerspectiveVerticalFOV(verticalFOV);
					}, std::stof(params.at(2)), sceneCamera.GetPerspectiveVerticalFOV(), "Assistant-CameraComponent-PerspectiveVerticalFOV");

				return "Success";
			}
			else
				return "Parameter is not float.";
		}
		else if (params.at(1) == "PerspectiveNearClip")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) != false)
			{
				Commands::ExecuteValueCommand<float>([&sceneCamera](float nearClip)
					{
						sceneCamera.SetPerspectiveNearClip(nearClip);
					}, std::stof(params.at(2)), sceneCamera.GetPerspectiveNearClip(), "Assistant-CameraComponent-PerspectiveNearClip");

				return "Success";
			}
			else
				return "Parameter is not float.";
		}
		else if (params.at(1) == "PerspectiveFarClip")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) != false)
			{
				Commands::ExecuteValueCommand<float>([&sceneCamera](float farClip)
					{
						sceneCamera.SetPerspectiveFarClip(farClip);
					}, std::stof(params.at(2)), sceneCamera.GetPerspectiveFarClip(), "Assistant-CameraComponent-PerspectiveFarClip");

				return "Success";
			}
			else
				return "Parameter is not float.";
		}
		else
			return "Invalid property.";

		return "None action was taken.";
	}

	std::string EditorActions::GetSpriteRendererComponent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!entity.HasComponent<SpriteRendererComponent>())
			return "Entity does not have SpriteRendererComponent.";

		SpriteRendererComponent& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();

		std::string spriteRendererString = "Color: R: "
			+ std::to_string(spriteRenderer.Color.x * 255) + " G: "
			+ std::to_string(spriteRenderer.Color.y * 255) + " B: "
			+ std::to_string(spriteRenderer.Color.z * 255) + " A: "
			+ std::to_string(spriteRenderer.Color.w * 255) + " Texture: "
			+ spriteRenderer.Texture->GetPath() + "TextureUUID: "
			+ std::to_string(spriteRenderer.TextureUUID) + " TilingFactor: "
			+ std::to_string(spriteRenderer.TilingFactor);

		return spriteRendererString;
	}

	std::string EditorActions::SetSpriteRendererComponent(const std::vector<std::string>& params)
	{
        EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No property name provided.";
		else if (params.size() < 3)
			return "No value provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!entity.HasComponent<SpriteRendererComponent>())
			return "Entity does not have SpriteRendererComponent.";

		SpriteRendererComponent& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();

		if (params.at(1) == "TilingFactor")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "Parameter is not float.";

			float factor = spriteRenderer.TilingFactor;
			spriteRenderer.TilingFactor = std::stof(params.at(2));
			Commands::ExecuteRawValueCommand<float, SpriteRendererComponent>(&spriteRenderer.TilingFactor, factor, entity, "Assistant-SpriteRendererComponent-TilingFactor");

			return "Success";
		}
		else if (params.at(1) == "Color")
		{
			bool hasAlpha = true;

			if (params.size() < 4)
				return "No G value provided.";
			else if (params.size() < 5)
				return "No B value provided.";
			else if (params.size() < 6)
				hasAlpha = false;

			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "R value is not float.";
			if (ComponentHelper::CanConvertToFloat(params.at(3)) == false)
				return "G value is not float.";
			if (ComponentHelper::CanConvertToFloat(params.at(4)) == false)
				return "B value is not float.";

			if (hasAlpha)
			{
				if (ComponentHelper::CanConvertToFloat(params.at(5)) == false)
					return "A value is not float.";
			}

			glm::vec4 color = spriteRenderer.Color;
			spriteRenderer.Color.x = std::stof(params.at(2)) / 255;
			spriteRenderer.Color.y = std::stof(params.at(3)) / 255;
			spriteRenderer.Color.z = std::stof(params.at(4)) / 255;

			if (hasAlpha)
				spriteRenderer.Color.w = std::stof(params.at(5)) / 255;
			else
				spriteRenderer.Color.w = 1.0f;

			Commands::ExecuteRawValueCommand<glm::vec4, SpriteRendererComponent>(&spriteRenderer.Color, color, entity, "Assistant-SpriteRendererComponent-Color");

			return "Success";
		}
		else if (params.at(1) == "Texture")
		{
			if (ComponentHelper::CanConvertToInteger(params.at(2)) == false)
				return "Parameter is not integer.";

			Ref<Texture2D> texture = ResourceDatabase::GetTextureRuntimeResource(std::stoi(params.at(2)));

			if (texture->IsLoaded())
			{
				spriteRenderer.TextureUUID = std::stoi(params.at(2));
				Ref<Texture2D> oldTexture = spriteRenderer.Texture;
				spriteRenderer.Texture = texture;
				Commands::ExecuteRawValueCommand<Ref<Texture2D>, SpriteRendererComponent>(&spriteRenderer.Texture, oldTexture, entity, "Assistant-SpriteRendererComponent-Texture");

				return "Success";
			}
			else
				return "Texture could not be loaded.";
		}
		else
			return "Invalid property.";

		return "None action was taken.";
	}

	std::string EditorActions::GetCircleRendererComponent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!entity.HasComponent<CircleRendererComponent>())
			return "Entity does not have CircleRendererComponent.";

		CircleRendererComponent& circleRenderer = entity.GetComponent<CircleRendererComponent>();

		std::string circleRendererString = "Color: R: "
			+ std::to_string(circleRenderer.Color.x * 255) + " G: "
			+ std::to_string(circleRenderer.Color.y * 255) + " B: "
			+ std::to_string(circleRenderer.Color.z * 255) + " A: "
			+ std::to_string(circleRenderer.Color.w * 255) + " Fade: "
			+ std::to_string(circleRenderer.Fade) + " Thickness: "
			+ std::to_string(circleRenderer.Thickness);

		return circleRendererString;
	}

	std::string EditorActions::SetCircleRendererComponent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No property name provided.";
		else if (params.size() < 3)
			return "No value provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (entity.HasComponent<CircleRendererComponent>() == false)
			return "Entity does not have CircleRendererComponent.";

		if (entity.HasComponent<CircleRendererComponent>() == false)
			return "Entity does not have CircleRendererComponent.";

		CircleRendererComponent& circleRenderer = entity.GetComponent<CircleRendererComponent>();

		if (params.at(1) == "Thickness")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "Parameter is not float.";

			float thickness = circleRenderer.Thickness;
			circleRenderer.Thickness = std::stof(params.at(2));
			Commands::ExecuteRawValueCommand<float, CircleRendererComponent>(&circleRenderer.Thickness, thickness, entity, "Assistant-CircleRendererComponent-Thickness");

			return "Success";
		}
		else if (params.at(1) == "Fade")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "Parameter is not float.";

			float fade = circleRenderer.Fade;
			circleRenderer.Fade = std::stof(params.at(2));
			Commands::ExecuteRawValueCommand<float, CircleRendererComponent>(&circleRenderer.Fade, fade, entity, "Assistant-CircleRendererComponent-Fade");

			return "Success";
		}
		else if (params.at(1) == "Color")
		{
			bool hasAlpha = true;

			if (params.size() < 4)
				return "No G value provided.";
			else if (params.size() < 5)
				return "No B value provided.";
			else if (params.size() < 6)
				hasAlpha = false;

			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "R value is not float.";
			if (ComponentHelper::CanConvertToFloat(params.at(3)) == false)
				return "G value is not float.";
			if (ComponentHelper::CanConvertToFloat(params.at(4)) == false)
				return "B value is not float.";

			if (hasAlpha)
			{
				if (ComponentHelper::CanConvertToFloat(params.at(5)) == false)
					return "A value is not float.";
			}

			glm::vec4 color = circleRenderer.Color;
			circleRenderer.Color.x = std::stof(params.at(2)) / 255;
			circleRenderer.Color.y = std::stof(params.at(3)) / 255;
			circleRenderer.Color.z = std::stof(params.at(4)) / 255;

			if(hasAlpha)
				circleRenderer.Color.w = std::stof(params.at(5)) / 255;
			else
				circleRenderer.Color.w = 1.0f;

			Commands::ExecuteRawValueCommand<glm::vec4, CircleRendererComponent>(&circleRenderer.Color, color, entity, "Assistant-CircleRendererComponent-Color");

			return "Success";
		}
		else
			return "Invalid property.";

		return "None action was taken.";
	}

	std::string EditorActions::GetRigidBody2DComponent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!entity.HasComponent<RigidBody2DComponent>())
			return "Entity does not have RigidBody2DComponent.";

		RigidBody2DComponent& rigidBody2D = entity.GetComponent<RigidBody2DComponent>();

		std::string rigidBody2DString = "Type: ";
		rigidBody2DString += rigidBody2D.Type == RigidBody2DComponent::BodyType::Static ? "Static" : rigidBody2D.Type == RigidBody2DComponent::BodyType::Dynamic ? "Dynamic" : "Kinematic";
		rigidBody2DString += " FixedRotation: " + std::to_string(rigidBody2D.FixedRotation);

		return rigidBody2DString;
	}

	std::string EditorActions::SetRigidBody2DComponent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No property name provided.";
		else if (params.size() < 3)
			return "No value provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!entity.HasComponent<RigidBody2DComponent>())
			return "Entity does not have RigidBody2DComponent.";

		RigidBody2DComponent& rigidBody2D = entity.GetComponent<RigidBody2DComponent>();

		if (params.at(1) == "FixedRotation")
		{
			if (params.at(2) == "true" || params.at(2) == "True")
			{
				bool fixedRotation = rigidBody2D.FixedRotation;
				rigidBody2D.FixedRotation = true;
				Commands::ExecuteRawValueCommand(&rigidBody2D.FixedRotation, fixedRotation, "Assistant-RigidBody2DComponent-FixedRotation");

				return "Success";
			}
			else if (params.at(2) == "false" || params.at(2) == "False")
			{
				bool fixedRotation = rigidBody2D.FixedRotation;
				rigidBody2D.FixedRotation = false;
				Commands::ExecuteRawValueCommand(&rigidBody2D.FixedRotation, fixedRotation, "Assistant-RigidBody2DComponent-FixedRotation");

				return "Success";
			}
			else
				return "Invalid value.";
		}
		else if (params.at(1) == "Type")
		{
			if (params.at(2) == "Static")
			{
				RigidBody2DComponent::BodyType type = rigidBody2D.Type;
				rigidBody2D.Type = RigidBody2DComponent::BodyType::Static;
				Commands::ExecuteRawValueCommand<RigidBody2DComponent::BodyType, RigidBody2DComponent>(&rigidBody2D.Type, type, entity, "Assistant-RigidBody2DComponent-Type");

				return "Success";
			}
			else if (params.at(2) == "Dynamic")
			{
				RigidBody2DComponent::BodyType type = rigidBody2D.Type;
				rigidBody2D.Type = RigidBody2DComponent::BodyType::Dynamic;
				Commands::ExecuteRawValueCommand<RigidBody2DComponent::BodyType, RigidBody2DComponent>(&rigidBody2D.Type, type, entity, "Assistant-RigidBody2DComponent-Type");

				return "Success";
			}
			else if (params.at(2) == "Kinematic")
			{
				RigidBody2DComponent::BodyType type = rigidBody2D.Type;
				rigidBody2D.Type = RigidBody2DComponent::BodyType::Kinematic;
				Commands::ExecuteRawValueCommand<RigidBody2DComponent::BodyType, RigidBody2DComponent>(&rigidBody2D.Type, type, entity, "Assistant-RigidBody2DComponent-Type");

				return "Success";
			}
			else
				return "Invalid value.";
		}
		else
			return "Invalid property.";

		return "None action was taken.";
	}

	std::string EditorActions::GetBoxCollider2DComponent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!entity.HasComponent<BoxCollider2DComponent>())
			return "Entity does not have BoxCollider2DComponent.";

		BoxCollider2DComponent& boxCollider2D = entity.GetComponent<BoxCollider2DComponent>();

		std::string boxCollider2DString = "Offset: X: "
			+ std::to_string(boxCollider2D.Offset.x) + " Y: "
			+ std::to_string(boxCollider2D.Offset.y) + " Size: X: "
			+ std::to_string(boxCollider2D.Size.x) + " Y: "
			+ std::to_string(boxCollider2D.Size.y) + " Density: "
			+ std::to_string(boxCollider2D.Density) + " Friction: "
			+ std::to_string(boxCollider2D.Friction) + " Restitution: "
			+ std::to_string(boxCollider2D.Restitution) + " RestitutionThreshold: "
			+ std::to_string(boxCollider2D.RestitutionThreshold);

		return boxCollider2DString;
	}

	std::string EditorActions::SetBoxCollider2DComponent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No property name provided.";
		else if (params.size() < 3)
			return "No value provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!entity.HasComponent<BoxCollider2DComponent>())
			return "Entity does not have BoxCollider2DComponent.";

		BoxCollider2DComponent& boxCollider2D = entity.GetComponent<BoxCollider2DComponent>();

		if (params.at(1) == "Offset")
		{
			if (params.size() < 4)
				return "No Y value provided.";

			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "Parameter is not float.";
			if (ComponentHelper::CanConvertToFloat(params.at(3)) == false)
				return "Parameter is not float.";

			glm::vec2 offset = boxCollider2D.Offset;
			glm::vec2 newOffset = glm::vec2(std::stof(params.at(2)), std::stof(params.at(3)));
			boxCollider2D.Offset = newOffset;
			Commands::ExecuteRawValueCommand<glm::vec2, BoxCollider2DComponent>(&boxCollider2D.Offset, offset, entity, "Assistant-BoxCollider2DComponent-Offset");

			return "Success";
		}
		else if (params.at(1) == "Size")
		{
			if (params.size() < 4)
				return "No Y value provided.";

			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "Parameter is not float.";
			if (ComponentHelper::CanConvertToFloat(params.at(3)) == false)
				return "Parameter is not float.";

			glm::vec2 size = boxCollider2D.Size;
			glm::vec2 newSize = glm::vec2(std::stof(params.at(2)), std::stof(params.at(3)));
			boxCollider2D.Size = newSize;
			Commands::ExecuteRawValueCommand<glm::vec2, BoxCollider2DComponent>(&boxCollider2D.Size, size, entity, "Assistant-BoxCollider2DComponent-Size");

			return "Success";
		}
		else if (params.at(1) == "Density")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "Parameter is not float.";

			float density = boxCollider2D.Density;
			boxCollider2D.Density = std::stof(params.at(2));
			Commands::ExecuteRawValueCommand<float, BoxCollider2DComponent>(&boxCollider2D.Density, density, entity, "Assistant-BoxCollider2DComponent-Density");

			return "Success";
		}
		else if (params.at(1) == "Friction")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "Parameter is not float.";

			float friction = boxCollider2D.Friction;
			boxCollider2D.Friction = std::stof(params.at(2));
			Commands::ExecuteRawValueCommand<float, BoxCollider2DComponent>(&boxCollider2D.Friction, friction, entity, "Assistant-BoxCollider2DComponent-Friction");

			return "Success";
		}
		else if (params.at(1) == "Restitution")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "Parameter is not float.";

			float restitution = boxCollider2D.Restitution;
			boxCollider2D.Restitution = std::stof(params.at(2));
			Commands::ExecuteRawValueCommand<float, BoxCollider2DComponent>(&boxCollider2D.Restitution, restitution, entity, "Assistant-BoxCollider2DComponent-Restitution");

			return "Success";
		}
		else if (params.at(1) == "RestitutionThreshold")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "Parameter is not float.";

			float restitutionThreshold = boxCollider2D.RestitutionThreshold;
			boxCollider2D.RestitutionThreshold = std::stof(params.at(2));
			Commands::ExecuteRawValueCommand<float, BoxCollider2DComponent>(&boxCollider2D.RestitutionThreshold, restitutionThreshold, entity, "Assistant-BoxCollider2DComponent-RestitutionThreshold");

			return "Success";
		}
		else
			return "Invalid property.";

		return "None action was taken.";
	}

	std::string EditorActions::GetCircleCollider2DComponent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!entity.HasComponent<CircleCollider2DComponent>())
			return "Entity does not have CircleCollider2DComponent.";

		CircleCollider2DComponent& circleCollider2D = entity.GetComponent<CircleCollider2DComponent>();

		std::string circleCollider2DString = "Offset: X: "
			+ std::to_string(circleCollider2D.Offset.x) + " Y: "
			+ std::to_string(circleCollider2D.Offset.y) + " Radius: "
			+ std::to_string(circleCollider2D.Radius) + " Density: "
			+ std::to_string(circleCollider2D.Density) + " Friction: "
			+ std::to_string(circleCollider2D.Friction) + " Restitution: "
			+ std::to_string(circleCollider2D.Restitution) + " RestitutionThreshold: "
			+ std::to_string(circleCollider2D.RestitutionThreshold);

		return circleCollider2DString;
	}

	std::string EditorActions::SetCircleCollider2DComponent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No property name provided.";
		else if (params.size() < 3)
			return "No value provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!entity.HasComponent<CircleCollider2DComponent>())
			return "Entity does not have CircleCollider2DComponent.";

		CircleCollider2DComponent& circleCollider2D = entity.GetComponent<CircleCollider2DComponent>();

		if (params.at(1) == "Offset")
		{
			if (params.size() < 4)
				return "No Y value provided.";

			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "Parameter is not float.";
			if (ComponentHelper::CanConvertToFloat(params.at(3)) == false)
				return "Parameter is not float.";

			glm::vec2 offset = circleCollider2D.Offset;
			glm::vec2 newOffset = glm::vec2(std::stof(params.at(2)), std::stof(params.at(3)));
			circleCollider2D.Offset = newOffset;
			Commands::ExecuteRawValueCommand<glm::vec2, CircleCollider2DComponent>(&circleCollider2D.Offset, offset, entity, "Assistant-CircleCollider2DComponent-Offset");

			return "Success";
		}
		else if (params.at(1) == "Radius")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "Parameter is not float.";

			float radius = circleCollider2D.Radius;
			circleCollider2D.Radius = std::stof(params.at(2));
			Commands::ExecuteRawValueCommand<float, CircleCollider2DComponent>(&circleCollider2D.Radius, radius, entity, "Assistant-CircleCollider2DComponent-Radius");

			return "Success";
		}
		else if (params.at(1) == "Density")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "Parameter is not float.";

			float density = circleCollider2D.Density;
			circleCollider2D.Density = std::stof(params.at(2));
			Commands::ExecuteRawValueCommand<float, CircleCollider2DComponent>(&circleCollider2D.Density, density, entity, "Assistant-CircleCollider2DComponent-Density");

			return "Success";
		}
		else if (params.at(1) == "Friction")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "Parameter is not float.";

			float friction = circleCollider2D.Friction;
			circleCollider2D.Friction = std::stof(params.at(2));
			Commands::ExecuteRawValueCommand<float, CircleCollider2DComponent>(&circleCollider2D.Friction, friction, entity, "Assistant-CircleCollider2DComponent-Friction");

			return "Success";
		}
		else if (params.at(1) == "Restitution")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "Parameter is not float.";

			float restitution = circleCollider2D.Restitution;
			circleCollider2D.Restitution = std::stof(params.at(2));
			Commands::ExecuteRawValueCommand<float, CircleCollider2DComponent>(&circleCollider2D.Restitution, restitution, entity, "Assistant-CircleCollider2DComponent-Restitution");

			return "Success";
		}
		else if (params.at(1) == "RestitutionThreshold")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "Parameter is not float.";

			float restitutionThreshold = circleCollider2D.RestitutionThreshold;
			circleCollider2D.RestitutionThreshold = std::stof(params.at(2));
			Commands::ExecuteRawValueCommand<float, CircleCollider2DComponent>(&circleCollider2D.RestitutionThreshold, restitutionThreshold, entity, "Assistant-CircleCollider2DComponent-RestitutionThreshold");

			return "Success";
		}
		else
			return "Invalid property.";
	}

	std::string EditorActions::GetTextComponent(const std::vector<std::string>& params)
	{
        EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!entity.HasComponent<TextComponent>())
			return "Entity does not have TextComponent.";

		TextComponent& text = entity.GetComponent<TextComponent>();

		std::string textString = "Text: " + text.TextString + " Font: " +
			ResourceDatabase::GetResourcePath(text.FontAsset).string() + " Color: R: " +
			std::to_string(text.Color.x * 255) + " G: " +
			std::to_string(text.Color.y * 255) + " B: " +
			std::to_string(text.Color.z * 255) + " A: " +
			std::to_string(text.Color.w * 255) + " Kerning: " +
			std::to_string(text.Kerning) + " LineSpacing: " +
			std::to_string(text.LineSpacing);

		return textString;
	}

	std::string EditorActions::SetTextComponent(const std::vector<std::string>& params)
	{
		EG_PROFILE_FUNCTION();
		if (params.size() < 1)
			return "No entity UUID provided.";
		else if (params.size() < 2)
			return "No property name provided.";
		else if (params.size() < 3)
			return "No value provided.";

		if (!ComponentHelper::CanConvertToUUID(params.at(0)))
			return "First parameter is not UUID.";

		Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

		if (entity == Entity())
			return "Entity with UUID " + params.at(0) + " not found";

		if (!entity.HasComponent<TextComponent>())
			return "Entity does not have TextComponent.";

		TextComponent& text = entity.GetComponent<TextComponent>();

		if (params.at(1) == "Text")
		{
			std::string oldText = text.TextString;
			text.TextString = params.at(2);
			Commands::ExecuteRawValueCommand<std::string, TextComponent>(&text.TextString, oldText, entity, "Assistant-TextComponent-Text");

			return "Success";
		}
		else if (params.at(1) == "Font")
		{
			if (ComponentHelper::CanConvertToInteger(params.at(2)) == false)
				return "Parameter is not integer.";

			UUID oldFont = text.FontAsset;
			text.FontAsset = std::stoi(params.at(2));
			text.RuntimeFont = ResourceDatabase::GetFontRuntimeResource(text.FontAsset);
			Commands::ExecuteRawValueCommand<UUID, TextComponent>(&text.FontAsset, oldFont, entity, "Assistant-TextComponent-Font");

			return "Success";
		}
		else if (params.at(1) == "Color")
		{
			bool hasAlpha = true;

			if (params.size() < 4)
				return "No G value provided.";
			else if (params.size() < 5)
				return "No B value provided.";
			else if (params.size() < 6)
				hasAlpha = false;

			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "R value is not float.";
			if (ComponentHelper::CanConvertToFloat(params.at(3)) == false)
				return "G value is not float.";
			if (ComponentHelper::CanConvertToFloat(params.at(4)) == false)
				return "B value is not float.";

			if (hasAlpha)
			{
				if (ComponentHelper::CanConvertToFloat(params.at(5)) == false)
					return "A value is not float.";
			}

			glm::vec4 color = text.Color;
			text.Color.x = std::stof(params.at(2)) / 255;
			text.Color.y = std::stof(params.at(3)) / 255;
			text.Color.z = std::stof(params.at(4)) / 255;

			if (hasAlpha)
				text.Color.w = std::stof(params.at(5)) / 255;
			else
				text.Color.w = 1.0f;

			Commands::ExecuteRawValueCommand<glm::vec4, TextComponent>(&text.Color, color, entity, "Assistant-TextComponent-Color");

			return "Success";
		}
		else if (params.at(1) == "Kerning")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "Parameter is not float.";

			float kerning = text.Kerning;
			text.Kerning = std::stof(params.at(2));
			Commands::ExecuteRawValueCommand<float, TextComponent>(&text.Kerning, kerning, entity, "Assistant-TextComponent-Kerning");

			return "Success";
		}
		else if (params.at(1) == "LineSpacing")
		{
			if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
				return "Parameter is not float.";

			float lineSpacing = text.LineSpacing;
			text.LineSpacing = std::stof(params.at(2));
			Commands::ExecuteRawValueCommand<float, TextComponent>(&text.LineSpacing, lineSpacing, entity, "Assistant-TextComponent-LineSpacing");

			return "Success";
		}
		else
			return "Invalid property.";

		return "None action was taken.";
	}

#pragma endregion
}