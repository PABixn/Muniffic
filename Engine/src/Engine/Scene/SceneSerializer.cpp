#include "egpch.h"
#include "Engine/Core/Core.h"
#include "SceneSerializer.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include "Entity.h"
#include "Components.h"
#include <optional>
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Project/Project.h"

namespace YAML
{

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			//node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			//node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			//node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

	template<>
	struct convert<eg::UUID>
	{
		static Node encode(const eg::UUID& uuid)
		{
			Node node;
			node.push_back((uint64_t)uuid);
			return node;
		}

		static bool decode(const Node& node, eg::UUID& uuid)
		{
			uuid = node.as<uint64_t>();
			return true;
		}
	};
}

namespace eg {

#define READ_SCRIPT_FIELD(FieldType, Type)             \
	case ScriptFieldType::FieldType:                   \
	{                                                  \
		Type data = scriptField["Value"].as<Type>();    \
		fieldInstance.SetValue(data);                  \
		break;                                         \
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
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
		return std::string();
	
	}

	static RigidBody2DComponent::BodyType RigidBody2dBodyTypeFromString(const std::string& bodyType)
	{
		if (bodyType == "Static")    return RigidBody2DComponent::BodyType::Static;
		if (bodyType == "Dynamic")   return RigidBody2DComponent::BodyType::Dynamic;
		if (bodyType == "Kinematic") return RigidBody2DComponent::BodyType::Kinematic;
		EG_CORE_ASSERT(false, "Unknown RigidBody2DComponent::BodyType!");
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
			out << YAML::EndMap; // TransformComponent
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
			out << YAML::Key << "TilignFactor" << YAML::Value << spriteRendererComponent.TilingFactor;
			if(spriteRendererComponent.Texture)
				out << YAML::Key << "TexturePath" << YAML::Value << spriteRendererComponent.Texture->GetPath();
			out << YAML::EndMap; // SpriteRendererComponent
		}

		if (entity.HasComponent<SpriteComponentSTComponent>())
		{
			auto& spriteRendererComponent = entity.GetComponent<SpriteComponentSTComponent>();
			out << YAML::Key << "SpriteComponentSTComponent";
			out << YAML::BeginMap; // SpriteRendererComponent
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
			out << YAML::Key << "TilingFactor" << YAML::Value << spriteRendererComponent.TilingFactor;
			if (spriteRendererComponent.SubTexture->GetTexture())
			{
				out << YAML::Key << "TexturePath" << YAML::Value << spriteRendererComponent.SubTexture->GetTexture()->GetPath();
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
			out << YAML::EndMap; // CircleRendererComponent
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

			out << YAML::EndMap; // CameraComponent
		}

		if (entity.HasComponent<ScriptComponent>())
		{
			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap; // ScriptComponent
			auto& scriptComponent = entity.GetComponent<ScriptComponent>();
			out << YAML::Key << "Name" << YAML::Value << scriptComponent.Name;

			// Fields
			Ref<ScriptClass> scriptClass = ScriptEngine::GetEntityClass(scriptComponent.Name);
			const auto& fields = scriptClass->GetFields();
			if (fields.size() > 0) {
				out << YAML::Key << "ScriptFields" << YAML::Value;
				auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);
				out << YAML::BeginSeq;
				for (auto& [name, field] : fields)
				{
					if(entityFields.find(field.Name) == entityFields.end())
						continue;
					
					out << YAML::BeginMap; // Field
					// Field has been set in the editor
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
					out << YAML::EndMap; // Field
				}
				out << YAML::EndSeq;
			}
			out << YAML::EndMap; // ScriptComponent

		}
		if (entity.HasComponent<RigidBody2DComponent>())
		{
			auto& rb = entity.GetComponent<RigidBody2DComponent>();
			out << YAML::Key << "RigidBody2DComponent";
			out << YAML::BeginMap; // RigidBody2DComponent
			out << YAML::Key << "Body Type" << YAML::Value << RigidBody2dBodyTypeToString(rb.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rb.FixedRotation;
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
			EG_CORE_ERROR("Failed to load .hazel file '{0}'\n     {1}", filepath, e.what());
			return false;
		}
		if(!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		EG_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>(); 

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if(tagComponent)
					name =  tagComponent["Tag"].as<std::string>();

				EG_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntityWithID(uuid, name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transforms
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
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
				}

				auto scriptComponent = entity["ScriptComponent"];
				if (scriptComponent)
				{
					auto& sc = deserializedEntity.AddComponent<ScriptComponent>();
					sc.Name = scriptComponent["Name"].as<std::string>();
					
					auto scriptFields = scriptComponent["ScriptFields"];
					if (scriptFields)
					{
						Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(sc.Name);
						EG_CORE_ASSERT(entityClass, "Entity class not found!");
						const auto& fields = entityClass->GetFields();
						auto& entityFields = ScriptEngine::GetScriptFieldMap(deserializedEntity);

						if (entityClass) {
							for (auto scriptField : scriptFields)
							{
								std::string fieldName = scriptField["Name"].as<std::string>();
								std::string field = scriptField["Type"].as<std::string>();
								ScriptFieldType fieldType = Utils::ScriptFieldTypeFromString(field);

								ScriptFieldInstance& fieldInstance = entityFields[fieldName];

								if (fields.find(name) == fields.end()) {
									EG_CORE_WARN("Field not found!");
									continue;
								}
								fieldInstance.Field = fields.at(fieldName);

								switch (fieldType)
								{
									READ_SCRIPT_FIELD(Float, float);
									READ_SCRIPT_FIELD(Double, double);
									READ_SCRIPT_FIELD(Bool, bool);
									READ_SCRIPT_FIELD(Char, char);
									READ_SCRIPT_FIELD(Byte, uint8_t);
									READ_SCRIPT_FIELD(Short, int16_t);
									READ_SCRIPT_FIELD(Int32, int32_t);
									READ_SCRIPT_FIELD(Int64, int64_t);
									READ_SCRIPT_FIELD(SByte, int8_t);
									READ_SCRIPT_FIELD(UShort, uint16_t);
									READ_SCRIPT_FIELD(UInt32, uint32_t);
									READ_SCRIPT_FIELD(UInt64, uint64_t);
									READ_SCRIPT_FIELD(Vector2, glm::vec2);
									READ_SCRIPT_FIELD(Vector3, glm::vec3);
									READ_SCRIPT_FIELD(Vector4, glm::vec4);
									READ_SCRIPT_FIELD(Entity, UUID);
								}
							}
						}
					}
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>();
					src.Color = spriteRendererComponent["Color"].as<glm::vec4>();
					if(spriteRendererComponent["TilingFactor"])
						src.TilingFactor = spriteRendererComponent["TilingFactor"].as<float>();
					if (spriteRendererComponent["TexturePath"])
					{
						std::string texturePath = spriteRendererComponent["TexturePath"].as<std::string>();
						//TODO: Later should be handled by the asset manager
						auto path = Project::GetAssetFileSystemPath(texturePath);
						src.Texture = Texture2D::Create(path.string());
					}
				}

				auto spriteRendererComponentST = entity["SpriteComponentSTComponent"];
				if (spriteRendererComponentST)
				{
					auto& src = deserializedEntity.AddComponent<SpriteComponentSTComponent>();
					src.Color = spriteRendererComponentST["Color"].as<glm::vec4>();
					src.TilingFactor = spriteRendererComponentST["TilingFactor"].as<float>();
					if (spriteRendererComponentST["TexturePath"])
					{
						std::string texturePath = spriteRendererComponentST["TexturePath"].as<std::string>();
						auto path = Project::GetAssetFileSystemPath(texturePath);
						auto minCoords = spriteRendererComponentST["MinCoords"].as<glm::vec2>();
						auto maxCoords = spriteRendererComponentST["MaxCoords"].as<glm::vec2>();
						Ref<Texture2D> texture = Texture2D::Create(path.string());
						src.SubTexture = CreateRef<SubTexture2D>(texture, minCoords, maxCoords);
					}

				}

				auto circleRendererComponent = entity["CircleRendererComponent"];
				if (circleRendererComponent)
				{
					auto& crc = deserializedEntity.AddComponent<CircleRendererComponent>();
					crc.Color = circleRendererComponent["Color"].as<glm::vec4>();
					crc.Thickness = circleRendererComponent["Thickness"].as<float>();
					crc.Fade = circleRendererComponent["Fade"].as<float>();
				}

				auto rigidBody2DComponent = entity["RigidBody2DComponent"];
				if (rigidBody2DComponent)
				{
					auto& rb = deserializedEntity.AddComponent<RigidBody2DComponent>();
					rb.Type = RigidBody2dBodyTypeFromString(rigidBody2DComponent["Body Type"].as<std::string>());
					rb.FixedRotation = rigidBody2DComponent["FixedRotation"].as<bool>();
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