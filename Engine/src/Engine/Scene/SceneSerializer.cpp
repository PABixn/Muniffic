#include "egpch.h"
#include "Engine/Core/Core.h"
#include "SceneSerializer.h"
#include "Engine/Utils/YAMLConversion.h"
#include "Entity.h"
#include "Components.h"
#include <optional>
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Project/Project.h"
#include "Engine/Resources/ResourceSerializer.h"
#include "Engine/Resources/ResourceDatabase.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include "../Engine-Editor/src/Panels/ConsolePanel.h"



namespace eg {
	ConsolePanel consolePanel;
#define READ_SCRIPT_FIELD(FieldType, Type)             \
	case ScriptFieldType::FieldType:                   \
	{                                                  \
		Type data = scriptField["Value"].as<Type>();    \
		fieldInstance.SetValue(data);                  \
		break;                                         \
	}

	

	static std::string RigidBody2dBodyTypeToString(RigidBody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case RigidBody2DComponent::BodyType::Static:    return "Static";
		case RigidBody2DComponent::BodyType::Dynamic:   return "Dynamic";
		case RigidBody2DComponent::BodyType::Kinematic: return "Kinematic";
		}
		EG_CORE_ASSERT(false, "Unknown RigidBody2DComponent::BodyType!");
		ConsolePanel::Log("File: SceneSerializer.cpp - Unknown type of RigidBody2DComponent", ConsolePanel::LogType::Error);
		return std::string();
	
	}

	static RigidBody2DComponent::BodyType RigidBody2dBodyTypeFromString(const std::string& bodyType)
	{
		if (bodyType == "Static")    return RigidBody2DComponent::BodyType::Static;
		if (bodyType == "Dynamic")   return RigidBody2DComponent::BodyType::Dynamic;
		if (bodyType == "Kinematic") return RigidBody2DComponent::BodyType::Kinematic;
		EG_CORE_ASSERT(false, "Unknown RigidBody2DComponent::BodyType!");
		ConsolePanel::Log("File: SceneSerializer.cpp - Unknown type of RigidBody2DComponent", ConsolePanel::LogType::Error);
		return RigidBody2DComponent::BodyType::Static;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		EG_CORE_ASSERT(entity.HasComponent<IDComponent>(), "Entity has no IDComponent!");

		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetUUID();
		out << YAML::Key << "Name" << YAML::Value << "Cube";

		if (entity.GetParent().has_value())
			out << YAML::Key << "Parent" << YAML::Value << entity.GetParent().value().GetUUID();

		if (entity.GetChildren().size() > 0)
		{
			out << YAML::Key << "Children" << YAML::Value << YAML::BeginSeq;

			for (auto child : entity.GetChildren())
				out << YAML::Value << child.GetUUID();

			out << YAML::EndSeq; // EntityInfo
		}


		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap; // TagComponent
		}

		if (entity.HasComponent<TransformComponent>())
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent
			out << YAML::Key << "Translation" << YAML::Value << transform.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << transform.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transform.Scale;
			out << YAML::Key << "IsInherited" << YAML::Value << transform.isInherited;
			out << YAML::Key << "IsInheritedInChildren" << YAML::Value << transform.isInheritedInChildren;
			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
			out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.TilingFactor;
			if(spriteRendererComponent.Texture)
				out << YAML::Key << "TextureUUID" << YAML::Value << spriteRendererComponent.TextureUUID;
			out << YAML::Key << "IsInherited" << YAML::Value << spriteRendererComponent.isInherited;
			out << YAML::Key << "IsInheritedInChildren" << YAML::Value << spriteRendererComponent.isInheritedInChildren;
			out << YAML::EndMap; // SpriteRendererComponent
		}

		if (entity.HasComponent<SpriteRendererSTComponent>())
		{
			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererSTComponent>();
			out << YAML::Key << "SpriteRendererSTComponent";
			out << YAML::BeginMap; // SpriteRendererComponent
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
			out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.TilingFactor;
			if (spriteRendererComponent.SubTexture->GetTexture())
			{
				out << YAML::Key << "TextureUUID" << YAML::Value << spriteRendererComponent.SubTextureUUID;
				out << YAML::Key << "MinCoords" << YAML::Value << spriteRendererComponent.SubTexture->GetCoords(0);
				out << YAML::Key << "MaxCoords" << YAML::Value << spriteRendererComponent.SubTexture->GetCoords(2);
			}
			out << YAML::EndMap; 
		}

		if (entity.HasComponent<CircleRendererComponent>())
		{
			auto& circleRendererComponent = entity.GetComponent<CircleRendererComponent>();
			out << YAML::Key << "CircleRendererComponent";
			out << YAML::BeginMap; // CircleRendererComponent
			out << YAML::Key << "Color" << YAML::Value << circleRendererComponent.Color;
			out << YAML::Key << "Thickness" << YAML::Value << circleRendererComponent.Thickness;
			out << YAML::Key << "Fade" << YAML::Value << circleRendererComponent.Fade;
			out << YAML::Key << "IsInherited" << YAML::Value << circleRendererComponent.isInherited;
			out << YAML::Key << "IsInheritedInChildren" << YAML::Value << circleRendererComponent.isInheritedInChildren;
			out << YAML::EndMap; // CircleRendererComponent
		}

		if (entity.HasComponent<AnimatorComponent>())
		{
			auto& animatorComponent = entity.GetComponent<AnimatorComponent>();
			out << YAML::Key << "AnimatorComponent";
			out << YAML::BeginMap; // AnimatorComponent
			out << YAML::Key << "Speed" << YAML::Value << animatorComponent.Animator2D->GetSpeed();
			out << YAML::Key << "Animations" << YAML::BeginSeq;
			for (auto& animation : *animatorComponent.Animator2D->GetAnimations())
			{
				out << animation->GetID();
			}

			out << YAML::EndSeq; // Animations
			out << YAML::Key << "Transitions" << YAML::BeginSeq;
			for (auto& transition : *animatorComponent.Animator2D->GetTransitions())
			{
				out << YAML::BeginMap;
				out << YAML::Key << "From" << YAML::Value << transition.first;
				out << YAML::Key << "To" << YAML::Value << transition.second;
				out << YAML::EndMap;
			}

			out << YAML::EndSeq; // Transitions
			out << YAML::EndMap; // AnimatorComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap; // Camera

			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			out << YAML::Key << "IsInherited" << YAML::Value << cameraComponent.isInherited;
			out << YAML::Key << "IsInheritedInChildren" << YAML::Value << cameraComponent.isInheritedInChildren;
			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<ScriptComponent>())
		{
			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap; 
			auto& scriptComponent = entity.GetComponent<ScriptComponent>();

			out << YAML::Key << "Scripts" << YAML::Value << YAML::BeginSeq; 

			for (UUID script : scriptComponent.Scripts)
			{
				out << YAML::BeginMap; 
				out << YAML::Key << "ScriptUUID" << YAML::Value << script;

				std::string name = ResourceDatabase::GetResourceName(script);

				// Fields
				Ref<ScriptClass> scriptClass = ScriptEngine::GetEntityClass(name);

				if (!scriptClass)
					continue;

				const auto& fields = scriptClass->GetFields();

				if (fields.empty())
					continue;

				out << YAML::Key << "ScriptFields" << YAML::Value << YAML::BeginSeq;
				auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);

				for (auto& [name, field] : fields)
				{
					if (entityFields.find(field.Name) == entityFields.end())
						continue;

					out << YAML::BeginMap;

					out << YAML::Key << "Name" << YAML::Value << field.Name;
					ScriptFieldInstance& fieldInstance = entityFields.at(name);
					out << YAML::Key << "Type" << YAML::Value << Utils::ScriptFieldTypeToString(field.Type);
					out << YAML::Key << "Value" << YAML::Value;

					switch (field.Type)
					{
						case ScriptFieldType::Float:	out << fieldInstance.GetValue<float>(); break;
						case ScriptFieldType::Int32:	out << fieldInstance.GetValue<int>(); break;
						case ScriptFieldType::UInt32:	out << fieldInstance.GetValue<uint32_t>(); break;
						case ScriptFieldType::Int64:	out << fieldInstance.GetValue<int64_t>(); break;
						case ScriptFieldType::Bool:		out << fieldInstance.GetValue<bool>(); break;
						case ScriptFieldType::Short:	out << fieldInstance.GetValue<short>(); break;
						case ScriptFieldType::UShort:	out << fieldInstance.GetValue<unsigned short>(); break;
						case ScriptFieldType::Byte:		out << fieldInstance.GetValue<unsigned char>(); break;
						case ScriptFieldType::SByte:	out << fieldInstance.GetValue<char>(); break;
						case ScriptFieldType::Double:	out << fieldInstance.GetValue<double>(); break;
						case ScriptFieldType::Vector2:	out << fieldInstance.GetValue<glm::vec2>(); break;
						case ScriptFieldType::Vector3:	out << fieldInstance.GetValue<glm::vec3>(); break;
						case ScriptFieldType::Vector4:	out << fieldInstance.GetValue<glm::vec4>(); break;
						case ScriptFieldType::Entity:	out << fieldInstance.GetValue<float>(); break;
					}

					out << YAML::EndMap;
				}

				out << YAML::EndSeq;
				out << YAML::EndMap;
			}

			out << YAML::EndSeq;

			out << YAML::Key << "IsInherited" << YAML::Value << scriptComponent.isInherited;
			out << YAML::Key << "IsInheritedInChildren" << YAML::Value << scriptComponent.isInheritedInChildren;
			out << YAML::EndMap; // ScriptComponent
		}

		if (entity.HasComponent<RigidBody2DComponent>())
		{
			auto& rb = entity.GetComponent<RigidBody2DComponent>();
			out << YAML::Key << "RigidBody2DComponent";
			out << YAML::BeginMap; // RigidBody2DComponent
			out << YAML::Key << "Body Type" << YAML::Value << RigidBody2dBodyTypeToString(rb.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rb.FixedRotation;
			out << YAML::Key << "IsInherited" << YAML::Value << rb.isInherited;
			out << YAML::Key << "IsInheritedInChildren" << YAML::Value << rb.isInheritedInChildren;
			out << YAML::EndMap; // RigidBody2DComponent
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			auto& bc = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap; // BoxCollider2DComponent
			out << YAML::Key << "Offset" << YAML::Value << bc.Offset;
			out << YAML::Key << "Size" << YAML::Value << bc.Size;
			out << YAML::Key << "Density" << YAML::Value << bc.Density;
			out << YAML::Key << "Friction" << YAML::Value << bc.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc.RestitutionThreshold;
			out << YAML::Key << "IsInherited" << YAML::Value << bc.isInherited;
			out << YAML::Key << "IsInheritedInChildren" << YAML::Value << bc.isInheritedInChildren;
			out << YAML::EndMap; // BoxCollider2DComponent
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			auto& cc = entity.GetComponent<CircleCollider2DComponent>();
			out << YAML::Key << "CircleCollider2DComponent";
			out << YAML::BeginMap; // CircleCollider2DComponent
			out << YAML::Key << "Offset" << YAML::Value << cc.Offset;
			out << YAML::Key << "Radius" << YAML::Value << cc.Radius;
			out << YAML::Key << "Density" << YAML::Value << cc.Density;
			out << YAML::Key << "Friction" << YAML::Value << cc.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << cc.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << cc.RestitutionThreshold;
			out << YAML::Key << "IsInherited" << YAML::Value << cc.isInherited;
			out << YAML::Key << "IsInheritedInChildren" << YAML::Value << cc.isInheritedInChildren;
			out << YAML::EndMap; // CircleCollider2DComponent
		}

		if (entity.HasComponent<TextComponent>())
		{
			out << YAML::Key << "TextComponent";
			out << YAML::BeginMap; // TextComponent

			auto& textComponent = entity.GetComponent<TextComponent>();
			out << YAML::Key << "TextString" << YAML::Value << textComponent.TextString;
			// TODO: textComponent.FontAsset
			out << YAML::Key << "Color" << YAML::Value << textComponent.Color;
			out << YAML::Key << "Kerning" << YAML::Value << textComponent.Kerning;
			out << YAML::Key << "LineSpacing" << YAML::Value << textComponent.LineSpacing;
			out << YAML::Key << "IsInherited" << YAML::Value << textComponent.isInherited;
			out << YAML::Key << "IsInheritedInChildren" << YAML::Value << textComponent.isInheritedInChildren;
			out << YAML::Key << "Font" << YAML::Value << textComponent.FontAsset;
			out << YAML::EndMap; // TextComponent
		}

		out << YAML::EndMap; // Entity
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
		fout.close();

		ResourceSerializer::SerializeResourceCache();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		EG_CORE_ASSERT(false, "Not implemented");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(filepath);
		}
		catch (YAML::ParserException e)
		{
			EG_CORE_ERROR("Failed to load .mnproj file '{0}'\n     {1}", filepath, e.what());
			return false;
		}

		if(!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		EG_CORE_TRACE("Deserializing scene '{0}'", sceneName);
		ConsolePanel::Log("File: SceneSerializer.cpp - Deserializing scene " + sceneName, ConsolePanel::LogType::Info);

		ResourceSerializer::DeserializeResourceCache();

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>(); 

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if(tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				EG_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);
				ConsolePanel::Log("File: SceneSerializer.cpp - Deserialized entity with name " + name, ConsolePanel::LogType::Info);

				Entity deserializedEntity = m_Scene->CreateEntityWithID(uuid, name);

				if(entity["Parent"])
					deserializedEntity.SetParent(entity["Parent"].as<UUID>());

				auto children = entity["Children"];

				if (children)
				{
					for (auto child : children)
					{
						deserializedEntity.AddChild(child.as<UUID>());
					}
				}

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transforms
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
					if(transformComponent["IsInherited"])
						tc.isInherited = transformComponent["IsInherited"].as<bool>();
					if(transformComponent["IsInheritedInChildren"])
						tc.isInheritedInChildren = transformComponent["IsInheritedInChildren"].as<bool>();
				}

				YAML::Node cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());
					
					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();

					if(cameraComponent["IsInherited"])
						cc.isInherited = cameraComponent["IsInherited"].as<bool>();
					if(cameraComponent["IsInheritedInChildren"])
						cc.isInheritedInChildren = cameraComponent["IsInheritedInChildren"].as<bool>();
				}

				auto scriptComponent = entity["ScriptComponent"];
				if (scriptComponent)
				{
					auto& sc = deserializedEntity.AddComponent<ScriptComponent>();

					auto scripts = scriptComponent["Scripts"];

					for (auto script : scripts)
					{
						if(!script["ScriptUUID"])
							continue;

						UUID uuid = script["ScriptUUID"].as<UUID>();

						sc.Scripts.push_back(uuid);

						/*std::string name = ResourceDatabase::GetResourceName(uuid);*/

						//auto scriptFields = script["ScriptFields"];
						//if (scriptFields)
						//{
						//	Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(name);
						//	EG_CORE_ASSERT(entityClass, "Entity class not found!");
						//	const auto& fields = entityClass->GetFields();
						//	auto& entityFields = ScriptEngine::GetScriptFieldMap(deserializedEntity);

						//	if (entityClass) {
						//		for (auto scriptField : scriptFields)
						//		{
						//			std::string fieldName = scriptField["Name"].as<std::string>();
						//			std::string field = scriptField["Type"].as<std::string>();
						//			ScriptFieldType fieldType = Utils::ScriptFieldTypeFromString(field);

						//			ScriptFieldInstance& fieldInstance = entityFields[fieldName];

						//			if (fields.find(name) == fields.end()) {
						//				EG_CORE_WARN("Field not found!");
						//				//ConsolePanel::Log("File: SceneSerializer.cpp - Field not found!", ConsolePanel::LogType::Error);
						//				continue;
						//			}
						//			fieldInstance.Field = fields.at(fieldName);

						//			switch (fieldType)
						//			{
						//				READ_SCRIPT_FIELD(Float, float);
						//				READ_SCRIPT_FIELD(Double, double);
						//				READ_SCRIPT_FIELD(Bool, bool);
						//				READ_SCRIPT_FIELD(Char, char);
						//				READ_SCRIPT_FIELD(Byte, uint8_t);
						//				READ_SCRIPT_FIELD(Short, int16_t);
						//				READ_SCRIPT_FIELD(Int32, int32_t);
						//				READ_SCRIPT_FIELD(Int64, int64_t);
						//				READ_SCRIPT_FIELD(SByte, int8_t);
						//				READ_SCRIPT_FIELD(UShort, uint16_t);
						//				READ_SCRIPT_FIELD(UInt32, uint32_t);
						//				READ_SCRIPT_FIELD(UInt64, uint64_t);
						//				READ_SCRIPT_FIELD(Vector2, glm::vec2);
						//				READ_SCRIPT_FIELD(Vector3, glm::vec3);
						//				READ_SCRIPT_FIELD(Vector4, glm::vec4);
						//				READ_SCRIPT_FIELD(Entity, UUID);
						//			}
						//		}
						//	}
						//}
					}

					if(scriptComponent["IsInherited"])
						sc.isInherited = scriptComponent["IsInherited"].as<bool>();
					if(scriptComponent["IsInheritedInChildren"])
						sc.isInheritedInChildren = scriptComponent["IsInheritedInChildren"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
					if(spriteRendererComponent["TilingFactor"])
						src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();

					if (spriteRendererComponent["TextureUUID"])
					{
						uint64_t textureUUID = spriteRendererComponent["TextureUUID"].as<uint64_t>();

						if (ResourceDatabase::FindResourceData(textureUUID, ResourceType::Image))
						{
							src.TextureUUID = textureUUID;
							src.Texture = ResourceDatabase::GetTextureRuntimeResource(textureUUID);
						}
						else
						{
							src.Texture = Texture2D::Create((Project::GetResourcesPath() / std::filesystem::path("resources/graphics/image_not_found.png")).string());
							EG_CORE_WARN("Texture not found: {0}", textureUUID);
						}
					}

					if(spriteRendererComponent["IsInherited"])
						src.isInherited = spriteRendererComponent["IsInherited"].as<bool>();
					if(spriteRendererComponent["IsInheritedInChildren"])
						src.isInheritedInChildren = spriteRendererComponent["IsInheritedInChildren"].as<bool>();
				}

				auto spriteRendererComponentST = entity["SpriteRendererSTComponent"];
				if (spriteRendererComponentST)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererSTComponent>();
					src.Color = spriteRendererComponentST["Color"].as<glm::vec4>();
					src.TilingFactor = spriteRendererComponentST["TilingFactor"].as<float>();
					if (spriteRendererComponentST["TextureUUID"])
					{
						uint64_t textureUUID = spriteRendererComponentST["TextureUUID"].as<uint64_t>();

						if (ResourceDatabase::FindResourceData(uuid, ResourceType::Image))
						{
							std::filesystem::path texturePath = ResourceDatabase::GetResourcePath(uuid);
							auto minCoords = spriteRendererComponentST["MinCoords"].as<glm::vec2>();
							auto maxCoords = spriteRendererComponentST["MaxCoords"].as<glm::vec2>();
							Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
							src.SubTexture = CreateRef<SubTexture2D>(texture, minCoords, maxCoords);
						}
						else
						{
							Ref<Texture2D> texture = Texture2D::Create((Project::GetResourcesPath() / std::filesystem::path("resources/graphics/image_not_found.png")).string());
							src.SubTexture = CreateRef<SubTexture2D>(texture, glm::vec2(0,0), glm::vec2(1000, 1000));
						}
					}

				}

				auto circleRendererComponent = entity["CircleRendererComponent"];
				if (circleRendererComponent)
				{
					auto& crc = deserializedEntity.AddComponent<CircleRendererComponent>();
					crc.Color = circleRendererComponent["Color"].as<glm::vec4>();
					crc.Thickness = circleRendererComponent["Thickness"].as<float>();
					crc.Fade = circleRendererComponent["Fade"].as<float>();

					if(circleRendererComponent["IsInherited"])
						crc.isInherited = circleRendererComponent["IsInherited"].as<bool>();
					if(circleRendererComponent["IsInheritedInChildren"])
						crc.isInheritedInChildren = circleRendererComponent["IsInheritedInChildren"].as<bool>();
				}

				auto animatorComponent = entity["AnimatorComponent"];
				if (animatorComponent)
				{
					auto& ac = deserializedEntity.AddComponent<AnimatorComponent>();
					ac.Animator2D = CreateRef<Animator>();
					ac.Animator2D->SetSpeed(animatorComponent["Speed"].as<float>());

					auto animations = animatorComponent["Animations"];
					if (animations)
					{
						for (auto animation : animations)
						{
							//TODO: if animation uses prefab
							Ref<Animation> anim = Animation::Create(UUID(animation.as<uint64_t>()));
							if(anim)
								ac.Animator2D->AddAnimation(anim);
							//TODO: else load all data for animation from scene file
						}
					}

					auto transitions = animatorComponent["Transitions"];
					if (transitions)
					{
						for (auto transition : transitions)
						{
							ac.Animator2D->AddTransition(transition["From"].as<uint64_t>(), transition["To"].as<uint64_t>());
						}
					}
				}

				auto rigidBody2DComponent = entity["RigidBody2DComponent"];
				if (rigidBody2DComponent)
				{
					auto& rb = deserializedEntity.AddComponent<RigidBody2DComponent>();
					rb.Type = RigidBody2dBodyTypeFromString(rigidBody2DComponent["Body Type"].as<std::string>());
					rb.FixedRotation = rigidBody2DComponent["FixedRotation"].as<bool>();

					if(rigidBody2DComponent["IsInherited"])
						rb.isInherited = rigidBody2DComponent["IsInherited"].as<bool>();
					if(rigidBody2DComponent["IsInheritedInChildren"])
						rb.isInheritedInChildren = rigidBody2DComponent["IsInheritedInChildren"].as<bool>();
				}

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent)
				{
					auto& bc = deserializedEntity.AddComponent<BoxCollider2DComponent>();
					bc.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
					bc.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
					bc.Density = boxCollider2DComponent["Density"].as<float>();
					bc.Friction = boxCollider2DComponent["Friction"].as<float>();
					bc.Restitution = boxCollider2DComponent["Restitution"].as<float>();
					bc.RestitutionThreshold = boxCollider2DComponent["RestitutionThreshold"].as<float>();

					if(boxCollider2DComponent["IsInherited"])
						bc.isInherited = boxCollider2DComponent["IsInherited"].as<bool>();
					if(boxCollider2DComponent["IsInheritedInChildren"])
						bc.isInheritedInChildren = boxCollider2DComponent["IsInheritedInChildren"].as<bool>();
				}

				auto circleCollider2DComponent = entity["CircleCollider2DComponent"];
				if (circleCollider2DComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CircleCollider2DComponent>();
					cc.Offset = circleCollider2DComponent["Offset"].as<glm::vec2>();
					cc.Radius = circleCollider2DComponent["Radius"].as<float>();
					cc.Density = circleCollider2DComponent["Density"].as<float>();
					cc.Friction = circleCollider2DComponent["Friction"].as<float>();
					cc.Restitution = circleCollider2DComponent["Restitution"].as<float>();
					cc.RestitutionThreshold = circleCollider2DComponent["RestitutionThreshold"].as<float>();

					if(circleCollider2DComponent["IsInherited"])
						cc.isInherited = circleCollider2DComponent["IsInherited"].as<bool>();
					if(circleCollider2DComponent["IsInheritedInChildren"])
						cc.isInheritedInChildren = circleCollider2DComponent["IsInheritedInChildren"].as<bool>();
				}

				auto textComponent = entity["TextComponent"];
				if (textComponent)
				{
					auto& tc = deserializedEntity.AddComponent<TextComponent>();
					tc.TextString = textComponent["TextString"].as<std::string>();
					// tc.FontAsset // TODO
					tc.Color = textComponent["Color"].as<glm::vec4>();
					tc.Kerning = textComponent["Kerning"].as<float>();
					tc.LineSpacing = textComponent["LineSpacing"].as<float>();

					if(textComponent["Font"])
						tc.FontAsset = textComponent["Font"].as<UUID>();

					if (!ResourceDatabase::FindResourceData(tc.FontAsset))
						tc.FontAsset = Font::GetDefaultFontUUID();


					tc.RuntimeFont = ResourceDatabase::GetFontRuntimeResource(tc.FontAsset);

					if(textComponent["IsInherited"])
						tc.isInherited = textComponent["IsInherited"].as<bool>();
					if(textComponent["IsInheritedInChildren"])
						tc.isInheritedInChildren = textComponent["IsInheritedInChildren"].as<bool>();
				}
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		EG_CORE_ASSERT(false, "Not implemented");
		return false;
	}
}