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
#include "../Engine-Editor/src/Commands/Commands.h"
#include <mono/metadata/appdomain.h>

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

		const char* getSubstringAfterColon(const char* input) {
			const char* colonPosition = strchr(input, ':');

			if (colonPosition != nullptr) {
				return colonPosition + 2;
			}

			return nullptr;
		}

		const char* getSubstringAfterDot(const char* input) {
			const char* colonPosition = strchr(input, '.');

			if (colonPosition != nullptr) {
				return colonPosition + 1;
			}

			return nullptr;
		}

		template<typename... Component>
		bool IsInheritedInChildren(MonoType* managedType, Entity e)
		{
			bool isInherited = false;

			([&managedType, &e, &isInherited]()
				{
					if (std::strcmp(getSubstringAfterColon(typeid(Component).name()), getSubstringAfterDot(mono_type_get_name(managedType))) == 0)
						isInherited = e.GetInheritableComponent<Component>()->isInheritedInChildren;
				}(), ...);

			return isInherited;
		}

		template<typename... Component>
		bool IsInheritedInChildren(ComponentGroup<Component...>, MonoType* managedType, Entity e)
		{
			return IsInheritedInChildren<Component...>(managedType, e);
		}

		template<typename... Component>
		bool IsInheritedFromParent(MonoType* managedType, Entity e)
		{
			bool isInherited = false;

			([&managedType, &e, &isInherited]()
				{
					if (std::strcmp(getSubstringAfterColon(typeid(Component).name()), getSubstringAfterDot(mono_type_get_name(managedType))) == 0)
						isInherited = e.GetInheritableComponent<Component>()->isInherited;
				}(), ...);

			return isInherited;
		}

		template<typename... Component>
		bool IsInheritedFromParent(ComponentGroup<Component...>, MonoType* managedType, Entity e)
		{
			return IsInheritedFromParent<Component...>(managedType, e);
		}

		template<typename... Component>
		void CopyComponentToChildren(MonoType* managedType, Entity e, bool isUndo)
		{
			([&managedType, &e, &isUndo]()
				{
					if (std::strcmp(getSubstringAfterColon(typeid(Component).name()), getSubstringAfterDot(mono_type_get_name(managedType))) == 0)
						e.CopyComponentToChildren<Component>(isUndo);
				}(), ...);
		}

		template<typename... Component>
		void CopyComponentToChildren(ComponentGroup<Component...>, MonoType* managedType, Entity e, bool isUndo)
		{
			CopyComponentToChildren<Component...>(managedType, e, isUndo);
		}

		template<typename... Component>
		void CopyComponentValuesToChildren(MonoType* managedType, Entity e)
		{
			([&managedType, &e]()
				{
					if (std::strcmp(getSubstringAfterColon(typeid(Component).name()), getSubstringAfterDot(mono_type_get_name(managedType))) == 0)
						e.CopyComponentValuesToChildren<Component>();
				}(), ...);
		}

		template<typename... Component>
		void CopyComponentValuesToChildren(ComponentGroup<Component...>, MonoType* managedType, Entity e)
		{
			CopyComponentValuesToChildren<Component...>(managedType, e);
		}

		template<typename... Component>
		void InheritComponent(MonoType* managedType, Entity e, bool isUndo)
		{
			([&managedType, &e, &isUndo]()
				{
					if (std::strcmp(getSubstringAfterColon(typeid(Component).name()), getSubstringAfterDot(mono_type_get_name(managedType))) == 0)
						e.InheritComponentInChildren<Component>(false, isUndo);
				}(), ...);
		}

		template<typename... Component>
		void InheritComponent(ComponentGroup<Component...>, MonoType* managedType, Entity e, bool isUndo)
		{
			InheritComponent<Component...>(managedType, e, isUndo);
		}

		template<typename... Component>
		void AddComponent(MonoType* managedType, Entity e)
		{
			([&managedType, &e]()
				{
					if (std::strcmp(getSubstringAfterColon(typeid(Component).name()), getSubstringAfterDot(mono_type_get_name(managedType))) == 0)
						e.AddComponent<Component>();
				}(), ...);
		}

		template<typename... Component>
		void AddComponent(ComponentGroup<Component...>, MonoType* managedType, Entity e)
		{
			AddComponent<Component...>(managedType, e);
		}
		
		template<typename... Component>
		void RemoveComponent(MonoType* managedType, Entity e)
		{
			([&managedType, &e]()
				{
					if (std::strcmp(getSubstringAfterColon(typeid(Component).name()), getSubstringAfterDot(mono_type_get_name(managedType))) == 0)
						e.RemoveComponent<Component>();
				}(), ...);
		}

		template<typename... Component>
		void RemoveComponent(ComponentGroup<Component...>, MonoType* managedType, Entity e)
		{
			RemoveComponent<Component...>(managedType, e);
		}
	}

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFunctions;

#define EG_ADD_INTERNAL_CALL(Name) mono_add_internal_call("eg.InternalCalls::" #Name, Name)

	#pragma region Entity
	static MonoObject* Entity_GetScriptInstance(UUID uuid)
	{
		return ScriptEngine::GetManagedInstance(uuid);
	} 

	static bool Entity_IsInheritedInChildren(UUID uuid, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		EG_CORE_ASSERT(s_EntityHasComponentFunctions.find(managedType) != s_EntityHasComponentFunctions.end(), "Entity_IsInherited: Component type not registered!");
		return Utils::IsInheritedInChildren(AllComponents{}, managedType, e);
	}

	static bool Entity_IsInheritedFromParent(UUID uuid, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		EG_CORE_ASSERT(s_EntityHasComponentFunctions.find(managedType) != s_EntityHasComponentFunctions.end(), "Entity_IsInherited: Component type not registered!");
		return Utils::IsInheritedFromParent(AllComponents{}, managedType, e);
	}

	static void Entity_CopyComponentToChildren(UUID uuid, MonoReflectionType* componentType, bool isUndo)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		EG_CORE_ASSERT(s_EntityHasComponentFunctions.find(managedType) != s_EntityHasComponentFunctions.end(), "Entity_CopyComponentToChildren: Component type not registered!");
		Utils::CopyComponentToChildren(AllComponents{}, managedType, e, isUndo);
	}

	static void Entity_CopyComponentValuesToChildren(UUID uuid, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		EG_CORE_ASSERT(s_EntityHasComponentFunctions.find(managedType) != s_EntityHasComponentFunctions.end(), "Entity_CopyComponentValuesToChildren: Component type not registered!");
		Utils::CopyComponentValuesToChildren(AllComponents{}, managedType, e);
	}

	static void Entity_RemoveAnyChildren(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.RemoveAnyChildren();
	}

	static void Entity_RemoveChild(UUID uuid, UUID childUUID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity child = scene->GetEntityByUUID(childUUID);
		entity.RemoveChild(child);
	}

	static void Entity_AddChild(UUID uuid, UUID childUUID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity child = scene->GetEntityByUUID(childUUID);
		entity.AddChild(child);
	}

	static bool Entity_IsChildOfAny(UUID uuid, UUID childUUID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity child = scene->GetEntityByUUID(childUUID);
		return entity.IsChildOfAny(child);

	}

	static bool Entity_IsChild(UUID uuid, UUID childUUID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity child = scene->GetEntityByUUID(childUUID);
		return entity.IsChild(child);
	
	}

	static uint64_t Entity_GetParent(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		std::optional<Entity> parent = entity.GetParent();
		if (!parent.has_value())
			return 0;
		return parent.value().GetUUID();
	}

	static void Entity_SetParent(UUID uuid, UUID parentUUID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity parent = scene->GetEntityByUUID(parentUUID);
		entity.SetParent(parent);
	}

	static void Entity_InheritComponent(UUID uuid, MonoReflectionType* componentType, bool isUndo)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		EG_CORE_ASSERT(s_EntityHasComponentFunctions.find(managedType) != s_EntityHasComponentFunctions.end(), "Entity_RemoveComponent: Component type not registered!");
		if (!s_EntityHasComponentFunctions.at(managedType)(e))
		{
			EG_CORE_WARN("Entity does not have component of type {}", mono_type_get_name(managedType));
			return;
		}

		Utils::InheritComponent(AllComponents{}, managedType, e, isUndo);
	}

	static MonoString* Entity_GetAnyChildren(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		EG_CORE_ASSERT(entity, "Entity does not exist!");

		std::string children = "";

		for (auto& child : entity.GetAnyChildren())
		{
			children += std::to_string(child.GetUUID()) + ",";
		}

		return ScriptEngine::CreateString(children.c_str());
	}

	static MonoString* Entity_GetChildren(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		EG_CORE_ASSERT(entity, "Entity does not exist!");

		std::string children = "";

		for (auto& child : entity.GetChildren())
		{
			children += std::to_string(child.GetUUID()) + ",";
		}

		return ScriptEngine::CreateString(children.c_str());
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

	static void Entity_AddComponent(UUID uuid, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		EG_CORE_ASSERT(s_EntityHasComponentFunctions.find(managedType) != s_EntityHasComponentFunctions.end(), "Entity_AddComponent: Component type not registered!");
		if (s_EntityHasComponentFunctions.at(managedType)(e))
		{
			EG_CORE_WARN("Entity already has component of type {}", mono_type_get_name(managedType));
			return;
		}

		Utils::AddComponent(AllComponents{}, managedType, e);
	}

	static void Entity_RemoveComponent(UUID uuid, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		EG_CORE_ASSERT(s_EntityHasComponentFunctions.find(managedType) != s_EntityHasComponentFunctions.end(), "Entity_RemoveComponent: Component type not registered!");
		if (!s_EntityHasComponentFunctions.at(managedType)(e))
		{
			EG_CORE_WARN("Entity does not have component of type {}", mono_type_get_name(managedType));
			return;
		}

		Utils::RemoveComponent(AllComponents{}, managedType, e);
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

	static uint64_t Entity_Create(MonoString* name)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->CreateEntity(Utils::MonoStringToString(name));

		if(!e)
			return 0;

		return e.GetUUID();
	}

	static void Entity_Destroy(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		scene->DestroyEntity(e);
	}

	static MonoString* Entity_GetName(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		return ScriptEngine::CreateString(e.GetName().c_str());
	}

	static void Entity_SetName(UUID uuid, MonoString* name)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		e.SetName(Utils::MonoStringToString(name));
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

	static float SpriteRendererComponent_GetTilingFactor(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<SpriteRendererComponent>().TilingFactor;
	}

	static void SpriteRendererComponent_SetTilingFactor(UUID uuid, float tilingFactor)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<SpriteRendererComponent>().TilingFactor = tilingFactor;
	}

	static MonoString* SpriteRendererComponent_GetTexture(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		Ref<Texture2D> texture = entity.GetComponent<SpriteRendererComponent>().Texture;
		return ScriptEngine::CreateString(texture->GetPath().c_str());
	}

	static void SpriteRendererComponent_SetTexture(UUID uuid, MonoString* texturePath)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<SpriteRendererComponent>().Texture = Texture2D::Create(Utils::MonoStringToString(texturePath));
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

	static float CircleRendererComponent_GetThickness(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CircleRendererComponent>().Thickness;
	}

	static void CircleRendererComponent_SetThickness(UUID uuid, float thickness)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CircleRendererComponent>().Thickness = thickness;
	}

	static float CircleRendererComponent_GetFade(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CircleRendererComponent>().Fade;
	}

	static void CircleRendererComponent_SetFade(UUID uuid, float fade)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CircleRendererComponent>().Fade = fade;
	}
#pragma endregion

#pragma region Animator
	static void AnimatorComponent_PlayAnimation(UUID uuid, MonoString* animationName)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->Play();
	}

	static void AnimatorComponent_StopAnimation(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->Stop();
	}

	static void AnimatorComponent_UpdateAnimation(UUID uuid, float dt)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->Update(dt);
	}

	static void AnimatorComponent_ChangeAnimation(UUID uuid, MonoString* animationName)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->ChangeAnimation(Utils::MonoStringToString(animationName));
	}

	static void AnimatorComponent_PauseAnimation(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->Pause();
	}

	static void AnimatorComponent_SetSpeed(UUID uuid, float speed)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->SetSpeed(speed);
	}

	static float AnimatorComponent_GetSpeed(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<AnimatorComponent>().Animator2D->GetSpeed();
	}

	static const Ref<Animation> AnimatorComponent_GetCurrentAnimation(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<AnimatorComponent>().Animator2D->GetCurrentAnimation();
	}

	static void AnimatorComponent_AddAnimation(UUID uuid, MonoString* animationName)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->AddAnimationWithName(Utils::MonoStringToString(animationName));
	}

	static void AnimatorComponent_RemoveAnimation(UUID uuid, MonoString* animationName)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->RemoveAnimation(Utils::MonoStringToString(animationName));
	}

	static void AnimatorComponent_RemoveLastAnimation(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->RemoveLastAnimation();
	}

	static void AnimatorComponent_AddTransition(UUID uuid, MonoString* fromName, MonoString* toName)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->AddTransition(Utils::MonoStringToString(fromName), Utils::MonoStringToString(toName));
	}

	static void AnimatorComponent_AddTransitionByIndex(UUID uuid, int fromIndex, int toIndex)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->AddTransition(fromIndex, toIndex);
	}

	static void AnimatorComponent_RemoveTransition(UUID uuid, MonoString* fromName, MonoString* toName)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->RemoveTransition(Utils::MonoStringToString(fromName), Utils::MonoStringToString(toName));
	}

	static void AnimatorComponent_RemoveTransitionByIndex(UUID uuid, int fromIndex, int toIndex)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->RemoveTransition(fromIndex, toIndex);
	}

	static bool AnimatorComponent_CanTransition(UUID uuid, MonoString* fromName, MonoString* toName)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<AnimatorComponent>().Animator2D->CanTransition(Utils::MonoStringToString(fromName), Utils::MonoStringToString(toName));
	}

	static bool AnimatorComponent_CanTransitionByIndex(UUID uuid, int fromIndex, int toIndex)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<AnimatorComponent>().Animator2D->CanTransition(fromIndex, toIndex);
	}
#pragma endregion
	#pragma region Camera
	static bool CameraComponent_IsPrimary(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().Primary;
	}

	static void CameraComponent_SetPrimary(UUID uuid, bool primary)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Primary = primary;
	}

	static bool CameraComponent_IsFixedAspectRatio(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().FixedAspectRatio;
	}

	static void CameraComponent_SetFixedAspectRatio(UUID uuid, bool fixedAspectRatio)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().FixedAspectRatio = fixedAspectRatio;
	}

	#pragma region SceneCamera
	static void CameraComponent_SetOrtograpic(UUID uuid, float size, float nearClip, float farClip)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetOrthographic(size, nearClip, farClip);
	}

	static void CameraComponent_SetPerspective(UUID uuid, float verticalFOV, float nearClip, float farClip)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetPerspective(verticalFOV, nearClip, farClip);
	}

	static SceneCamera::ProjectionType CameraComponent_GetProjectionType(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().Camera.GetProjectionType();
	}

	static void CameraComponent_SetProjectionType(UUID uuid, SceneCamera::ProjectionType projectionType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetProjectionType(projectionType);
	} 

	static float CameraComponent_GetOrthographicSize(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().Camera.GetOrthographicSize();
	}

	static void CameraComponent_SetOrthographicSize(UUID uuid, float orthographicSize)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetOrthographicSize(orthographicSize);
	}

	static float CameraComponent_GetOrthographicNearClip(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().Camera.GetOrthographicNearClip();
	}

	static void CameraComponent_SetOrthographicNearClip(UUID uuid, float orthographicNearClip)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetOrthographicNearClip(orthographicNearClip);
	}

	static float CameraComponent_GetOrthographicFarClip(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().Camera.GetOrthographicFarClip();
	}

	static void CameraComponent_SetOrthographicFarClip(UUID uuid, float orthographicFarClip)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetOrthographicFarClip(orthographicFarClip);
	}

	static float CameraComponent_GetPerspectiveVerticalFOV(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().Camera.GetPerspectiveVerticalFOV();
	}

	static void CameraComponent_SetPerspectiveVerticalFOV(UUID uuid, float perspectiveVerticalFOV)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetPerspectiveVerticalFOV(perspectiveVerticalFOV);
	}

	static float CameraComponent_GetPerspectiveNearClip(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().Camera.GetPerspectiveNearClip();
	}

	static void CameraComponent_SetPerspectiveNearClip(UUID uuid, float perspectiveNearClip)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetPerspectiveNearClip(perspectiveNearClip);
	}

	static float CameraComponent_GetPerspectiveFarClip(UUID uuid)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().Camera.GetPerspectiveFarClip();
	}

	static void CameraComponent_SetPerspectiveFarClip(UUID uuid, float perspectiveFarClip)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetPerspectiveFarClip(perspectiveFarClip);
	}
	#pragma endregion 

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

		return entity.GetComponent<RigidBody2DComponent>().Type;
	}

	static void Rigidbody2DComponent_SetType(UUID entityID, RigidBody2DComponent::BodyType bodyType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);

		entity.GetComponent<RigidBody2DComponent>().Type = bodyType;
	}

	static bool Rigidbody2DComponent_IsFixedRotation(UUID entityID)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);

		return entity.GetComponent<RigidBody2DComponent>().FixedRotation;
	}

	static void Rigidbody2DComponent_SetFixedRotation(UUID entityID, bool fixedRotation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);

		entity.GetComponent<RigidBody2DComponent>().FixedRotation = fixedRotation;
	}
#pragma endregion

	#pragma region BoxCollider2D
	static void BoxCollider2DComponent_GetOffset(UUID uuid, glm::vec2* outOffset)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outOffset = entity.GetComponent<BoxCollider2DComponent>().Offset;
	}

	static void BoxCollider2DComponent_SetOffset(UUID uuid, glm::vec2* offset)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<BoxCollider2DComponent>().Offset = *offset;
	}

	static void BoxCollider2DComponent_GetSize(UUID uuid, glm::vec2* outSize)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outSize = entity.GetComponent<BoxCollider2DComponent>().Size;
	}

	static void BoxCollider2DComponent_SetSize(UUID uuid, glm::vec2* size)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<BoxCollider2DComponent>().Size = *size;
	}

	static float BoxCollider2DComponent_GetDensity(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<BoxCollider2DComponent>().Density;
	}

	static void BoxCollider2DComponent_SetDensity(UUID uuid, float density)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<BoxCollider2DComponent>().Density = density;
	}

	static float BoxCollider2DComponent_GetFriction(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<BoxCollider2DComponent>().Friction;
	}

	static void BoxCollider2DComponent_SetFriction(UUID uuid, float friction)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<BoxCollider2DComponent>().Friction = friction;
	}

	static float BoxCollider2DComponent_GetRestitution(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<BoxCollider2DComponent>().Restitution;
	}

	static void BoxCollider2DComponent_SetRestitution(UUID uuid, float restitution)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<BoxCollider2DComponent>().Restitution = restitution;
	}

	static float BoxCollider2DComponent_GetRestitutionThreshold(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<BoxCollider2DComponent>().RestitutionThreshold;
	}

	static void BoxCollider2DComponent_SetRestitutionThreshold(UUID uuid, float restitutionThreshold)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<BoxCollider2DComponent>().RestitutionThreshold = restitutionThreshold;
	}
	#pragma endregion

	#pragma region CircleCollider2D
	static void CircleCollider2DComponent_GetOffset(UUID uuid, glm::vec2* outOffset)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outOffset = entity.GetComponent<CircleCollider2DComponent>().Offset;
	}

	static void CircleCollider2DComponent_SetOffset(UUID uuid, glm::vec2* offset)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CircleCollider2DComponent>().Offset = *offset;
	}

	static float CircleCollider2DComponent_GetRadius(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CircleCollider2DComponent>().Radius;
	}

	static void CircleCollider2DComponent_SetRadius(UUID uuid, float radius)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CircleCollider2DComponent>().Radius = radius;
	}

	static float CircleCollider2DComponent_GetDensity(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CircleCollider2DComponent>().Density;
	}

	static void CircleCollider2DComponent_SetDensity(UUID uuid, float density)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CircleCollider2DComponent>().Density = density;
	}

	static float CircleCollider2DComponent_GetFriction(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CircleCollider2DComponent>().Friction;
	}

	static void CircleCollider2DComponent_SetFriction(UUID uuid, float friction)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CircleCollider2DComponent>().Friction = friction;
	}

	static float CircleCollider2DComponent_GetRestitution(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CircleCollider2DComponent>().Restitution;
	}

	static void CircleCollider2DComponent_SetRestitution(UUID uuid, float restitution)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CircleCollider2DComponent>().Restitution = restitution;
	}

	static float CircleCollider2DComponent_GetRestitutionThreshold(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CircleCollider2DComponent>().RestitutionThreshold;
	}

	static void CircleCollider2DComponent_SetRestitutionThreshold(UUID uuid, float restitutionThreshold)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CircleCollider2DComponent>().RestitutionThreshold = restitutionThreshold;
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
		EG_ADD_INTERNAL_CALL(Entity_Create);
		EG_ADD_INTERNAL_CALL(Entity_Destroy);
		EG_ADD_INTERNAL_CALL(Entity_AddComponent);
		EG_ADD_INTERNAL_CALL(Entity_RemoveComponent);
		EG_ADD_INTERNAL_CALL(Entity_GetName);
		EG_ADD_INTERNAL_CALL(Entity_SetName);
		EG_ADD_INTERNAL_CALL(Entity_GetChildren);
		EG_ADD_INTERNAL_CALL(Entity_GetAnyChildren);
		EG_ADD_INTERNAL_CALL(Entity_InheritComponent);
		EG_ADD_INTERNAL_CALL(Entity_CopyComponentToChildren);
		EG_ADD_INTERNAL_CALL(Entity_CopyComponentValuesToChildren);
		EG_ADD_INTERNAL_CALL(Entity_RemoveAnyChildren);
		EG_ADD_INTERNAL_CALL(Entity_RemoveChild);
		EG_ADD_INTERNAL_CALL(Entity_AddChild);
		EG_ADD_INTERNAL_CALL(Entity_IsChildOfAny);
		EG_ADD_INTERNAL_CALL(Entity_IsChild);
		EG_ADD_INTERNAL_CALL(Entity_GetParent);
		EG_ADD_INTERNAL_CALL(Entity_SetParent);
		EG_ADD_INTERNAL_CALL(Entity_IsInheritedFromParent);
		EG_ADD_INTERNAL_CALL(Entity_IsInheritedInChildren);

		EG_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		EG_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);
		EG_ADD_INTERNAL_CALL(TransformComponent_GetRotation);
		EG_ADD_INTERNAL_CALL(TransformComponent_SetRotation);
		EG_ADD_INTERNAL_CALL(TransformComponent_GetScale);
		EG_ADD_INTERNAL_CALL(TransformComponent_SetScale);

		EG_ADD_INTERNAL_CALL(SpriteRendererComponent_GetColor);
		EG_ADD_INTERNAL_CALL(SpriteRendererComponent_SetColor);
		EG_ADD_INTERNAL_CALL(SpriteRendererComponent_GetTilingFactor);
		EG_ADD_INTERNAL_CALL(SpriteRendererComponent_SetTilingFactor);
		EG_ADD_INTERNAL_CALL(SpriteRendererComponent_GetTexture);
		EG_ADD_INTERNAL_CALL(SpriteRendererComponent_SetTexture);

		EG_ADD_INTERNAL_CALL(CircleRendererComponent_GetColor);
		EG_ADD_INTERNAL_CALL(CircleRendererComponent_SetColor);
		EG_ADD_INTERNAL_CALL(CircleRendererComponent_GetThickness);
		EG_ADD_INTERNAL_CALL(CircleRendererComponent_SetThickness);
		EG_ADD_INTERNAL_CALL(CircleRendererComponent_GetFade);
		EG_ADD_INTERNAL_CALL(CircleRendererComponent_SetFade);

		EG_ADD_INTERNAL_CALL(AnimatorComponent_PlayAnimation);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_StopAnimation);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_UpdateAnimation);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_ChangeAnimation);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_PauseAnimation);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_StopAnimation);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_SetSpeed);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_GetSpeed);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_GetCurrentAnimation);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_AddAnimation);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_RemoveAnimation);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_RemoveLastAnimation);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_AddTransition);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_AddTransitionByIndex);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_RemoveTransition);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_RemoveTransitionByIndex);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_CanTransition);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_CanTransitionByIndex);

		EG_ADD_INTERNAL_CALL(CameraComponent_IsPrimary);
		EG_ADD_INTERNAL_CALL(CameraComponent_SetPrimary);
		EG_ADD_INTERNAL_CALL(CameraComponent_IsFixedAspectRatio);
		EG_ADD_INTERNAL_CALL(CameraComponent_SetFixedAspectRatio);
		EG_ADD_INTERNAL_CALL(CameraComponent_SetOrtograpic);
		EG_ADD_INTERNAL_CALL(CameraComponent_SetPerspective);
		EG_ADD_INTERNAL_CALL(CameraComponent_GetProjectionType);
		EG_ADD_INTERNAL_CALL(CameraComponent_SetProjectionType);
		EG_ADD_INTERNAL_CALL(CameraComponent_GetOrthographicSize);
		EG_ADD_INTERNAL_CALL(CameraComponent_SetOrthographicSize);
		EG_ADD_INTERNAL_CALL(CameraComponent_GetOrthographicNearClip);
		EG_ADD_INTERNAL_CALL(CameraComponent_SetOrthographicNearClip);
		EG_ADD_INTERNAL_CALL(CameraComponent_GetOrthographicFarClip);
		EG_ADD_INTERNAL_CALL(CameraComponent_SetOrthographicFarClip);
		EG_ADD_INTERNAL_CALL(CameraComponent_GetPerspectiveVerticalFOV);
		EG_ADD_INTERNAL_CALL(CameraComponent_SetPerspectiveVerticalFOV);
		EG_ADD_INTERNAL_CALL(CameraComponent_GetPerspectiveNearClip);
		EG_ADD_INTERNAL_CALL(CameraComponent_SetPerspectiveNearClip);
		EG_ADD_INTERNAL_CALL(CameraComponent_GetPerspectiveFarClip);
		EG_ADD_INTERNAL_CALL(CameraComponent_SetPerspectiveFarClip);

		EG_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulse);
		EG_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulseToCenter);
		EG_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetLinearVelocity);
		EG_ADD_INTERNAL_CALL(Rigidbody2DComponent_GetType);
		EG_ADD_INTERNAL_CALL(Rigidbody2DComponent_SetType);
		EG_ADD_INTERNAL_CALL(Rigidbody2DComponent_IsFixedRotation);
		EG_ADD_INTERNAL_CALL(Rigidbody2DComponent_SetFixedRotation);

		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetOffset);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetOffset);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetSize);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetSize);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetDensity);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetDensity);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetFriction);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetFriction);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetRestitution);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetRestitution);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_GetRestitutionThreshold);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_SetRestitutionThreshold);

		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetOffset);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetOffset);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetRadius);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetRadius);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetDensity);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetDensity);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetFriction);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetFriction);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetRestitution);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetRestitution);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_GetRestitutionThreshold);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_SetRestitutionThreshold);

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
				s_EntityHasComponentFunctions[managedType] = [](Entity entity) { return entity.HasComponent<Component>();};
			}(), ...);
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