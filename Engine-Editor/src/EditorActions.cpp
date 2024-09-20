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
			if (params.size() < 1)
				return "No entity name provided.";

			Entity entity = m_Scene->FindEntityByName(params.at(0));

			if (entity == Entity())
				return "Entity with name " + params.at(0) + " not found";

			return std::to_string(entity.GetUUID());
		}
		else if (actionName == "Exists")
		{
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
		else if (actionName == "GetAllEntities")
		{
			auto view = m_Scene->GetEntitiesWith<TransformComponent>();
			std::string entities = "";
			for (auto entity : view)
			{
				entities += Entity(entity, m_Scene.get()).GetName() + " (UUID:" +
					std::to_string(Entity(entity, m_Scene.get()).GetUUID()) + "), ";
			}
			return entities;
		}
		else if (actionName == "ChangeName")
		{
			if (params.size() < 1)
				return "No entity name provided.";
			else if(params.size() < 2)
				return "No new name provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(!entity.HasComponent<TagComponent>())
				return "Entity does not have TagComponent.";

			std::string& tag = entity.GetComponent<TagComponent>().Tag;
			std::string name = tag;
			tag = params.at(1);

			Commands::ExecuteRawValueCommand(&tag, name, "Assistant-TagComponent-Tag");

			return "Success";
		}
		else if (actionName == "CreateEntity")
		{
			if (params.size() < 1)
				return "No entity name provided.";

			Commands::ExecuteCommand<Commands::CreateEntityCommand>(Commands::CommandArgs(params.at(0), {}, m_Scene, *m_SelectedEntity));

			UUID uuid = m_Scene->FindEntityByName(params.at(0)).GetUUID();

			return "Entity created with UUID: " + uuid;
		}
		else if (actionName == "DeleteEntity")
		{
			if (params.size() < 1)
				return "No entity UUID provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			Commands::ExecuteCommand<Commands::DeleteEntityCommand>(Commands::CommandArgs("", entity, m_Scene, *m_SelectedEntity));

			return "Entity removed";
		}
		else if (actionName == "GetAnyChildren")
		{
			if (params.size() < 1)
				return "No entity UUID provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

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
			if (params.size() < 1)
				return "No entity UUID provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

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
		else if (actionName == "SetParent")
		{
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

			if(params.at(1) == "" || params.at(1) == "NULL" || params.at(1) == "null" || params.at(1) == "0")
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
		else if (actionName == "IsChild")
		{
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

			if(entity.IsChild(child))
				return "true";
			else
				return "false";
		}
		else if(actionName == "IsChildOfAny")
		{
			if(params.size() < 1)
				return "No child entity UUID provided.";
			else if(params.size() < 2)
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

			if(entity.IsChildOfAny(child))
				return "true";
			else
				return "false";
		}
		else if (actionName == "AddComponent")
		{
			if (params.size() < 1)
				return "No entity UUID provided.";
			else if (params.size() < 2)
				return "No component name provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(ComponentHelper::AddComponent(params.at(1), entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if(actionName == "RemoveComponent")
		{
			if (params.size() < 1)
				return "No entity UUID provided.";
			else if (params.size() < 2)
				return "No component name provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(ComponentHelper::RemoveComponent(params.at(1), entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "HasComponent")
		{
			if (params.size() < 1)
				return "No entity UUID provided.";
			else if (params.size() < 2)
				return "No component name provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(ComponentHelper::HasComponent(params.at(1), entity, m_Scene))
				return "true";
			else
				return "false";
		}
		else if (actionName == "InheritComponentInChildren")
		{
			if (params.size() < 1)
				return "No entity UUID provided.";
			else if (params.size() < 2)
				return "No component name provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::InheritComponentInChildren, params.at(1), entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "InheritComponentFromParent")
		{
			if (params.size() < 1)
				return "No entity UUID provided.";
			else if (params.size() < 2)
				return "No component name provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::InheritComponentFromParent, params.at(1), entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "CopyComponentToChildren")
		{
			if (params.size() < 1)
				return "No entity UUID provided.";
			else if (params.size() < 2)
				return "No component name provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::CopyComponentToChildren, params.at(1), entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "CopyComponentValuesToChildren")
		{
			if (params.size() < 1)
				return "No entity UUID provided.";
			else if (params.size() < 2)
				return "No component name provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::CopyComponentValuesToChildren, params.at(1), entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "CopyComponentWithValuesToChildren")
		{
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
		else if (actionName == "IsInheritedFromParent")
		{
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
		else if (actionName == "IsInheritedInChildren")
		{
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
		else
			return ExecuteComponentAction(actionName, params);
	}

	std::string EditorActions::ExecuteComponentAction(const std::string& actionName, std::vector<std::string> params)
	{
		if (actionName == "GetTransformComponent")
		{
			if(params.size() < 1)
				return "No entity UUID provided.";

			if(!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(!entity.HasComponent<TransformComponent>())
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
		else if (actionName == "SetTransformComponent")
		{
			if(params.size() < 1)
				return "No entity UUID provided.";
			else if(params.size() < 2)
				return "No property name provided.";
			else if(params.size() < 5)
				return "No values provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(!entity.HasComponent<TransformComponent>())
				return "Entity does not have TransformComponent.";

			TransformComponent& transform = entity.GetComponent<TransformComponent>();

			if(ComponentHelper::CanConvertToFloat(params.at(2)) == false)
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
		else if (actionName == "GetCameraComponent")
		{
			if(params.size() < 1)
				return "No entity UUID provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(!entity.HasComponent<CameraComponent>())
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
		else if (actionName == "SetCameraComponent")
		{
			if(params.size() < 1)
				return "No entity UUID provided.";
			else if(params.size() < 2)
				return "No property name provided.";
			else if(params.size() < 3)
				return "No value provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(!entity.HasComponent<CameraComponent>())
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
		else if (actionName == "GetSpriteRendererComponent")
		{
			if(params.size() < 1)
				return "No entity UUID provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(!entity.HasComponent<SpriteRendererComponent>())
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
		else if (actionName == "SetSpriteRendererComponent")
		{
			if(params.size() < 1)
				return "No entity UUID provided.";
			else if(params.size() < 2)
				return "No property name provided.";
			else if(params.size() < 3)
				return "No value provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(!entity.HasComponent<SpriteRendererComponent>())
				return "Entity does not have SpriteRendererComponent.";

			SpriteRendererComponent& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();

			if (params.at(1) == "TilingFactor")
			{
				if(ComponentHelper::CanConvertToFloat(params.at(2)) == false)
					return "Parameter is not float.";

				float factor = spriteRenderer.TilingFactor;
				spriteRenderer.TilingFactor = std::stof(params.at(2));
				Commands::ExecuteRawValueCommand<float, SpriteRendererComponent>(&spriteRenderer.TilingFactor, factor, entity, "Assistant-SpriteRendererComponent-TilingFactor");

				return "Success";
			}
			else if (params.at(1) == "Color")
			{
				if(params.size() < 4)
					return "No G value provided.";
				else if(params.size() < 5)
					return "No B value provided.";
				else if(params.size() < 6)
					params.insert(params.begin() + 5, "255");
					//return "No A value provided.";


				if(ComponentHelper::CanConvertToFloat(params.at(2)) == false)
					return "R value is not float.";
				if(ComponentHelper::CanConvertToFloat(params.at(3)) == false)
					return "G value is not float.";
				if(ComponentHelper::CanConvertToFloat(params.at(4)) == false)
					return "B value is not float.";
				if(ComponentHelper::CanConvertToFloat(params.at(5)) == false)
					return "A value is not float.";

				glm::vec4 color = spriteRenderer.Color;
				spriteRenderer.Color.x = std::stof(params.at(2)) / 255;
				spriteRenderer.Color.y = std::stof(params.at(3)) / 255;
				spriteRenderer.Color.z = std::stof(params.at(4)) / 255;
				spriteRenderer.Color.w = std::stof(params.at(5)) / 255;

				Commands::ExecuteRawValueCommand<glm::vec4, SpriteRendererComponent>(&spriteRenderer.Color, color, entity, "Assistant-SpriteRendererComponent-Color");

				return "Success";
			}
			else if (params.at(1) == "Texture")
			{
				if(ComponentHelper::CanConvertToInteger(params.at(2)) == false)
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
		else if (actionName == "GetCircleRendererComponent")
		{
			if(params.size() < 1)
				return "No entity UUID provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(!entity.HasComponent<CircleRendererComponent>())
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
		else if (actionName == "SetCircleRendererComponent")
		{
			if(params.size() < 1)
				return "No entity UUID provided.";
			else if(params.size() < 2)
				return "No property name provided.";
			else if(params.size() < 3)
				return "No value provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(entity.HasComponent<CircleRendererComponent>() == false)
				return "Entity does not have CircleRendererComponent.";

			if(entity.HasComponent<CircleRendererComponent>() == false)
				return "Entity does not have CircleRendererComponent.";

			CircleRendererComponent& circleRenderer = entity.GetComponent<CircleRendererComponent>();

			if (params.at(1) == "Thickness")
			{
				if(ComponentHelper::CanConvertToFloat(params.at(2)) == false)
					return "Parameter is not float.";

				float thickness = circleRenderer.Thickness;
				circleRenderer.Thickness = std::stof(params.at(2));
				Commands::ExecuteRawValueCommand<float, CircleRendererComponent>(&circleRenderer.Thickness, thickness, entity, "Assistant-CircleRendererComponent-Thickness");

				return "Success";
			}
			else if (params.at(1) == "Fade")
			{
				if(ComponentHelper::CanConvertToFloat(params.at(2)) == false)
					return "Parameter is not float.";

				float fade = circleRenderer.Fade;
				circleRenderer.Fade = std::stof(params.at(2));
				Commands::ExecuteRawValueCommand<float, CircleRendererComponent>(&circleRenderer.Fade, fade, entity, "Assistant-CircleRendererComponent-Fade");

				return "Success";
			}
			else if (params.at(1) == "Color")
			{
				if(params.size() < 4)
					return "No G value provided.";
				else if(params.size() < 5)
					return "No B value provided.";
				else if(params.size() < 6)
					params.insert(params.begin() + 5, "255");
					//return "No A value provided.";

				if(ComponentHelper::CanConvertToFloat(params.at(2)) == false)
					return "R value is not float.";
				if(ComponentHelper::CanConvertToFloat(params.at(3)) == false)
					return "G value is not float.";
				if(ComponentHelper::CanConvertToFloat(params.at(4)) == false)
					return "B value is not float.";
				if(ComponentHelper::CanConvertToFloat(params.at(5)) == false)
					return "A value is not float.";

				glm::vec4 color = circleRenderer.Color;
				circleRenderer.Color.x = std::stof(params.at(2)) / 255;
				circleRenderer.Color.y = std::stof(params.at(3)) / 255;
				circleRenderer.Color.z = std::stof(params.at(4)) / 255;
				circleRenderer.Color.w = std::stof(params.at(5)) / 255;

				Commands::ExecuteRawValueCommand<glm::vec4, CircleRendererComponent>(&circleRenderer.Color, color, entity, "Assistant-CircleRendererComponent-Color");

				return "Success";
			}
			else
				return "Invalid property.";

			return "None action was taken.";
		}
		else if (actionName == "GetRigidBody2DComponent")
		{
			if(params.size() < 1)
				return "No entity UUID provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(!entity.HasComponent<RigidBody2DComponent>())
				return "Entity does not have RigidBody2DComponent.";

			RigidBody2DComponent& rigidBody2D = entity.GetComponent<RigidBody2DComponent>();

			std::string rigidBody2DString = "Type: ";
			rigidBody2DString += rigidBody2D.Type == RigidBody2DComponent::BodyType::Static ? "Static" : rigidBody2D.Type == RigidBody2DComponent::BodyType::Dynamic ? "Dynamic" : "Kinematic";
			rigidBody2DString += " FixedRotation: " + std::to_string(rigidBody2D.FixedRotation);

			return rigidBody2DString;
		}
		else if (actionName == "SetRigidBody2DComponent")
		{
			if(params.size() < 1)
				return "No entity UUID provided.";
			else if(params.size() < 2)
				return "No property name provided.";
			else if(params.size() < 3)
				return "No value provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(!entity.HasComponent<RigidBody2DComponent>())
				return "Entity does not have RigidBody2DComponent.";

			RigidBody2DComponent& rigidBody2D = entity.GetComponent<RigidBody2DComponent>();

			if (params.at(1) == "FixedRotation")
			{
				if(params.at(2) == "true" || params.at(2) == "True")
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
			else if(params.at(1) == "Type")
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
		else if (actionName == "GetBoxCollider2DComponent")
		{
			if(params.size() < 1)
				return "No entity UUID provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(!entity.HasComponent<BoxCollider2DComponent>())
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
		else if (actionName == "SetBoxCollider2DComponent")
		{
			if(params.size() < 1)
				return "No entity UUID provided.";
			else if(params.size() < 2)
				return "No property name provided.";
			else if(params.size() < 3)
				return "No value provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(!entity.HasComponent<BoxCollider2DComponent>())
				return "Entity does not have BoxCollider2DComponent.";

			BoxCollider2DComponent& boxCollider2D = entity.GetComponent<BoxCollider2DComponent>();

			if (params.at(1) == "Offset")
			{
				if(params.size() < 4)
					return "No Y value provided.";

				if(ComponentHelper::CanConvertToFloat(params.at(2)) == false)
					return "Parameter is not float.";
				if(ComponentHelper::CanConvertToFloat(params.at(3)) == false)
					return "Parameter is not float.";

				glm::vec2 offset = boxCollider2D.Offset;
				glm::vec2 newOffset = glm::vec2(std::stof(params.at(2)), std::stof(params.at(3)));
				boxCollider2D.Offset = newOffset;
				Commands::ExecuteRawValueCommand<glm::vec2, BoxCollider2DComponent>(&boxCollider2D.Offset, offset, entity, "Assistant-BoxCollider2DComponent-Offset");

				return "Success";
			}
			else if (params.at(1) == "Size")
			{
				if(params.size() < 4)
					return "No Y value provided.";

				if(ComponentHelper::CanConvertToFloat(params.at(2)) == false)
					return "Parameter is not float.";
				if(ComponentHelper::CanConvertToFloat(params.at(3)) == false)
					return "Parameter is not float.";

				glm::vec2 size = boxCollider2D.Size;
				glm::vec2 newSize = glm::vec2(std::stof(params.at(2)), std::stof(params.at(3)));
				boxCollider2D.Size = newSize;
				Commands::ExecuteRawValueCommand<glm::vec2, BoxCollider2DComponent>(&boxCollider2D.Size, size, entity, "Assistant-BoxCollider2DComponent-Size");

				return "Success";
			}
			else if (params.at(1) == "Density")
			{
				if(ComponentHelper::CanConvertToFloat(params.at(2)) == false)
					return "Parameter is not float.";

				float density = boxCollider2D.Density;
				boxCollider2D.Density = std::stof(params.at(2));
				Commands::ExecuteRawValueCommand<float, BoxCollider2DComponent>(&boxCollider2D.Density, density, entity, "Assistant-BoxCollider2DComponent-Density");

				return "Success";
			}
			else if (params.at(1) == "Friction")
			{
				if(ComponentHelper::CanConvertToFloat(params.at(2)) == false)
					return "Parameter is not float.";

				float friction = boxCollider2D.Friction;
				boxCollider2D.Friction = std::stof(params.at(2));
				Commands::ExecuteRawValueCommand<float, BoxCollider2DComponent>(&boxCollider2D.Friction, friction, entity, "Assistant-BoxCollider2DComponent-Friction");

				return "Success";
			}
			else if (params.at(1) == "Restitution")
			{
				if(ComponentHelper::CanConvertToFloat(params.at(2)) == false)
					return "Parameter is not float.";

				float restitution = boxCollider2D.Restitution;
				boxCollider2D.Restitution = std::stof(params.at(2));
				Commands::ExecuteRawValueCommand<float, BoxCollider2DComponent>(&boxCollider2D.Restitution, restitution, entity, "Assistant-BoxCollider2DComponent-Restitution");

				return "Success";
			}
			else if (params.at(1) == "RestitutionThreshold")
			{
				if(ComponentHelper::CanConvertToFloat(params.at(2)) == false)
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
		else if (actionName == "GetCircleCollider2DComponent")
		{
			if(params.size() < 1)
				return "No entity UUID provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(!entity.HasComponent<CircleCollider2DComponent>())
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
		else if (actionName == "SetCircleCollider2DComponent")
		{
			if(params.size() < 1)
				return "No entity UUID provided.";
			else if(params.size() < 2)
				return "No property name provided.";
			else if(params.size() < 3)
				return "No value provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(!entity.HasComponent<CircleCollider2DComponent>())
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
		else if (actionName == "GetTextComponent")
		{
			if(params.size() < 1)
				return "No entity UUID provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(!entity.HasComponent<TextComponent>())
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
		else if (actionName == "SetTextComponent")
		{
			if(params.size() < 1)
				return "No entity UUID provided.";
			else if(params.size() < 2)
				return "No property name provided.";
			else if(params.size() < 3)
				return "No value provided.";

			if (!ComponentHelper::CanConvertToUUID(params.at(0)))
				return "First parameter is not UUID.";

			Entity entity = m_Scene->GetEntityByUUID(std::stoll(params.at(0)));

			if (entity == Entity())
				return "Entity with UUID " + params.at(0) + " not found";

			if(!entity.HasComponent<TextComponent>())
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
				if (params.size() < 4)
					return "No G value provided.";
				else if (params.size() < 5)
					return "No B value provided.";
				else if (params.size() < 6)
					params.insert(params.begin() + 5, "255");
					//return "No A value provided.";

				if (ComponentHelper::CanConvertToFloat(params.at(2)) == false)
					return "R value is not float.";
				if (ComponentHelper::CanConvertToFloat(params.at(3)) == false)
					return "G value is not float.";
				if (ComponentHelper::CanConvertToFloat(params.at(4)) == false)
					return "B value is not float.";
				if (ComponentHelper::CanConvertToFloat(params.at(5)) == false)
					return "A value is not float.";

				glm::vec4 color = text.Color;
				text.Color.x = std::stof(params.at(2)) / 255;
				text.Color.y = std::stof(params.at(3)) / 255;
				text.Color.z = std::stof(params.at(4)) / 255;
				text.Color.w = std::stof(params.at(5)) / 255;

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
		else
			return "Invalid action name.";
	}
}