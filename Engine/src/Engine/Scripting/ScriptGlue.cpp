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

	static std::unordered_map<MonoType *, std::function<bool(Entity)>> s_EntityHasComponentFunctions;

#define EG_ADD_INTERNAL_CALL(Name) mono_add_internal_call("eg.InternalCalls::" #Name, Name)

	static MonoObject *Entity_GetScriptInstance(UUID uuid)
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

	static void RigidBody2DComponnet_ApplyLinearImpulse(UUID uuid, glm::vec2 *impulse, glm::vec2 *point, bool wake)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		EG_CORE_ASSERT(entity, "Entity does not exist!");

		auto &rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body *body = (b2Body *)rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void RigidBody2DComponnet_ApplyLinearImpulseToCenter(UUID uuid, glm::vec2 *impulse, bool wake)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		EG_CORE_ASSERT(entity, "Entity does not exist!");

		auto &rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body *body = (b2Body *)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	static void Rigidbody2DComponent_GetLinearVelocity(UUID entityID, glm::vec2 *outLinearVelocity)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);

		auto &rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body *body = (b2Body *)rb2d.RuntimeBody;
		const b2Vec2 &linearVelocity = body->GetLinearVelocity();
		*outLinearVelocity = glm::vec2(linearVelocity.x, linearVelocity.y);
	}

	static RigidBody2DComponent::BodyType Rigidbody2DComponent_GetType(UUID entityID)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);

		auto &rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body *body = (b2Body *)rb2d.RuntimeBody;
		return Utils::RigidBody2DTypeFromBox2DBody(body->GetType());
	}

	static void Rigidbody2DComponent_SetType(UUID entityID, RigidBody2DComponent::BodyType bodyType)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);

		auto &rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body *body = (b2Body *)rb2d.RuntimeBody;
		body->SetType(Utils::RigidBody2DTypeToBox2DBody(bodyType));
	}

	void ScriptGlue::RegisterFunctions()
	{
		EG_ADD_INTERNAL_CALL(Entity_HasComponent);
		EG_ADD_INTERNAL_CALL(Entity_FindEntityByName);

		EG_ADD_INTERNAL_CALL(Entity_GetScriptInstance);
		EG_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		EG_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		EG_ADD_INTERNAL_CALL(RigidBody2DComponnet_ApplyLinearImpulse);
		EG_ADD_INTERNAL_CALL(RigidBody2DComponnet_ApplyLinearImpulseToCenter);

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