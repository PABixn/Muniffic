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

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			return std::to_string(entity.GetUUID());
		}
		else if (actionName == "Exists")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

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
				entities += (Entity(entity, m_Scene.get()).GetName()) + ",";
			}
			return entities;
		}
		else if (actionName == "ChangeName")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			std::string& tag = entity.GetComponent<TagComponent>().Tag;
			std::string name = tag;
			tag = params[1];

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
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			Commands::ExecuteCommand<Commands::DeleteEntityCommand>(Commands::CommandArgs("", entity, m_Scene, *m_SelectedEntity));

			return "Success";
		}
		else if (actionName == "GetAnyChildren")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

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

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

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

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			std::optional<Entity> parent = entity.GetParent();

			if (parent.has_value())
				return parent.value().GetName();
			else
				return "NULL";
		}
		else if (actionName == "SetParent")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			std::optional<Entity> parent;

			if(params[1] == "" || params[1] == "NULL")
				parent = std::nullopt;
			else
			{
				parent = m_Scene->FindEntityByName(params[1]);
				if (parent.value() == Entity())
					return "Parent with name " + params[1] + " not found";
			}
			
			Commands::ExecuteChangeParentCommand(entity, parent, m_Scene);
				
			return "Success";
		}
		else if (actionName == "IsChild")
		{
			Entity child = m_Scene->FindEntityByName(params[0]);

			if (child == Entity())
				return "Entity with name " + params[0] + " not found";

			Entity entity = m_Scene->FindEntityByName(params[1]);

			if (entity == Entity())
				return "Entity with name " + params[1] + " not found";

			if(entity.IsChild(child))
				return "true";
			else
				return "false";
		}
		else if(actionName == "IsChildOfAny")
		{
			Entity child = m_Scene->FindEntityByName(params[0]);

			if (child == Entity())
				return "Entity with name " + params[0] + " not found";

			Entity entity = m_Scene->FindEntityByName(params[1]);

			if (entity == Entity())
				return "Entity with name " + params[1] + " not found";

			if(entity.IsChildOfAny(child))
				return "true";
			else
				return "false";
		}
		else if (actionName == "AddComponent")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			if(ComponentHelper::AddComponent(params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if(actionName == "RemoveComponent")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			if(ComponentHelper::RemoveComponent(params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "HasComponent")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			if(ComponentHelper::HasComponent(params[1], entity, m_Scene))
				return "true";
			else
				return "false";
		}
		else if (actionName == "InheritComponentInChildren")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			if(ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::InheritComponentInChildren, params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "InheritComponentFromParent")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			if(ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::InheritComponentFromParent, params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "CopyComponentToChildren")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			if(ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::CopyComponentToChildren, params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "CopyComponentValuesToChildren")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			if(ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::CopyComponentValuesToChildren, params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "CopyComponentWithValuesToChildren")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			if (ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::CopyComponentWithValuesToChildren, params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "IsInheritedFromParent")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			if (ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::IsInheritedFromParent, params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
		else if (actionName == "IsInheritedInChildren")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			if (ComponentHelper::ExecuteComponentAction(ComponentHelper::ComponentAction::IsInheritedInChildren, params[1], entity, m_Scene))
				return "Success";
			else
				return "Failed";
		}
	}

	std::string EditorActions::ExecuteComponentAction(const std::string& actionName, std::vector<std::string> params)
	{
		if (actionName == "GetTransformComponent")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

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
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			TransformComponent& transform = entity.GetComponent<TransformComponent>();

			if(ComponentHelper::CanConvertToFloat(params[3]) == false)
				return "Parameter is not float.";

			if (params[1] == "Position")
			{
				if (params[2] == "X")
				{
					float x = transform.Translation.x;
					transform.Translation.x = std::stof(params[3]);
					Commands::ExecuteRawValueCommand<float, TransformComponent>(&transform.Translation.x, x, entity, "Assistant-TransformComponent-Translation-X");

					return "Success";
				}
				else if (params[2] == "Y")
				{
					float y = transform.Translation.y;
					transform.Translation.y = std::stof(params[3]);
					Commands::ExecuteRawValueCommand<float, TransformComponent>(&transform.Translation.y, y, entity, "Assistant-TransformComponent-Translation-Y");

					return "Success";
				}
				else if (params[2] == "Z")
				{
					float z = transform.Translation.z;
					transform.Translation.z = std::stof(params[3]);
					Commands::ExecuteRawValueCommand<float, TransformComponent>(&transform.Translation.z, z, entity, "Assistant-TransformComponent-Translation-Z");

					return "Success";
				}
				else 
					return "Invalid parameter.";
			}
			else if (params[1] == "Rotation")
			{
				if (params[2] == "X")
				{
					float x = transform.Rotation.x;
					transform.Rotation.x = std::stof(params[3]);
					Commands::ExecuteRawValueCommand<float, TransformComponent>(&transform.Rotation.x, x, entity, "Assistant-TransformComponent-Rotation-X");

					return "Success";
				}
				else if (params[2] == "Y")
				{
					float y = transform.Rotation.y;
					transform.Rotation.y = std::stof(params[3]);
					Commands::ExecuteRawValueCommand<float, TransformComponent>(&transform.Rotation.y, y, entity, "Assistant-TransformComponent-Rotation-Y");

					return "Success";
				}
				else if (params[2] == "Z")
				{
					float z = transform.Rotation.z;
					transform.Rotation.z = std::stof(params[3]);
					Commands::ExecuteRawValueCommand<float, TransformComponent>(&transform.Rotation.z, z, entity, "Assistant-TransformComponent-Rotation-Z");

					return "Success";
				}
				else 
					return "Invalid parameter.";
			}
			else if (params[1] == "Scale")
			{
				if (params[2] == "X")
				{
					float x = transform.Scale.x;
					transform.Scale.x = std::stof(params[3]);
					Commands::ExecuteRawValueCommand<float, TransformComponent>(&transform.Scale.x, x, entity, "Assistant-TransformComponent-Scale-X");

					return "Success";
				}
				else if (params[2] == "Y")
				{
					float y = transform.Scale.y;
					transform.Scale.y = std::stof(params[3]);
					Commands::ExecuteRawValueCommand<float, TransformComponent>(&transform.Scale.y, y, entity, "Assistant-TransformComponent-Scale-Y");

					return "Success";
				}
				else if (params[2] == "Z")
				{
					float z = transform.Scale.z;
					transform.Scale.z = std::stof(params[3]);
					Commands::ExecuteRawValueCommand<float, TransformComponent>(&transform.Scale.z, z, entity, "Assistant-TransformComponent-Scale-Z");

					return "Success";
				}
				else 
					return "Invalid parameter.";
			}
			else
				return "Invalid parameter.";

			return "None action was taken.";
		}
		else if (actionName == "GetCameraComponent")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

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
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			CameraComponent& camera = entity.GetComponent<CameraComponent>();
			SceneCamera& sceneCamera = camera.Camera;

			if (params[1] == "Primary")
			{
				if (ComponentHelper::CanConvertToInteger(params[2]) != false)
				{
					bool primary = camera.Primary;
					camera.Primary = std::stoi(params[2]);
					Commands::ExecuteRawValueCommand(&camera.Primary, primary, "Assistant-CameraComponent-Primary");

					return "Success";
				}
				else
					return "Parameter is not integer.";
			}
			else if (params[1] == "FixedAspectRatio")
			{
				if (ComponentHelper::CanConvertToInteger(params[2]) != false)
				{
					bool fixedAspectRatio = camera.FixedAspectRatio;
					camera.FixedAspectRatio = std::stoi(params[2]);
					Commands::ExecuteRawValueCommand(&camera.FixedAspectRatio, fixedAspectRatio, "Assistant-CameraComponent-FixedAspectRatio");

					return "Success";
				}
				else
					return "Parameter is not integer.";
			}
			else if (params[1] == "ProjectionType")
			{
				if (params[2] == "Orthographic")
				{
					Commands::ExecuteValueCommand<SceneCamera::ProjectionType>([&sceneCamera](SceneCamera::ProjectionType projectionType)
						{
							sceneCamera.SetProjectionType(projectionType);
						}, SceneCamera::ProjectionType::Orthographic, sceneCamera.GetProjectionType(), "Assistant-CameraComponent-ProjectionType");

					return "Success";
				}
				else if (params[2] == "Perspective")
				{
					Commands::ExecuteValueCommand<SceneCamera::ProjectionType>([&sceneCamera](SceneCamera::ProjectionType projectionType)
						{
							sceneCamera.SetProjectionType(projectionType);
						}, SceneCamera::ProjectionType::Perspective, sceneCamera.GetProjectionType(), "Assistant-CameraComponent-ProjectionType");

					return "Success";
				}
				else
					return "Invalid parameter.";
			}
			else if (params[1] == "OrthographicSize")
			{
				if (ComponentHelper::CanConvertToFloat(params[2]) != false)
				{
					Commands::ExecuteValueCommand<float>([&sceneCamera](float size)
						{
							sceneCamera.SetOrthographicSize(size);
						}, std::stof(params[2]), sceneCamera.GetOrthographicSize(), "Assistant-CameraComponent-OrthographicSize");

					return "Success";
				}
				else
					return "Parameter is not float.";
			}
			else if (params[1] == "OrthographicNearClip")
			{
				if (ComponentHelper::CanConvertToFloat(params[2]) != false)
				{
					Commands::ExecuteValueCommand<float>([&sceneCamera](float nearClip)
						{
							sceneCamera.SetOrthographicNearClip(nearClip);
						}, std::stof(params[2]), sceneCamera.GetOrthographicNearClip(), "Assistant-CameraComponent-OrthographicNearClip");

					return "Success";
				}
				else
					return "Parameter is not float.";
			}
			else if (params[1] == "OrthographicFarClip")
			{
				if (ComponentHelper::CanConvertToFloat(params[2]) != false)
				{
					Commands::ExecuteValueCommand<float>([&sceneCamera](float farClip)
						{
							sceneCamera.SetOrthographicFarClip(farClip);
						}, std::stof(params[2]), sceneCamera.GetOrthographicFarClip(), "Assistant-CameraComponent-OrthographicFarClip");

					return "Success";
				}
				else
					return "Parameter is not float.";
			}
			else if (params[1] == "PerspectiveVerticalFOV")
			{
				if (ComponentHelper::CanConvertToFloat(params[2]) != false)
				{
					Commands::ExecuteValueCommand<float>([&sceneCamera](float verticalFOV)
						{
							sceneCamera.SetPerspectiveVerticalFOV(verticalFOV);
						}, std::stof(params[2]), sceneCamera.GetPerspectiveVerticalFOV(), "Assistant-CameraComponent-PerspectiveVerticalFOV");

					return "Success";
				}
				else
					return "Parameter is not float.";
			}
			else if (params[1] == "PerspectiveNearClip")
			{
				if (ComponentHelper::CanConvertToFloat(params[2]) != false)
				{
					Commands::ExecuteValueCommand<float>([&sceneCamera](float nearClip)
						{
							sceneCamera.SetPerspectiveNearClip(nearClip);
						}, std::stof(params[2]), sceneCamera.GetPerspectiveNearClip(), "Assistant-CameraComponent-PerspectiveNearClip");

					return "Success";
				}
				else
					return "Parameter is not float.";
			}
			else if (params[1] == "PerspectiveFarClip")
			{
				if (ComponentHelper::CanConvertToFloat(params[2]) != false)
				{
					Commands::ExecuteValueCommand<float>([&sceneCamera](float farClip)
						{
							sceneCamera.SetPerspectiveFarClip(farClip);
						}, std::stof(params[2]), sceneCamera.GetPerspectiveFarClip(), "Assistant-CameraComponent-PerspectiveFarClip");

					return "Success";
				}
				else
					return "Parameter is not float.";
			}
			else
				return "Invalid parameter.";

			return "None action was taken.";
		}
		else if (actionName == "GetSpriteRendererComponent")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			SpriteRendererComponent& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();

			std::string spriteRendererString = "Color: R: "
				+ std::to_string(spriteRenderer.Color.r) + " G: "
				+ std::to_string(spriteRenderer.Color.g) + " B: "
				+ std::to_string(spriteRenderer.Color.b) + " A: "
				+ std::to_string(spriteRenderer.Color.a) + " Texture: "
				+ spriteRenderer.Texture->GetPath() + "TextureUUID: "
				+ std::to_string(spriteRenderer.TextureUUID) + " TilingFactor: "
				+ std::to_string(spriteRenderer.TilingFactor);

			return spriteRendererString;
		}
		else if (actionName == "SetSpriteRendererComponent")
		{
			Entity entity = m_Scene->FindEntityByName(params[0]);

			if (entity == Entity())
				return "Entity with name " + params[0] + " not found";

			SpriteRendererComponent& spriteRenderer = entity.GetComponent<SpriteRendererComponent>();

			if (params[1] == "TilingFactor")
			{
				if(ComponentHelper::CanConvertToFloat(params[2]) == false)
					return "Parameter is not float.";

				float factor = spriteRenderer.TilingFactor;
				spriteRenderer.TilingFactor = std::stof(params[2]);
				Commands::ExecuteRawValueCommand<float, SpriteRendererComponent>(&spriteRenderer.TilingFactor, factor, entity, "Assistant-SpriteRendererComponent-TilingFactor");

				return "Success";
			}
			else if (params[1] == "Color")
			{
				if(ComponentHelper::CanConvertToInteger(params[2]) == false)
					return "Parameter is not integer.";
				if(ComponentHelper::CanConvertToFloat(params[3]) == false)
					return "Parameter is not integer.";
				if(ComponentHelper::CanConvertToFloat(params[4]) == false)
					return "Parameter is not integer.";
				if(ComponentHelper::CanConvertToFloat(params[5]) == false)
					return "Parameter is not integer.";

				glm::vec4 color = spriteRenderer.Color;
				spriteRenderer.Color.r = std::stoi(params[2]);
				spriteRenderer.Color.g = std::stof(params[3]);
				spriteRenderer.Color.b = std::stof(params[4]);
				spriteRenderer.Color.a = std::stof(params[5]);

				Commands::ExecuteRawValueCommand<glm::vec4, SpriteRendererComponent>(&spriteRenderer.Color, color, entity, "Assistant-SpriteRendererComponent-Color");

				return "Success";
			}
			else if (params[1] == "Texture")
			{
				if(ComponentHelper::CanConvertToInteger(params[2]) == false)
					return "Parameter is not integer.";

				Ref<Texture2D> texture = ResourceDatabase::GetTextureRuntimeResource(std::stoi(params[2]));

				if (texture->IsLoaded())
				{
					spriteRenderer.TextureUUID = std::stoi(params[2]);
					Ref<Texture2D> oldTexture = spriteRenderer.Texture;
					spriteRenderer.Texture = texture;
					Commands::ExecuteRawValueCommand<Ref<Texture2D>, SpriteRendererComponent>(&spriteRenderer.Texture, oldTexture, entity, "Assistant-SpriteRendererComponent-Texture");

					return "Success";
				}
				else 
					return "Texture could not be loaded.";
			}
			else
				return "Invalid parameter.";

			return "None action was taken.";
		}
	}
}