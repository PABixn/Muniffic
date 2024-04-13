#include "egpch.h"

#include "ScriptGlue.h"
#include "ScriptEngine.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/Input.h"
#include "Engine/Physics/Physics2D.h"

#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

#include "../Engine-Editor/src/Commands/Commands.h"
#include <mono/metadata/appdomain.h>

#include "../Engine-Editor/src/Panels/ConsolePanel.h"

namespace eg
{

	namespace Utils
	{

		std::string MonoStringToString(MonoString *string)
		{
			char *cStr = mono_string_to_utf8(string);
			std::string str(cStr);
			mono_free(cStr);
			return str;
		}

		const char *getSubstringAfterColon(const char *input)
		{
			const char *colonPosition = strchr(input, ':');

			if (colonPosition != nullptr)
			{
				return colonPosition + 2;
			}

			return nullptr;
		}

		const char *getSubstringAfterDot(const char *input)
		{
			const char *colonPosition = strchr(input, '.');

			if (colonPosition != nullptr)
			{
				return colonPosition + 1;
			}

			return nullptr;
		}

		template <typename... Component>
		bool IsInheritedInChildren(MonoType *managedType, Entity e)
		{
			bool isInherited = false;

			([&managedType, &e, &isInherited]()
			 {
					if (std::strcmp(getSubstringAfterColon(typeid(Component).name()), getSubstringAfterDot(mono_type_get_name(managedType))) == 0)
						isInherited = e.GetInheritableComponent<Component>()->isInheritedInChildren; }(),
			 ...);

			return isInherited;
		}

		template <typename... Component>
		bool IsInheritedInChildren(ComponentGroup<Component...>, MonoType *managedType, Entity e)
		{
			return IsInheritedInChildren<Component...>(managedType, e);
		}

		template <typename... Component>
		bool IsInheritedFromParent(MonoType *managedType, Entity e)
		{
			bool isInherited = false;

			([&managedType, &e, &isInherited]()
			 {
					if (std::strcmp(getSubstringAfterColon(typeid(Component).name()), getSubstringAfterDot(mono_type_get_name(managedType))) == 0)
						isInherited = e.GetInheritableComponent<Component>()->isInherited; }(),
			 ...);

			return isInherited;
		}

		template <typename... Component>
		bool IsInheritedFromParent(ComponentGroup<Component...>, MonoType *managedType, Entity e)
		{
			return IsInheritedFromParent<Component...>(managedType, e);
		}

		template <typename... Component>
		void CopyComponentToChildren(MonoType *managedType, Entity e, bool isUndo)
		{
			([&managedType, &e, &isUndo]()
			 {
					if (std::strcmp(getSubstringAfterColon(typeid(Component).name()), getSubstringAfterDot(mono_type_get_name(managedType))) == 0)
						e.CopyComponentToChildren<Component>(isUndo); }(),
			 ...);
		}

		template <typename... Component>
		void CopyComponentToChildren(ComponentGroup<Component...>, MonoType *managedType, Entity e, bool isUndo)
		{
			CopyComponentToChildren<Component...>(managedType, e, isUndo);
		}

		template <typename... Component>
		void CopyComponentValuesToChildren(MonoType *managedType, Entity e)
		{
			([&managedType, &e]()
			 {
					if (std::strcmp(getSubstringAfterColon(typeid(Component).name()), getSubstringAfterDot(mono_type_get_name(managedType))) == 0)
						e.CopyComponentValuesToChildren<Component>(); }(),
			 ...);
		}

		template <typename... Component>
		void CopyComponentValuesToChildren(ComponentGroup<Component...>, MonoType *managedType, Entity e)
		{
			CopyComponentValuesToChildren<Component...>(managedType, e);
		}

		template <typename... Component>
		void InheritComponent(MonoType *managedType, Entity e, bool isUndo)
		{
			([&managedType, &e, &isUndo]()
			 {
					if (std::strcmp(getSubstringAfterColon(typeid(Component).name()), getSubstringAfterDot(mono_type_get_name(managedType))) == 0)
						e.InheritComponentInChildren<Component>(false, isUndo); }(),
			 ...);
		}

		template <typename... Component>
		void InheritComponent(ComponentGroup<Component...>, MonoType *managedType, Entity e, bool isUndo)
		{
			InheritComponent<Component...>(managedType, e, isUndo);
		}

		template <typename... Component>
		void AddComponent(MonoType *managedType, Entity e)
		{
			([&managedType, &e]()
			 {
					if (std::strcmp(getSubstringAfterColon(typeid(Component).name()), getSubstringAfterDot(mono_type_get_name(managedType))) == 0)
						e.AddComponent<Component>(); }(),
			 ...);
		}

		template <typename... Component>
		void AddComponent(ComponentGroup<Component...>, MonoType *managedType, Entity e)
		{
			AddComponent<Component...>(managedType, e);
		}

		template <typename... Component>
		void RemoveComponent(MonoType *managedType, Entity e)
		{
			([&managedType, &e]()
			 {
					if (std::strcmp(getSubstringAfterColon(typeid(Component).name()), getSubstringAfterDot(mono_type_get_name(managedType))) == 0)
						e.RemoveComponent<Component>(); }(),
			 ...);
		}

		template <typename... Component>
		void RemoveComponent(ComponentGroup<Component...>, MonoType *managedType, Entity e)
		{
			RemoveComponent<Component...>(managedType, e);
		}
	}

	static std::unordered_map<MonoType *, std::function<bool(Entity)>> s_EntityHasComponentFunctions;

	enum class Side {
		LEFT = 0, RIGHT, TOP, BOTTOM
	};

#define EG_ADD_INTERNAL_CALL(Name) mono_add_internal_call("eg.InternalCalls::" #Name, Name)
#pragma region Console
	static void Console_Log(MonoString* message, ConsolePanel::LogType logType)
	{
		ConsolePanel::Log(Utils::MonoStringToString(message), logType);
	}
#pragma endregion
#pragma region Entity
	static MonoObject *Entity_GetScriptInstance(UUID uuid)
	{
		return ScriptEngine::GetManagedInstance(uuid);
	}

	static bool Entity_IsInheritedInChildren(UUID uuid, MonoReflectionType *componentType)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		MonoType *managedType = mono_reflection_type_get_type(componentType);
		EG_CORE_ASSERT(s_EntityHasComponentFunctions.find(managedType) != s_EntityHasComponentFunctions.end(), "Entity_IsInherited: Component type not registered!");
		return Utils::IsInheritedInChildren(AllComponents{}, managedType, e);
	}

	static bool Entity_IsInheritedFromParent(UUID uuid, MonoReflectionType *componentType)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		MonoType *managedType = mono_reflection_type_get_type(componentType);
		EG_CORE_ASSERT(s_EntityHasComponentFunctions.find(managedType) != s_EntityHasComponentFunctions.end(), "Entity_IsInherited: Component type not registered!");
		return Utils::IsInheritedFromParent(AllComponents{}, managedType, e);
	}

	static void Entity_CopyComponentToChildren(UUID uuid, MonoReflectionType *componentType, bool isUndo)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		MonoType *managedType = mono_reflection_type_get_type(componentType);
		EG_CORE_ASSERT(s_EntityHasComponentFunctions.find(managedType) != s_EntityHasComponentFunctions.end(), "Entity_CopyComponentToChildren: Component type not registered!");
		Utils::CopyComponentToChildren(AllComponents{}, managedType, e, isUndo);
	}

	static void Entity_CopyComponentValuesToChildren(UUID uuid, MonoReflectionType *componentType)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		MonoType *managedType = mono_reflection_type_get_type(componentType);
		EG_CORE_ASSERT(s_EntityHasComponentFunctions.find(managedType) != s_EntityHasComponentFunctions.end(), "Entity_CopyComponentValuesToChildren: Component type not registered!");
		Utils::CopyComponentValuesToChildren(AllComponents{}, managedType, e);
	}

	static void Entity_RemoveAnyChildren(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.RemoveAnyChildren();
	}

	static void Entity_RemoveChild(UUID uuid, UUID childUUID)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity child = scene->GetEntityByUUID(childUUID);
		entity.RemoveChild(child);
	}

	static void Entity_AddChild(UUID uuid, UUID childUUID)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity child = scene->GetEntityByUUID(childUUID);
		entity.AddChild(child);
	}

	static bool Entity_IsChildOfAny(UUID uuid, UUID childUUID)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity child = scene->GetEntityByUUID(childUUID);
		return entity.IsChildOfAny(child);
	}

	static bool Entity_IsChild(UUID uuid, UUID childUUID)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity child = scene->GetEntityByUUID(childUUID);
		return entity.IsChild(child);
	}

	static uint64_t Entity_GetParent(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		std::optional<Entity> parent = entity.GetParent();
		if (!parent.has_value())
			return 0;
		return parent.value().GetUUID();
	}

	static void Entity_SetParent(UUID uuid, UUID parentUUID)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity parent = scene->GetEntityByUUID(parentUUID);
		entity.SetParent(parent);
	}

	static void Entity_InheritComponent(UUID uuid, MonoReflectionType *componentType, bool isUndo)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		MonoType *managedType = mono_reflection_type_get_type(componentType);
		EG_CORE_ASSERT(s_EntityHasComponentFunctions.find(managedType) != s_EntityHasComponentFunctions.end(), "Entity_RemoveComponent: Component type not registered!");
		if (!s_EntityHasComponentFunctions.at(managedType)(e))
		{
			EG_CORE_WARN("Entity does not have component of type {}", mono_type_get_name(managedType));
			return;
		}

		Utils::InheritComponent(AllComponents{}, managedType, e, isUndo);
	}

	static MonoString *Entity_GetAnyChildren(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		EG_CORE_ASSERT(entity, "Entity does not exist!");

		std::string children = "";

		for (auto &child : entity.GetAnyChildren())
		{
			children += std::to_string(child.GetUUID()) + ",";
		}

		return ScriptEngine::CreateString(children.c_str());
	}

	static MonoString *Entity_GetChildren(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		EG_CORE_ASSERT(entity, "Entity does not exist!");

		std::string children = "";

		for (auto &child : entity.GetChildren())
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

	static void Entity_AddComponent(UUID uuid, MonoReflectionType *componentType)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		MonoType *managedType = mono_reflection_type_get_type(componentType);
		EG_CORE_ASSERT(s_EntityHasComponentFunctions.find(managedType) != s_EntityHasComponentFunctions.end(), "Entity_AddComponent: Component type not registered!");
		if (s_EntityHasComponentFunctions.at(managedType)(e))
		{
			EG_CORE_WARN("Entity already has component of type {}", mono_type_get_name(managedType));
			return;
		}

		Utils::AddComponent(AllComponents{}, managedType, e);
	}

	static void Entity_RemoveComponent(UUID uuid, MonoReflectionType *componentType)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		MonoType *managedType = mono_reflection_type_get_type(componentType);
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

	static bool Entity_Exists(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		return scene->EntityExists(uuid);
	}

	static uint64_t Entity_Create(MonoString *name)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->CreateEntity(Utils::MonoStringToString(name));

		if (!e)
			return 0;

		return e.GetUUID();
	}

	static void Entity_Destroy(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		scene->DestroyEntity(e);
	}

	static MonoString *Entity_GetName(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity e = scene->GetEntityByUUID(uuid);
		return ScriptEngine::CreateString(e.GetName().c_str());
	}

	static void Entity_SetName(UUID uuid, MonoString *name)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
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

	static void TransformComponent_GetRotation(UUID uuid, glm::vec3 *outRotation)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outRotation = entity.GetComponent<TransformComponent>().Rotation;
	}

	static void TransformComponent_SetRotation(UUID uuid, glm::vec3 *rotation)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<TransformComponent>().Rotation = *rotation;
	}

	static void TransformComponent_GetScale(UUID uuid, glm::vec3 *outScale)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outScale = entity.GetComponent<TransformComponent>().Scale;
	}

	static void TransformComponent_SetScale(UUID uuid, glm::vec3 *scale)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<TransformComponent>().Scale = *scale;
	}

#pragma endregion

#pragma region SpriteRenderer
	static void SpriteRendererComponent_GetColor(UUID uuid, glm::vec4 *outColor)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outColor = entity.GetComponent<SpriteRendererComponent>().Color;
	}

	static void SpriteRendererComponent_SetColor(UUID uuid, glm::vec4 *color)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<SpriteRendererComponent>().Color = *color;
	}

	static float SpriteRendererComponent_GetTilingFactor(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<SpriteRendererComponent>().TilingFactor;
	}

	static void SpriteRendererComponent_SetTilingFactor(UUID uuid, float tilingFactor)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<SpriteRendererComponent>().TilingFactor = tilingFactor;
	}

	static MonoString *SpriteRendererComponent_GetTexture(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		Ref<Texture2D> texture = entity.GetComponent<SpriteRendererComponent>().Texture;
		return ScriptEngine::CreateString(texture->GetPath().c_str());
	}

	static void SpriteRendererComponent_SetTexture(UUID uuid, MonoString *texturePath)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<SpriteRendererComponent>().Texture = Texture2D::Create(Utils::MonoStringToString(texturePath));
	}
#pragma endregion

#pragma region CircleRenderer
	static void CircleRendererComponent_GetColor(UUID uuid, glm::vec4 *outColor)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outColor = entity.GetComponent<CircleRendererComponent>().Color;
	}

	static void CircleRendererComponent_SetColor(UUID uuid, glm::vec4 *color)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CircleRendererComponent>().Color = *color;
	}

	static float CircleRendererComponent_GetThickness(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CircleRendererComponent>().Thickness;
	}

	static void CircleRendererComponent_SetThickness(UUID uuid, float thickness)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CircleRendererComponent>().Thickness = thickness;
	}

	static float CircleRendererComponent_GetFade(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CircleRendererComponent>().Fade;
	}

	static void CircleRendererComponent_SetFade(UUID uuid, float fade)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CircleRendererComponent>().Fade = fade;
	}
#pragma endregion

#pragma region Animator
	static void AnimatorComponent_PlayAnimation(UUID uuid, MonoString *animationName)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->Play();
	}

	static void AnimatorComponent_StopAnimation(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->Stop();
	}

	static void AnimatorComponent_UpdateAnimation(UUID uuid, float dt)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->Update(dt);
	}

	static void AnimatorComponent_ChangeAnimation(UUID uuid, MonoString *animationName)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->ChangeAnimation(Utils::MonoStringToString(animationName));
	}

	static void AnimatorComponent_PauseAnimation(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->Pause();
	}

	static void AnimatorComponent_SetSpeed(UUID uuid, float speed)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->SetSpeed(speed);
	}

	static float AnimatorComponent_GetSpeed(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<AnimatorComponent>().Animator2D->GetSpeed();
	}

	static const Ref<Animation> AnimatorComponent_GetCurrentAnimation(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<AnimatorComponent>().Animator2D->GetCurrentAnimation();
	}

	static void AnimatorComponent_AddAnimation(UUID uuid, MonoString *animationName)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->AddAnimationWithName(Utils::MonoStringToString(animationName));
	}

	static void AnimatorComponent_RemoveAnimation(UUID uuid, MonoString *animationName)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->RemoveAnimation(Utils::MonoStringToString(animationName));
	}

	static void AnimatorComponent_RemoveLastAnimation(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->RemoveLastAnimation();
	}

	static void AnimatorComponent_TransitionByIndex(UUID uuid, int toIndex)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->Transition(toIndex);
	}

	static void AnimatorComponent_Transition(UUID uuid, MonoString *toName)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->Transition(Utils::MonoStringToString(toName));
	}

	static void AnimatorComponent_AddTransition(UUID uuid, MonoString *fromName, MonoString *toName)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->AddTransition(Utils::MonoStringToString(fromName), Utils::MonoStringToString(toName));
	}

	static void AnimatorComponent_AddTransitionByIndex(UUID uuid, int fromIndex, int toIndex)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->AddTransition(fromIndex, toIndex);
	}

	static void AnimatorComponent_RemoveTransition(UUID uuid, MonoString *fromName, MonoString *toName)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->RemoveTransition(Utils::MonoStringToString(fromName), Utils::MonoStringToString(toName));
	}

	static void AnimatorComponent_RemoveTransitionByIndex(UUID uuid, int fromIndex, int toIndex)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<AnimatorComponent>().Animator2D->RemoveTransition(fromIndex, toIndex);
	}

	static bool AnimatorComponent_CanTransition(UUID uuid, MonoString *fromName, MonoString *toName)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<AnimatorComponent>().Animator2D->CanTransition(Utils::MonoStringToString(fromName), Utils::MonoStringToString(toName));
	}

	static bool AnimatorComponent_CanTransitionByIndex(UUID uuid, int fromIndex, int toIndex)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<AnimatorComponent>().Animator2D->CanTransition(fromIndex, toIndex);
	}
#pragma endregion

#pragma region Camera
	static bool CameraComponent_IsPrimary(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().Primary;
	}

	static void CameraComponent_SetPrimary(UUID uuid, bool primary)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Primary = primary;
	}

	static bool CameraComponent_IsFixedAspectRatio(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().FixedAspectRatio;
	}

	static void CameraComponent_SetFixedAspectRatio(UUID uuid, bool fixedAspectRatio)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().FixedAspectRatio = fixedAspectRatio;
	}

#pragma region SceneCamera
	static void CameraComponent_SetOrtograpic(UUID uuid, float size, float nearClip, float farClip)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetOrthographic(size, nearClip, farClip);
	}

	static void CameraComponent_SetPerspective(UUID uuid, float verticalFOV, float nearClip, float farClip)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetPerspective(verticalFOV, nearClip, farClip);
	}

	static SceneCamera::ProjectionType CameraComponent_GetProjectionType(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().Camera.GetProjectionType();
	}

	static void CameraComponent_SetProjectionType(UUID uuid, SceneCamera::ProjectionType projectionType)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetProjectionType(projectionType);
	}

	static float CameraComponent_GetOrthographicSize(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().Camera.GetOrthographicSize();
	}

	static void CameraComponent_SetOrthographicSize(UUID uuid, float orthographicSize)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetOrthographicSize(orthographicSize);
	}

	static float CameraComponent_GetOrthographicNearClip(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().Camera.GetOrthographicNearClip();
	}

	static void CameraComponent_SetOrthographicNearClip(UUID uuid, float orthographicNearClip)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetOrthographicNearClip(orthographicNearClip);
	}

	static float CameraComponent_GetOrthographicFarClip(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().Camera.GetOrthographicFarClip();
	}

	static void CameraComponent_SetOrthographicFarClip(UUID uuid, float orthographicFarClip)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetOrthographicFarClip(orthographicFarClip);
	}

	static float CameraComponent_GetPerspectiveVerticalFOV(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().Camera.GetPerspectiveVerticalFOV();
	}

	static void CameraComponent_SetPerspectiveVerticalFOV(UUID uuid, float perspectiveVerticalFOV)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetPerspectiveVerticalFOV(perspectiveVerticalFOV);
	}

	static float CameraComponent_GetPerspectiveNearClip(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().Camera.GetPerspectiveNearClip();
	}

	static void CameraComponent_SetPerspectiveNearClip(UUID uuid, float perspectiveNearClip)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetPerspectiveNearClip(perspectiveNearClip);
	}

	static float CameraComponent_GetPerspectiveFarClip(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		return entity.GetComponent<CameraComponent>().Camera.GetPerspectiveFarClip();
	}

	static void CameraComponent_SetPerspectiveFarClip(UUID uuid, float perspectiveFarClip)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<CameraComponent>().Camera.SetPerspectiveFarClip(perspectiveFarClip);
	}
#pragma endregion

#pragma endregion

#pragma region RigidBody2D
	static void RigidBody2DComponent_AwakeRuntimeBody(UUID uuid)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene, "No scene context!");
		Entity entity = scene->GetEntityByUUID(uuid);
		if(entity.HasComponent<RigidBody2DComponent>())
			scene->AwakeRuntimeBody(entity);
	}

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

		if(entity.GetName() == "projectile")
			ConsolePanel::Log(std::to_string(impulse->x), ConsolePanel::LogType::Info);

		auto &rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body *body = (b2Body *)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}

	static void RigidBody2DComponent_SetLinearVelocity(UUID uuid, glm::vec2* linearVelocity)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(uuid);
		EG_CORE_ASSERT(entity);

		auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		b2Vec2 velocity = b2Vec2(linearVelocity->x, linearVelocity->y);
		body->SetLinearVelocity(velocity);
	}

	static void RigidBody2DComponent_GetLinearVelocity(UUID entityID, glm::vec2 *outLinearVelocity)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);

		auto &rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body *body = (b2Body*)rb2d.RuntimeBody;
		const b2Vec2 &linearVelocity = body->GetLinearVelocity();
		*outLinearVelocity = glm::vec2(linearVelocity.x, linearVelocity.y);
	}

	static RigidBody2DComponent::BodyType RigidBody2DComponent_GetType(UUID entityID)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);

		return entity.GetComponent<RigidBody2DComponent>().Type;
	}

	static void RigidBody2DComponent_SetType(UUID entityID, RigidBody2DComponent::BodyType bodyType)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);

		entity.GetComponent<RigidBody2DComponent>().Type = bodyType;
	}

	static bool RigidBody2DComponent_IsFixedRotation(UUID entityID)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);

		return entity.GetComponent<RigidBody2DComponent>().FixedRotation;
	}

	static void RigidBody2DComponent_SetFixedRotation(UUID entityID, bool fixedRotation)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);

		entity.GetComponent<RigidBody2DComponent>().FixedRotation = fixedRotation;
	}
#pragma endregion

#pragma region BoxCollider2D
	static void BoxCollider2DComponent_GetOffset(UUID uuid, glm::vec2 *outOffset)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outOffset = entity.GetComponent<BoxCollider2DComponent>().Offset;
	}

	static void BoxCollider2DComponent_SetOffset(UUID uuid, glm::vec2 *offset)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<BoxCollider2DComponent>().Offset = *offset;
	}

	static void BoxCollider2DComponent_GetSize(UUID uuid, glm::vec2 *outSize)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outSize = entity.GetComponent<BoxCollider2DComponent>().Size;
	}

	static void BoxCollider2DComponent_SetSize(UUID uuid, glm::vec2 *size)
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

	static const b2PolygonShape& BoxCollider2DComponent_Getb2PolygonShape(Entity& entity, Scene* scene)
	{
		const auto& boxCollider = entity.GetComponent<BoxCollider2DComponent>();
		const auto& transform = entity.GetComponent<TransformComponent>();

		b2PolygonShape boxShape;
		boxShape.SetAsBox(boxCollider.Size.x * transform.Scale.x, boxCollider.Size.y * transform.Scale.y, b2Vec2(boxCollider.Offset.x + transform.Translation.x, boxCollider.Offset.y + transform.Translation.y), transform.Rotation.z);
		return boxShape;
	}

	static const b2CircleShape& CircleCollider2DComponent_Getb2CircleShape(Entity& entity, Scene* scene)
	{
		const auto& circleCollider = entity.GetComponent<CircleCollider2DComponent>();
		const auto& transform = entity.GetComponent<TransformComponent>();

		b2CircleShape circleShape;
		circleShape.m_p = b2Vec2(circleCollider.Offset.x + transform.Translation.x, circleCollider.Offset.y + transform.Translation.y);
		circleShape.m_radius = circleCollider.Radius;
		return circleShape;
	}

	static const b2EdgeShape& Getb2EdgeShapeFromBox(Entity& entity, const TransformComponent& transform, Scene* scene, Side side)
	{
		const auto& otherBoxCollider = entity.GetComponent<BoxCollider2DComponent>();

		b2EdgeShape edge;

		switch (side)
		{
		case Side::TOP:
			edge.SetTwoSided(b2Vec2(-otherBoxCollider.Size.x / 2.0f + transform.Translation.x, +otherBoxCollider.Size.y / 2.0f + transform.Translation.y), b2Vec2(+otherBoxCollider.Size.x / 2.0f + transform.Translation.x, +otherBoxCollider.Size.y / 2.0f + transform.Translation.y));
			break;
		case Side::BOTTOM:
			edge.SetTwoSided(b2Vec2(-otherBoxCollider.Size.x / 2.0f + transform.Translation.x, -otherBoxCollider.Size.y / 2.0f + transform.Translation.y), b2Vec2(+otherBoxCollider.Size.x / 2.0 + transform.Translation.x, -otherBoxCollider.Size.y / 2.0f + transform.Translation.y));
			break;
		case Side::LEFT:
			edge.SetTwoSided(b2Vec2(-otherBoxCollider.Size.x / 2.0f + transform.Translation.x, -otherBoxCollider.Size.y / 2.0f + transform.Translation.y), b2Vec2(-otherBoxCollider.Size.x / 2.0f + transform.Translation.x, +otherBoxCollider.Size.y / 2.0f + transform.Translation.y));
			break;
		case Side::RIGHT:
			edge.SetTwoSided(b2Vec2(+otherBoxCollider.Size.x / 2.0f + transform.Translation.x, -otherBoxCollider.Size.y / 2.0f + transform.Translation.y), b2Vec2(+otherBoxCollider.Size.x / 2.0f + transform.Translation.x, +otherBoxCollider.Size.y / 2.0f + transform.Translation.y));
			break;
		}

		return edge;
	}

	static bool Collider2D_TestOverlap(const b2Shape* shapeA, int32 indexA,
		const b2Shape* shapeB, int32 indexB,
		const TransformComponent& transformA, const TransformComponent& transformB) {
		return b2TestOverlap(shapeA, indexA, shapeB, indexB, b2Transform(b2Vec2(transformA.Translation.x, transformA.Translation.y), b2Rot(transformA.Rotation.z)), b2Transform(b2Vec2(transformA.Translation.x, transformB.Translation.y), b2Rot(transformB.Rotation.z)));
	}

	static bool Collider2D_TestOverlap(const b2Shape* shapeA, const b2Shape* shapeB)
	{
		b2Transform transform;
		transform.SetIdentity();
		return b2TestOverlap(shapeA, 0, shapeB, 0, transform, transform);
	}

	static b2PolygonShape* GetShapeFromBoxCollider2DComponent(const BoxCollider2DComponent& boxCollider)
	{
		b2Fixture* fixture = (b2Fixture*)boxCollider.RuntimeFixture;
		return (b2PolygonShape*)fixture->GetShape();
	}

	static b2CircleShape* GetShapeFromCircleCollider2DComponent(const CircleCollider2DComponent& circleCollider)
	{
		b2Fixture* fixture = (b2Fixture*)circleCollider.RuntimeFixture;
		return (b2CircleShape*)fixture->GetShape();
	}

	static bool BoxCollider2DComponent_CollidesWith(UUID uuid, UUID other)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity otherEntity = scene->GetEntityByUUID(other);

		if(!otherEntity)
			return false;

		const auto& collider = entity.GetComponent<BoxCollider2DComponent>();

		if(otherEntity.HasComponent<BoxCollider2DComponent>())
		{
			const auto& otherCollider = otherEntity.GetComponent<BoxCollider2DComponent>();
			return Collider2D_TestOverlap(GetShapeFromBoxCollider2DComponent(collider), GetShapeFromBoxCollider2DComponent(otherCollider));
		}
		else if(otherEntity.HasComponent<CircleCollider2DComponent>())
		{
			const auto& otherCollider = otherEntity.GetComponent<CircleCollider2DComponent>();
			return Collider2D_TestOverlap(GetShapeFromBoxCollider2DComponent(collider), GetShapeFromCircleCollider2DComponent(otherCollider));
		}

		return false;
	}

	static bool BoxCollider2DComponent_CollidesWithBox(UUID uuid, UUID other)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity otherEntity = scene->GetEntityByUUID(other);

		if(!otherEntity)
			return false;

		const auto& collider = entity.GetComponent<BoxCollider2DComponent>();
		const auto& otherCollider = otherEntity.GetComponent<BoxCollider2DComponent>();
		const auto& rigidBody = entity.GetComponent<RigidBody2DComponent>();
		const auto& otherRigidBody = otherEntity.GetComponent<RigidBody2DComponent>();

		b2Body* body = (b2Body*)rigidBody.RuntimeBody;
		b2Body* otherBody = (b2Body*)otherRigidBody.RuntimeBody;
		return b2TestOverlap(GetShapeFromBoxCollider2DComponent(collider), 0, GetShapeFromBoxCollider2DComponent(otherCollider), 0, body->GetTransform(), otherBody->GetTransform());
	}

	static bool BoxCollider2DComponent_CollidesWithPoint(UUID uuid, glm::vec2 *point)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		const auto& boxCollider = entity.GetComponent<BoxCollider2DComponent>();
		const auto& transform = entity.GetComponent<TransformComponent>();
		b2AABB aabb;
		
		aabb.lowerBound = b2Vec2(transform.Translation.x, transform.Translation.y) - b2Vec2(boxCollider.Size.x / 2.0f, boxCollider.Size.y / 2.0f);
		aabb.upperBound = b2Vec2(transform.Translation.x, transform.Translation.y) + b2Vec2(boxCollider.Size.x / 2.0f, boxCollider.Size.y / 2.0f);
		b2AABB pointAABB;
		pointAABB.lowerBound = b2Vec2(point->x, point->y);
		pointAABB.upperBound = b2Vec2(point->x, point->y);

		return aabb.Contains(pointAABB);
	}

	static bool BoxCollider2DComponent_CollidesWithCircle(UUID uuid, UUID other)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity otherEntity = scene->GetEntityByUUID(other);

		if(!otherEntity)
			return false;

		const auto& collider = entity.GetComponent<BoxCollider2DComponent>();
		const auto& circleCollider = otherEntity.GetComponent<CircleCollider2DComponent>();

		return Collider2D_TestOverlap(GetShapeFromBoxCollider2DComponent(collider), GetShapeFromCircleCollider2DComponent(circleCollider));
	}

	//static bool BoxCollider2DComponent_CollidesWithPolygon(UUID uuid, MonoArray *points)
	//{
	//	Scene *scene = ScriptEngine::GetSceneContext();
	//	Entity entity = scene->GetEntityByUUID(uuid);
	//	std::vector<glm::vec2> polygon;
	//	Utils::MonoArrayToVector(points, polygon);
	//	return entity.GetComponent<BoxCollider2DComponent>().CollidesWith(polygon);
	//}

	static bool BoxCollider2DComponent_CollidesWithEdge(UUID uuid, UUID other, Side side)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity otherEntity = scene->GetEntityByUUID(other);

		if(!otherEntity)
			return false;

		const auto& collider = entity.GetComponent<BoxCollider2DComponent>();
		const auto& otherTransform = otherEntity.GetComponent<TransformComponent>();

		b2EdgeShape edge = Getb2EdgeShapeFromBox(entity, otherTransform, scene, side);
		b2Transform edgeTransform = b2Transform(b2Vec2(0,0), b2Rot(otherTransform.Rotation.z));
		edgeTransform.SetIdentity();

		return b2TestOverlap(GetShapeFromBoxCollider2DComponent(collider), 0, &edge, 0, b2Transform(b2Vec2(0,0), b2Rot(0.0f)), edgeTransform);
	}

	static bool BoxCollider2DComponent_CollidesWithTopEdge(UUID uuid, UUID other)
	{
		return BoxCollider2DComponent_CollidesWithEdge(uuid, other, Side::TOP);
	}

	static bool BoxCollider2DComponent_CollidesWithBottomEdge(UUID uuid, UUID other)
	{
		return BoxCollider2DComponent_CollidesWithEdge(uuid, other, Side::BOTTOM);
	}

	static bool BoxCollider2DComponent_CollidesWithLeftEdge(UUID uuid, UUID other)
	{
		return BoxCollider2DComponent_CollidesWithEdge(uuid, other, Side::LEFT);
	}

	static bool BoxCollider2DComponent_CollidesWithRightEdge(UUID uuid, UUID other)
	{
		return BoxCollider2DComponent_CollidesWithEdge(uuid, other, Side::RIGHT);
	}

	static bool BoxCollider2DComponent_CollidesWithBoxCoords(UUID uuid, glm::vec2 *center, glm::vec2 *size)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);

		const auto& collider = entity.GetComponent<BoxCollider2DComponent>();
		b2PolygonShape otherShape;
		otherShape.SetAsBox(size->x, size->y, b2Vec2(center->x, center->y), 0);
		const auto& rigidBody = entity.GetComponent<RigidBody2DComponent>();

		b2Body* body = (b2Body*)rigidBody.RuntimeBody;

		b2Transform transform;
		transform.SetIdentity();

		return b2TestOverlap(GetShapeFromBoxCollider2DComponent(collider), 0, &otherShape, 0, body->GetTransform(), transform);
	}

	static bool BoxCollider2DComponent_CollidesWithCircleCoords(UUID uuid, glm::vec2 *center, float radius)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);

		const auto& collider = entity.GetComponent<BoxCollider2DComponent>();
		b2CircleShape otherShape;
		otherShape.m_p = b2Vec2(center->x, center->y);
		otherShape.m_radius = radius;

		return Collider2D_TestOverlap(GetShapeFromBoxCollider2DComponent(collider), &otherShape);
	}

	static bool BoxCollider2DComponent_CollidesWithEdgeCoords(UUID uuid, glm::vec2 *start, glm::vec2 *end)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);

		const auto& collider = entity.GetComponent<BoxCollider2DComponent>();
		b2EdgeShape edge;
		edge.SetTwoSided(b2Vec2(start->x, start->y), b2Vec2(end->x, end->y));

		return Collider2D_TestOverlap(GetShapeFromBoxCollider2DComponent(collider), &edge);
	}

#pragma endregion

#pragma region CircleCollider2D
	static void CircleCollider2DComponent_GetOffset(UUID uuid, glm::vec2 *outOffset)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outOffset = entity.GetComponent<CircleCollider2DComponent>().Offset;
	}

	static void CircleCollider2DComponent_SetOffset(UUID uuid, glm::vec2 *offset)
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

	static bool CircleCollider2DComponent_CollidesWith(UUID uuid, UUID other)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity otherEntity = scene->GetEntityByUUID(other);

		if(!otherEntity)
			return false;

		const auto& collider = entity.GetComponent<CircleCollider2DComponent>();

		if(otherEntity.HasComponent<BoxCollider2DComponent>())
		{
			const auto& otherBox = otherEntity.GetComponent<BoxCollider2DComponent>();
			return Collider2D_TestOverlap(GetShapeFromCircleCollider2DComponent(collider), GetShapeFromBoxCollider2DComponent(otherBox));
		}
		else if(otherEntity.HasComponent<CircleCollider2DComponent>())
		{
			const auto& otherCollider = otherEntity.GetComponent<CircleCollider2DComponent>();
			return Collider2D_TestOverlap(GetShapeFromCircleCollider2DComponent(collider), GetShapeFromCircleCollider2DComponent(otherCollider));
		}

		return false;
	}

	static bool CircleCollider2DComponent_CollidesWithCircle(UUID uuid, UUID other)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity otherEntity = scene->GetEntityByUUID(other);

		if(!otherEntity)
			return false;

		const auto& collider = entity.GetComponent<CircleCollider2DComponent>();
		const auto& otherCollider = otherEntity.GetComponent<CircleCollider2DComponent>();

		return Collider2D_TestOverlap(GetShapeFromCircleCollider2DComponent(collider), GetShapeFromCircleCollider2DComponent(otherCollider));
		}

	static bool CircleCollider2DComponent_CollidesWithPoint(UUID uuid, glm::vec2* point)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);

		const auto& collider = entity.GetComponent<CircleCollider2DComponent>();
		
		b2PolygonShape pointShape;
		pointShape.SetAsBox(0.0f, 0.0f, b2Vec2(point->x, point->y), 0.0f);

		return Collider2D_TestOverlap(GetShapeFromCircleCollider2DComponent(collider), &pointShape);
	}

	static bool CircleCollider2DComponent_CollidesWithBox(UUID uuid, UUID other)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity otherEntity = scene->GetEntityByUUID(other);

		if(!otherEntity)
			return false;

		const auto& collider = entity.GetComponent<CircleCollider2DComponent>();
		b2PolygonShape boxShape = BoxCollider2DComponent_Getb2PolygonShape(otherEntity, scene);

		return Collider2D_TestOverlap(GetShapeFromCircleCollider2DComponent(collider), &boxShape);
	}

	//static bool CircleCollider2DComponent_CollidesWithPolygon(UUID uuid, MonoArray *points)
	//{
	//}

	static bool CircleCollider2DComponent_CollidesWithEdge(UUID uuid, UUID other, Side side)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		Entity otherEntity = scene->GetEntityByUUID(other);
		
		if(!otherEntity)
			return false;



		const auto& collider = entity.GetComponent<CircleCollider2DComponent>();
		const auto& otherTransform = otherEntity.GetComponent<TransformComponent>();
		b2EdgeShape edge = Getb2EdgeShapeFromBox(entity, otherTransform, scene, side);
		b2Transform edgeTransform = b2Transform(b2Vec2(0,0), b2Rot(otherTransform.Rotation.z));
		edgeTransform.SetIdentity();

		return b2TestOverlap(GetShapeFromCircleCollider2DComponent(collider), 0, &edge, 0, b2Transform(b2Vec2(0.0f, 0.0f), b2Rot(0.0f)), edgeTransform);
	}

	static bool CircleCollider2DComponent_CollidesWithTopEdge(UUID uuid, UUID other)
	{
		return CircleCollider2DComponent_CollidesWithEdge(uuid, other, Side::TOP);
	}

	static bool CircleCollider2DComponent_CollidesWithBottomEdge(UUID uuid, UUID other)
	{
		return CircleCollider2DComponent_CollidesWithEdge(uuid, other, Side::BOTTOM);
	}

	static bool CircleCollider2DComponent_CollidesWithLeftEdge(UUID uuid, UUID other)
	{
		return CircleCollider2DComponent_CollidesWithEdge(uuid, other, Side::LEFT);
	}

	static bool CircleCollider2DComponent_CollidesWithRightEdge(UUID uuid, UUID other)
	{
		return CircleCollider2DComponent_CollidesWithEdge(uuid, other, Side::RIGHT);
	}

	static bool CircleCollider2DComponent_CollidesWithBoxCoords(UUID uuid, glm::vec2 *center, glm::vec2 *size)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);

		const auto& collider = entity.GetComponent<CircleCollider2DComponent>();
		b2PolygonShape boxShape;
		boxShape.SetAsBox(size->x, size->y, b2Vec2(center->x, center->y), 0.0f);

		b2CircleShape* shape = GetShapeFromCircleCollider2DComponent(collider);

		return Collider2D_TestOverlap(GetShapeFromCircleCollider2DComponent(collider), &boxShape);
	}

	static bool CircleCollider2DComponent_CollidesWithCircleCoords(UUID uuid, glm::vec2 *center, float radius)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);

		const auto& collider = entity.GetComponent<CircleCollider2DComponent>();
		b2CircleShape otherShape;
		otherShape.m_p = b2Vec2(center->x, center->y);
		otherShape.m_radius = radius;

		return Collider2D_TestOverlap(GetShapeFromCircleCollider2DComponent(collider), &otherShape);
	}

	static bool CircleCollider2DComponent_CollidesWithEdgeCoords(UUID uuid, glm::vec2 *start, glm::vec2 *end)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);

		const auto& collider = entity.GetComponent<CircleCollider2DComponent>();
		b2EdgeShape edge;
		edge.SetTwoSided(b2Vec2(start->x, start->y), b2Vec2(end->x, end->y));

		return Collider2D_TestOverlap(GetShapeFromCircleCollider2DComponent(collider), &edge);
	}

	
#pragma endregion

#pragma region Text
	static MonoString *TextComponent_GetText(UUID entityID)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);
		EG_CORE_ASSERT(entity.HasComponent<TextComponent>());

		auto &tc = entity.GetComponent<TextComponent>();
		return ScriptEngine::CreateString(tc.TextString.c_str());
	}

	static void TextComponent_SetText(UUID entityID, MonoString *textString)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);
		EG_CORE_ASSERT(entity.HasComponent<TextComponent>());

		auto &tc = entity.GetComponent<TextComponent>();
		tc.TextString = Utils::MonoStringToString(textString);
	}

	static void TextComponent_GetColor(UUID entityID, glm::vec4 *color)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);
		EG_CORE_ASSERT(entity.HasComponent<TextComponent>());

		auto &tc = entity.GetComponent<TextComponent>();
		*color = tc.Color;
	}

	static void TextComponent_SetColor(UUID entityID, glm::vec4 *color)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);
		EG_CORE_ASSERT(entity.HasComponent<TextComponent>());

		auto &tc = entity.GetComponent<TextComponent>();
		tc.Color = *color;
	}

	static float TextComponent_GetKerning(UUID entityID)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);
		EG_CORE_ASSERT(entity.HasComponent<TextComponent>());

		auto &tc = entity.GetComponent<TextComponent>();
		return tc.Kerning;
	}

	static void TextComponent_SetKerning(UUID entityID, float kerning)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);
		EG_CORE_ASSERT(entity.HasComponent<TextComponent>());

		auto &tc = entity.GetComponent<TextComponent>();
		tc.Kerning = kerning;
	}

	static float TextComponent_GetLineSpacing(UUID entityID)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);
		EG_CORE_ASSERT(entity.HasComponent<TextComponent>());

		auto &tc = entity.GetComponent<TextComponent>();
		return tc.LineSpacing;
	}

	static void TextComponent_SetLineSpacing(UUID entityID, float lineSpacing)
	{
		Scene *scene = ScriptEngine::GetSceneContext();
		EG_CORE_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		EG_CORE_ASSERT(entity);
		EG_CORE_ASSERT(entity.HasComponent<TextComponent>());

		auto &tc = entity.GetComponent<TextComponent>();
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
		EG_ADD_INTERNAL_CALL(Entity_Exists);
		EG_ADD_INTERNAL_CALL(Console_Log);

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
		EG_ADD_INTERNAL_CALL(AnimatorComponent_TransitionByIndex);
		EG_ADD_INTERNAL_CALL(AnimatorComponent_Transition);
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

		EG_ADD_INTERNAL_CALL(RigidBody2DComponent_AwakeRuntimeBody);
		EG_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulse);
		EG_ADD_INTERNAL_CALL(RigidBody2DComponent_ApplyLinearImpulseToCenter);
		EG_ADD_INTERNAL_CALL(RigidBody2DComponent_GetLinearVelocity);
		EG_ADD_INTERNAL_CALL(RigidBody2DComponent_SetLinearVelocity);
		EG_ADD_INTERNAL_CALL(RigidBody2DComponent_GetType);
		EG_ADD_INTERNAL_CALL(RigidBody2DComponent_SetType);
		EG_ADD_INTERNAL_CALL(RigidBody2DComponent_IsFixedRotation);
		EG_ADD_INTERNAL_CALL(RigidBody2DComponent_SetFixedRotation);

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
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_CollidesWith);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_CollidesWithBox);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_CollidesWithPoint);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_CollidesWithCircle);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_CollidesWithTopEdge);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_CollidesWithBottomEdge);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_CollidesWithLeftEdge);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_CollidesWithRightEdge);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_CollidesWithBoxCoords);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_CollidesWithCircleCoords);
		EG_ADD_INTERNAL_CALL(BoxCollider2DComponent_CollidesWithEdgeCoords);

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
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_CollidesWith);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_CollidesWithCircle);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_CollidesWithPoint);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_CollidesWithBox);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_CollidesWithTopEdge);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_CollidesWithBottomEdge);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_CollidesWithLeftEdge);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_CollidesWithRightEdge);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_CollidesWithBoxCoords);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_CollidesWithCircleCoords);
		EG_ADD_INTERNAL_CALL(CircleCollider2DComponent_CollidesWithEdgeCoords);

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
				s_EntityHasComponentFunctions[managedType] = [](Entity entity) { return entity.HasComponent<Component>();}; }(),
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