#include "egpch.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/Input.h"
#include "Engine/Physics/Physics2D.h"

#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

#include "box2d/b2_body.h"

namespace eg
{

	namespace Utils {

		std::string MonoStringToString(MonoString* string)
		{
			char* cStr = mono_string_to_utf8(string);
			std::string str(cStr);
			mono_free(cStr);
			return str;
		}

	}

	static std::unordered_map<MonoType *, std::function<bool(Entity)>> s_EntityHasComponentFunctions;

#define EG_ADD_INTERNAL_CALL(Name) mono_add_internal_call("eg.InternalCalls::" #Name, Name)

	#pragma region Entity
	static MonoObject* Entity_GetScriptInstance(UUID uuid)
	{
		return ScriptEngine::GetManagedInstance(uuid);
	}

	static bool Entity_HasComponent(UUID uuid, MonoReflectionType *componentType)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		EG_CORE_ASSERT(entity, "Entity does not exist!");

		MonoType *managedType = mono_reflection_type_get_type(componentType);
		EG_CORE_ASSERT(s_EntityHasComponentFunctions.find(managedType) != s_EntityHasComponentFunctions.end(), "Entity_HasComponent: Component type not registered!");
		return s_EntityHasComponentFunctions.at(managedType)(entity);
	}

	static uint64_t Entity_FindEntityByName(MonoString *name)
	{
		char *cStr = mono_string_to_utf8(name);

		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->FindEntityByName(cStr);
		mono_free(cStr);

		if (!e)
			return 0;

		return e.GetUUID();
	}

	#pragma endregion

	#pragma region Transform
	static void TransformComponent_GetTranslation(UUID uuid, glm::vec3 *outTranslation)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID uuid, glm::vec3 *translation)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

	static void TransformComponent_GetRotation(UUID uuid, glm::vec3* outRotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outRotation = entity.GetComponent<TransformComponent>().Rotation;
	}

	static void TransformComponent_SetRotation(UUID uuid, glm::vec3* rotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<TransformComponent>().Rotation = *rotation;
	}

	static void TransformComponent_GetScale(UUID uuid, glm::vec3* outScale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outScale = entity.GetComponent<TransformComponent>().Scale;
	}

	static void TransformComponent_SetScale(UUID uuid, glm::vec3* scale)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<TransformComponent>().Scale = *scale;
	}

	#pragma endregion 

	#pragma region SpriteRenderer
	static void SpriteRendererComponent_GetColor(UUID uuid, glm::vec4* outColor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outColor = entity.GetComponent<SpriteRendererComponent>().Color;
	}

	static void SpriteRendererComponent_SetColor(UUID uuid, glm::vec4* color)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<SpriteRendererComponent>().Color = *color;
	}

	static void SpriteRendererComponent_GetTilingFactor(UUID uuid, float outTilingFactor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		outTilingFactor = entity.GetComponent<SpriteRendererComponent>().TilingFactor;
	}

	static void SpriteRendererComponent_SetColor(UUID uuid, float tilingFactor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<SpriteRendererComponent>().TilingFactor = tilingFactor;
	}
	#pragma endregion

	#pragma region CircleRenderer
	static void CircleRendererComponent_GetColor(UUID uuid, glm::vec4* outColor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outColor = entity.GetComponent<CircleRendererComponent>().Color;
	}

	static void CircleRendererComponent_SetColor(UUID uuid, glm::vec4* color)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CircleRendererComponent>().Color = *color;
	}

	static void CircleRendererComponent_GetThickness(UUID uuid, float outThickness)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		outThickness = entity.GetComponent<CircleRendererComponent>().Thickness;
	}

	static void CircleRendererComponent_SetThickness(UUID uuid, float thickness)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CircleRendererComponent>().Thickness = thickness;
	}

	static void CircleRendererComponent_GetThickness(UUID uuid, float outFade)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		outFade = entity.GetComponent<CircleRendererComponent>().Fade;
	}

	static void CircleRendererComponent_SetThickness(UUID uuid, float fade)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CircleRendererComponent>().Fade = fade;
	}
#pragma endregion

	#pragma region Camera
	static void CameraComponent_IsPrimary(UUID uuid, bool outPrimary)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		outPrimary = entity.GetComponent<CameraComponent>().Primary;
	}

	static void CameraComponent_SetPrimary(UUID uuid, bool primary)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Primary = primary;
	}

	static void CameraComponent_IsFixedAspectRatio(UUID uuid, bool outFixedAspectRatio)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		outFixedAspectRatio = entity.GetComponent<CameraComponent>().FixedAspectRatio;
	}

	static void CameraComponent_SetFixedAspectRatio(UUID uuid, bool fixedAspectRatio)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().FixedAspectRatio = fixedAspectRatio;
	}
	#pragma endregion

	#pragma region RigidBody2D
	static void RigidBody2DComponent_ApplyLinearImpulse(UUID uuid, glm::vec2 *impulse, glm::vec2 *point, bool wake)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		EG_CORE_ASSERT(entity, "Entity does not exist!");

		auto &rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body *body = (b2Body *)rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void RigidBody2DComponent_ApplyLinearImpulseToCenter(UUID uuid, glm::vec2 *impulse, bool wake)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		EG_CORE_ASSERT(entity, "Entity does not exist!");

		auto &rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body *body = (b2Body *)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	static void Rigidbody2DComponent_GetLinearVelocity(UUID entityID, glm::vec2* outLinearVelocity)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		const b2Vec2& linearVelocity = body->GetLinearVelocity();
		*outLinearVelocity = glm::vec2(linearVelocity.x, linearVelocity.y);
	}

	static RigidBody2DComponent::BodyType Rigidbody2DComponent_GetType(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		return Utils::RigidBody2DTypeFromBox2DBody(body->GetType());
	}

	static void Rigidbody2DComponent_SetType(UUID entityID, RigidBody2DComponent::BodyType bodyType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->SetType(Utils::RigidBody2DTypeToBox2DBody(bodyType));
	}
#pragma endregion

	#pragma region Text
		static MonoString* TextComponent_GetText(UUID entityID)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			EG_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			EG_CORE_ASSERT(entity);
			EG_CORE_ASSERT(entity.HasComponent<TextComponent>());

			auto& tc = entity.GetComponent<TextComponent>();
			return ScriptEngine::CreateString(tc.TextString.c_str());
		}

		static void TextComponent_SetText(UUID entityID, MonoString* textString)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			EG_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			EG_CORE_ASSERT(entity);
			EG_CORE_ASSERT(entity.HasComponent<TextComponent>());

			auto& tc = entity.GetComponent<TextComponent>();
			tc.TextString = Utils::MonoStringToString(textString);
		}

		static void TextComponent_GetColor(UUID entityID, glm::vec4* color)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			EG_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			EG_CORE_ASSERT(entity);
			EG_CORE_ASSERT(entity.HasComponent<TextComponent>());

			auto& tc = entity.GetComponent<TextComponent>();
			*color = tc.Color;
		}

		static void TextComponent_SetColor(UUID entityID, glm::vec4* color)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			EG_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			EG_CORE_ASSERT(entity);
			EG_CORE_ASSERT(entity.HasComponent<TextComponent>());

			auto& tc = entity.GetComponent<TextComponent>();
			tc.Color = *color;
		}

		static float TextComponent_GetKerning(UUID entityID)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			EG_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			EG_CORE_ASSERT(entity);
			EG_CORE_ASSERT(entity.HasComponent<TextComponent>());

			auto& tc = entity.GetComponent<TextComponent>();
			return tc.Kerning;
		}

		static void TextComponent_SetKerning(UUID entityID, float kerning)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			EG_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			EG_CORE_ASSERT(entity);
			EG_CORE_ASSERT(entity.HasComponent<TextComponent>());

			auto& tc = entity.GetComponent<TextComponent>();
			tc.Kerning = kerning;
		}

		static float TextComponent_GetLineSpacing(UUID entityID)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			EG_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			EG_CORE_ASSERT(entity);
			EG_CORE_ASSERT(entity.HasComponent<TextComponent>());

			auto& tc = entity.GetComponent<TextComponent>();
			return tc.LineSpacing;
		}

		static void TextComponent_SetLineSpacing(UUID entityID, float lineSpacing)
		{
			Scene* scene = ScriptEngine::GetSceneContext();
			EG_CORE_ASSERT(scene);
			Entity entity = scene->GetEntityByUUID(entityID);
			EG_CORE_ASSERT(entity);
			EG_CORE_ASSERT(entity.HasComponent<TextComponent>());

			auto& tc = entity.GetComponent<TextComponent>();
			tc.LineSpacing = lineSpacing;
		}
	#pragma endregion

	#pragma region Input
	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}
#pragma endregion

	void ScriptGlue::RegisterFunctions()
	{
		EG_ADD_INTERNAL_CALL(Entity_HasComponent);
		EG_ADD_INTERNAL_CALL(Entity_FindEntityByName);

		EG_ADD_INTERNAL_CALL(Entity_GetScriptInstance);
		EG_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		EG_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		EG_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulse);
		EG_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulseToCenter);

		EG_ADD_INTERNAL_CALL(TextComponent_GetText);
		EG_ADD_INTERNAL_CALL(TextComponent_SetText);
		EG_ADD_INTERNAL_CALL(TextComponent_GetColor);
		EG_ADD_INTERNAL_CALL(TextComponent_SetColor);
		EG_ADD_INTERNAL_CALL(TextComponent_GetKerning);
		EG_ADD_INTERNAL_CALL(TextComponent_SetKerning);
		EG_ADD_INTERNAL_CALL(TextComponent_GetLineSpacing);
		EG_ADD_INTERNAL_CALL(TextComponent_SetLineSpacing);

		EG_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}

	template <typename... Component>
	static void RegisterComponent()
	{
		([]()
		 {
				std::string_view typeName = typeid(Component).name();
				size_t pos = typeName.find_last_of(":");
				std::string_view structName = typeName.substr(pos + 1);
				std::string managedTypeName = fmt::format("eg.{}", structName);

				MonoType* managedType = mono_reflection_type_from_name(managedTypeName.data(), ScriptEngine::GetCoreAssemblyImage());
				if (!managedType)
				{
					EG_CORE_ERROR("Could not find component type {}", managedTypeName);
					return;
				}
				s_EntityHasComponentFunctions[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); }; }(),
		 ...);
	}

	template <typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		s_EntityHasComponentFunctions.clear();
		RegisterComponent<Component...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent(AllComponents{});
	}
}