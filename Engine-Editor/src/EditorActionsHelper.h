#pragma once

#include "Engine.h"
#include "Commands/Commands.h"

namespace eg
{
	class ComponentHelper
	{
	public:
		enum class ComponentAction
		{
			AddComponent, RemoveComponent, HasComponent, InheritComponentInChildren, InheritComponentFromParent, CopyComponentToChildren, RemoveComponentFromChildren, CopyComponentValuesToChildren, CopyComponentWithValuesToChildren, IsInheritedFromParent, IsInheritedInChildren
		};

		static bool ExecuteComponentAction(ComponentAction action, std::string componentString, Entity e, Ref<Scene> scene)
		{
			switch (action)
			{
			case ComponentAction::AddComponent:
				return AddComponent(componentString, e, scene);
			case ComponentAction::RemoveComponent:
				return RemoveComponent(componentString, e, scene);
			case ComponentAction::HasComponent:
				return HasComponent(componentString, e, scene);
			case ComponentAction::InheritComponentInChildren:
				return InheritComponentInChildren(componentString, e, scene);
			case ComponentAction::InheritComponentFromParent:
				return InheritComponentFromParent(componentString, e, scene);
			case ComponentAction::CopyComponentToChildren:
				return CopyComponentToChildren(componentString, e, scene);
			case ComponentAction::RemoveComponentFromChildren:
				return RemoveComponentFromChildren(componentString, e, scene);
			case ComponentAction::CopyComponentValuesToChildren:
				return CopyComponentValuesToChildren(componentString, e, scene);
			case ComponentAction::CopyComponentWithValuesToChildren:
				return CopyComponentWithValuesToChildren(componentString, e, scene);
			case ComponentAction::IsInheritedFromParent:
				return IsInheritedFromParent(componentString, e, scene);
			case ComponentAction::IsInheritedInChildren:
				return IsInheritedInChildren(componentString, e, scene);
			default:
				return false;

			}
		}

		static bool AddComponent(std::string componentString, Entity e, Ref<Scene> scene)
		{
			if (componentString == "SpriteRendererComponent")
				Commands::ExecuteCommand<Commands::AddComponentCommand<SpriteRendererComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "CircleRendererComponent")
				Commands::ExecuteCommand<Commands::AddComponentCommand<CircleRendererComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "CameraComponent")
				Commands::ExecuteCommand<Commands::AddComponentCommand<CameraComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "ScriptComponent")
				Commands::ExecuteCommand<Commands::AddComponentCommand<ScriptComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "RigidBody2DComponent")
				Commands::ExecuteCommand<Commands::AddComponentCommand<RigidBody2DComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "BoxCollider2DComponent")
				Commands::ExecuteCommand<Commands::AddComponentCommand<BoxCollider2DComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "CircleCollider2DComponent")
				Commands::ExecuteCommand<Commands::AddComponentCommand<CircleCollider2DComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "TextComponent")
				Commands::ExecuteCommand<Commands::AddComponentCommand<TextComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "SpriteRendererSTComponent")
				Commands::ExecuteCommand<Commands::AddComponentCommand<SpriteRendererSTComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "AnimatorComponent")
				Commands::ExecuteCommand<Commands::AddComponentCommand<AnimatorComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "AudioSourceComponent")
				Commands::ExecuteCommand<Commands::AddComponentCommand<AudioSourceComponent>>(Commands::CommandArgs("", e, scene, e));
			else
				return false;

			return true;
		}

		static bool RemoveComponent(std::string componentString, Entity e, Ref<Scene> scene)
		{
			if (componentString == "SpriteRendererComponent")
				Commands::ExecuteCommand<Commands::RemoveComponentCommand<SpriteRendererComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "CircleRendererComponent")
				Commands::ExecuteCommand<Commands::RemoveComponentCommand<CircleRendererComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "CameraComponent")
				Commands::ExecuteCommand<Commands::RemoveComponentCommand<CameraComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "ScriptComponent")
				Commands::ExecuteCommand<Commands::RemoveComponentCommand<ScriptComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "RigidBody2DComponent")
				Commands::ExecuteCommand<Commands::RemoveComponentCommand<RigidBody2DComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "BoxCollider2DComponent")
				Commands::ExecuteCommand<Commands::RemoveComponentCommand<BoxCollider2DComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "CircleCollider2DComponent")
				Commands::ExecuteCommand<Commands::RemoveComponentCommand<CircleCollider2DComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "TextComponent")
				Commands::ExecuteCommand<Commands::RemoveComponentCommand<TextComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "SpriteRendererSTComponent")
				Commands::ExecuteCommand<Commands::RemoveComponentCommand<SpriteRendererSTComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "AnimatorComponent")
				Commands::ExecuteCommand<Commands::RemoveComponentCommand<AnimatorComponent>>(Commands::CommandArgs("", e, scene, e));
			else if (componentString == "AudioSourceComponent")
				Commands::ExecuteCommand<Commands::RemoveComponentCommand<AudioSourceComponent>>(Commands::CommandArgs("", e, scene, e));
			else
				return false;

			return true;
		}

		static bool HasComponent(std::string componentString, Entity e, Ref<Scene> scene)
		{
			if (componentString == "SpriteRendererComponent")
				return e.HasComponent<SpriteRendererComponent>();
			else if (componentString == "CircleRendererComponent")
				return e.HasComponent<CircleRendererComponent>();
			else if (componentString == "CameraComponent")
				return e.HasComponent<CameraComponent>();
			else if (componentString == "ScriptComponent")
				return e.HasComponent<ScriptComponent>();
			else if (componentString == "RigidBody2DComponent")
				return e.HasComponent<RigidBody2DComponent>();
			else if (componentString == "BoxCollider2DComponent")
				return e.HasComponent<BoxCollider2DComponent>();
			else if (componentString == "CircleCollider2DComponent")
				return e.HasComponent<CircleCollider2DComponent>();
			else if (componentString == "TextComponent")
				return e.HasComponent<TextComponent>();
			else if (componentString == "SpriteRendererSTComponent")
				return e.HasComponent<SpriteRendererSTComponent>();
			else if (componentString == "AnimatorComponent")
				return e.HasComponent<AnimatorComponent>();
			else if (componentString == "AudioSourceComponent")
				return e.HasComponent<AudioSourceComponent>();
			else
				return false;

			return true;
		}

		static bool InheritComponentInChildren(std::string componentString, Entity e, Ref<Scene> scene)
		{
			if (componentString == "SpriteRendererComponent")
				Commands::ExecuteInheritComponentCommand<SpriteRendererComponent>(e, scene, e.GetInheritableComponent<SpriteRendererComponent>()->isInheritedInChildren);
			else if (componentString == "CircleRendererComponent")
				Commands::ExecuteInheritComponentCommand<CircleRendererComponent>(e, scene, e.GetInheritableComponent<CircleRendererComponent>()->isInheritedInChildren);
			else if (componentString == "CameraComponent")
				Commands::ExecuteInheritComponentCommand<CameraComponent>(e, scene, e.GetInheritableComponent<CameraComponent>()->isInheritedInChildren);
			else if (componentString == "ScriptComponent")
				Commands::ExecuteInheritComponentCommand<ScriptComponent>(e, scene, e.GetInheritableComponent<ScriptComponent>()->isInheritedInChildren);
			else if (componentString == "RigidBody2DComponent")
				Commands::ExecuteInheritComponentCommand<RigidBody2DComponent>(e, scene, e.GetInheritableComponent<RigidBody2DComponent>()->isInheritedInChildren);
			else if (componentString == "BoxCollider2DComponent")
				Commands::ExecuteInheritComponentCommand<BoxCollider2DComponent>(e, scene, e.GetInheritableComponent<BoxCollider2DComponent>()->isInheritedInChildren);
			else if (componentString == "CircleCollider2DComponent")
				Commands::ExecuteInheritComponentCommand<CircleCollider2DComponent>(e, scene, e.GetInheritableComponent<CircleCollider2DComponent>()->isInheritedInChildren);
			else if (componentString == "TextComponent")
				Commands::ExecuteInheritComponentCommand<TextComponent>(e, scene, e.GetInheritableComponent<TextComponent>()->isInheritedInChildren);
			else if (componentString == "SpriteRendererSTComponent")
				Commands::ExecuteInheritComponentCommand<SpriteRendererSTComponent>(e, scene, e.GetInheritableComponent<SpriteRendererSTComponent>()->isInheritedInChildren);
			else if (componentString == "AnimatorComponent")
				Commands::ExecuteInheritComponentCommand<AnimatorComponent>(e, scene, e.GetInheritableComponent<AnimatorComponent>()->isInheritedInChildren);
			else if (componentString == "AudioSourceComponent")
				Commands::ExecuteInheritComponentCommand<AudioSourceComponent>(e, scene, e.GetInheritableComponent<AudioSourceComponent>()->isInheritedInChildren);
			else
				return false;

			return true;
		}

		static bool InheritComponentFromParent(std::string componentString, Entity e, Ref<Scene> scene)
		{
			if (componentString == "SpriteRendererComponent")
				Commands::ExecuteInheritComponentCommand<SpriteRendererComponent>(e, scene, e.GetInheritableComponent<SpriteRendererComponent>()->isInherited, true);
			else if (componentString == "CircleRendererComponent")
				Commands::ExecuteInheritComponentCommand<CircleRendererComponent>(e, scene, e.GetInheritableComponent<CircleRendererComponent>()->isInherited, true);
			else if (componentString == "CameraComponent")
				Commands::ExecuteInheritComponentCommand<CameraComponent>(e, scene, e.GetInheritableComponent<CameraComponent>()->isInherited, true);
			else if (componentString == "ScriptComponent")
				Commands::ExecuteInheritComponentCommand<ScriptComponent>(e, scene, e.GetInheritableComponent<ScriptComponent>()->isInherited, true);
			else if (componentString == "RigidBody2DComponent")
				Commands::ExecuteInheritComponentCommand<RigidBody2DComponent>(e, scene, e.GetInheritableComponent<RigidBody2DComponent>()->isInherited, true);
			else if (componentString == "BoxCollider2DComponent")
				Commands::ExecuteInheritComponentCommand<BoxCollider2DComponent>(e, scene, e.GetInheritableComponent<BoxCollider2DComponent>()->isInherited, true);
			else if (componentString == "CircleCollider2DComponent")
				Commands::ExecuteInheritComponentCommand<CircleCollider2DComponent>(e, scene, e.GetInheritableComponent<CircleCollider2DComponent>()->isInherited, true);
			else if (componentString == "TextComponent")
				Commands::ExecuteInheritComponentCommand<TextComponent>(e, scene, e.GetInheritableComponent<TextComponent>()->isInherited, true);
			else if (componentString == "SpriteRendererSTComponent")
				Commands::ExecuteInheritComponentCommand<SpriteRendererSTComponent>(e, scene, e.GetInheritableComponent<SpriteRendererSTComponent>()->isInherited, true);
			else if (componentString == "AnimatorComponent")
				Commands::ExecuteInheritComponentCommand<AnimatorComponent>(e, scene, e.GetInheritableComponent<AnimatorComponent>()->isInherited, true);
			else if (componentString == "AudioSourceComponent")
				Commands::ExecuteInheritComponentCommand<AudioSourceComponent>(e, scene, e.GetInheritableComponent<AudioSourceComponent>()->isInherited, true);
			else
				return false;

			return true;
		}

		static bool CopyComponentToChildren(std::string componentString, Entity e, Ref<Scene> scene)
		{
			if (componentString == "SpriteRendererComponent")
				Commands::ExecuteManageComponentInheritanceCommand<SpriteRendererComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT);
			else if (componentString == "CircleRendererComponent")
				Commands::ExecuteManageComponentInheritanceCommand<CircleRendererComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT);
			else if (componentString == "CameraComponent")
				Commands::ExecuteManageComponentInheritanceCommand<CameraComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT);
			else if (componentString == "ScriptComponent")
				Commands::ExecuteManageComponentInheritanceCommand<ScriptComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT);
			else if (componentString == "RigidBody2DComponent")
				Commands::ExecuteManageComponentInheritanceCommand<RigidBody2DComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT);
			else if (componentString == "BoxCollider2DComponent")
				Commands::ExecuteManageComponentInheritanceCommand<BoxCollider2DComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT);
			else if (componentString == "CircleCollider2DComponent")
				Commands::ExecuteManageComponentInheritanceCommand<CircleCollider2DComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT);
			else if (componentString == "TextComponent")
				Commands::ExecuteManageComponentInheritanceCommand<TextComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT);
			else if (componentString == "SpriteRendererSTComponent")
				Commands::ExecuteManageComponentInheritanceCommand<SpriteRendererSTComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT);
			else if (componentString == "AnimatorComponent")
				Commands::ExecuteManageComponentInheritanceCommand<AnimatorComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT);
			else if (componentString == "AudioSourceComponent")
				Commands::ExecuteManageComponentInheritanceCommand<AudioSourceComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT);
			else
				return false;

			return true;
		}

		static bool RemoveComponentFromChildren(std::string componentString, Entity e, Ref<Scene> scene)
		{
			if (componentString == "SpriteRendererComponent")
				Commands::ExecuteManageComponentInheritanceCommand<SpriteRendererComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT, true);
			else if (componentString == "CircleRendererComponent")
				Commands::ExecuteManageComponentInheritanceCommand<CircleRendererComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT, true);
			else if (componentString == "CameraComponent")
				Commands::ExecuteManageComponentInheritanceCommand<CameraComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT, true);
			else if (componentString == "ScriptComponent")
				Commands::ExecuteManageComponentInheritanceCommand<ScriptComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT, true);
			else if (componentString == "RigidBody2DComponent")
				Commands::ExecuteManageComponentInheritanceCommand<RigidBody2DComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT, true);
			else if (componentString == "BoxCollider2DComponent")
				Commands::ExecuteManageComponentInheritanceCommand<BoxCollider2DComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT, true);
			else if (componentString == "CircleCollider2DComponent")
				Commands::ExecuteManageComponentInheritanceCommand<CircleCollider2DComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT, true);
			else if (componentString == "TextComponent")
				Commands::ExecuteManageComponentInheritanceCommand<TextComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT, true);
			else if (componentString == "SpriteRendererSTComponent")
				Commands::ExecuteManageComponentInheritanceCommand<SpriteRendererSTComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT, true);
			else if (componentString == "AnimatorComponent")
				Commands::ExecuteManageComponentInheritanceCommand<AnimatorComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT, true);
			else if (componentString == "AudioSourceComponent")
				Commands::ExecuteManageComponentInheritanceCommand<AudioSourceComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT, true);
			else
				return false;

			return true;
		}

		static bool CopyComponentValuesToChildren(std::string componentString, Entity e, Ref<Scene> scene)
		{
			if (componentString == "SpriteRendererComponent")
				Commands::ExecuteManageComponentInheritanceCommand<SpriteRendererComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_VALUES);
			else if (componentString == "CircleRendererComponent")
				Commands::ExecuteManageComponentInheritanceCommand<CircleRendererComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_VALUES);
			else if (componentString == "CameraComponent")
				Commands::ExecuteManageComponentInheritanceCommand<CameraComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_VALUES);
			else if (componentString == "ScriptComponent")
				Commands::ExecuteManageComponentInheritanceCommand<ScriptComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_VALUES);
			else if (componentString == "RigidBody2DComponent")
				Commands::ExecuteManageComponentInheritanceCommand<RigidBody2DComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_VALUES);
			else if (componentString == "BoxCollider2DComponent")
				Commands::ExecuteManageComponentInheritanceCommand<BoxCollider2DComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_VALUES);
			else if (componentString == "CircleCollider2DComponent")
				Commands::ExecuteManageComponentInheritanceCommand<CircleCollider2DComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_VALUES);
			else if (componentString == "TextComponent")
				Commands::ExecuteManageComponentInheritanceCommand<TextComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_VALUES);
			else if (componentString == "SpriteRendererSTComponent")
				Commands::ExecuteManageComponentInheritanceCommand<SpriteRendererSTComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_VALUES);
			else if (componentString == "AnimatorComponent")
				Commands::ExecuteManageComponentInheritanceCommand<AnimatorComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_VALUES);
			else if (componentString == "AudioSourceComponent")
				Commands::ExecuteManageComponentInheritanceCommand<AudioSourceComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_VALUES);
			else
				return false;

			return true;
		}

		static bool CopyComponentWithValuesToChildren(std::string componentString, Entity e, Ref<Scene> scene)
		{
			if (componentString == "SpriteRendererComponent")
				Commands::ExecuteManageComponentInheritanceCommand<SpriteRendererComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_AND_VALUES);
			else if (componentString == "CircleRendererComponent")
				Commands::ExecuteManageComponentInheritanceCommand<CircleRendererComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_AND_VALUES);
			else if (componentString == "CameraComponent")
				Commands::ExecuteManageComponentInheritanceCommand<CameraComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_AND_VALUES);
			else if (componentString == "ScriptComponent")
				Commands::ExecuteManageComponentInheritanceCommand<ScriptComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_AND_VALUES);
			else if (componentString == "RigidBody2DComponent")
				Commands::ExecuteManageComponentInheritanceCommand<RigidBody2DComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_AND_VALUES);
			else if (componentString == "BoxCollider2DComponent")
				Commands::ExecuteManageComponentInheritanceCommand<BoxCollider2DComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_AND_VALUES);
			else if (componentString == "CircleCollider2DComponent")
				Commands::ExecuteManageComponentInheritanceCommand<CircleCollider2DComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_AND_VALUES);
			else if (componentString == "TextComponent")
				Commands::ExecuteManageComponentInheritanceCommand<TextComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_AND_VALUES);
			else if (componentString == "SpriteRendererSTComponent")
				Commands::ExecuteManageComponentInheritanceCommand<SpriteRendererSTComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_AND_VALUES);
			else if (componentString == "AnimatorComponent")
				Commands::ExecuteManageComponentInheritanceCommand<AnimatorComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_AND_VALUES);
			else if (componentString == "AudioSourceComponent")
				Commands::ExecuteManageComponentInheritanceCommand<AudioSourceComponent>(e, scene, Commands::InheritanceCommandType::COPY_COMPONENT_AND_VALUES);
			else
				return false;

			return true;
		}

		static bool IsInheritedFromParent(std::string componentString, Entity e, Ref<Scene> scene)
		{
			if (componentString == "SpriteRendererComponent")
				return e.GetInheritableComponent<SpriteRendererComponent>()->isInherited;
			else if (componentString == "CircleRendererComponent")
				return e.GetInheritableComponent<CircleRendererComponent>()->isInherited;
			else if (componentString == "CameraComponent")
				return e.GetInheritableComponent<CameraComponent>()->isInherited;
			else if (componentString == "ScriptComponent")
				return e.GetInheritableComponent<ScriptComponent>()->isInherited;
			else if (componentString == "RigidBody2DComponent")
				return e.GetInheritableComponent<RigidBody2DComponent>()->isInherited;
			else if (componentString == "BoxCollider2DComponent")
				return e.GetInheritableComponent<BoxCollider2DComponent>()->isInherited;
			else if (componentString == "CircleCollider2DComponent")
				return e.GetInheritableComponent<CircleCollider2DComponent>()->isInherited;
			else if (componentString == "TextComponent")
				return e.GetInheritableComponent<TextComponent>()->isInherited;
			else if (componentString == "SpriteRendererSTComponent")
				return e.GetInheritableComponent<SpriteRendererSTComponent>()->isInherited;
			else if (componentString == "AnimatorComponent")
				return e.GetInheritableComponent<AnimatorComponent>()->isInherited;
			else if (componentString == "AudioSourceComponent")
				return e.GetInheritableComponent<AudioSourceComponent>()->isInherited;
			else
				return false;

			return true;
		}

		static bool IsInheritedInChildren(std::string componentString, Entity e, Ref<Scene> scene)
		{
			if (componentString == "SpriteRendererComponent")
				return e.GetInheritableComponent<SpriteRendererComponent>()->isInheritedInChildren;
			else if (componentString == "CircleRendererComponent")
				return e.GetInheritableComponent<CircleRendererComponent>()->isInheritedInChildren;
			else if (componentString == "CameraComponent")
				return e.GetInheritableComponent<CameraComponent>()->isInheritedInChildren;
			else if (componentString == "ScriptComponent")
				return e.GetInheritableComponent<ScriptComponent>()->isInheritedInChildren;
			else if (componentString == "RigidBody2DComponent")
				return e.GetInheritableComponent<RigidBody2DComponent>()->isInheritedInChildren;
			else if (componentString == "BoxCollider2DComponent")
				return e.GetInheritableComponent<BoxCollider2DComponent>()->isInheritedInChildren;
			else if (componentString == "CircleCollider2DComponent")
				return e.GetInheritableComponent<CircleCollider2DComponent>()->isInheritedInChildren;
			else if (componentString == "TextComponent")
				return e.GetInheritableComponent<TextComponent>()->isInheritedInChildren;
			else if (componentString == "SpriteRendererSTComponent")
				return e.GetInheritableComponent<SpriteRendererSTComponent>()->isInheritedInChildren;
			else if (componentString == "AnimatorComponent")
				return e.GetInheritableComponent<AnimatorComponent>()->isInheritedInChildren;
			else if (componentString == "AudioSourceComponent")
				return e.GetInheritableComponent<AudioSourceComponent>()->isInheritedInChildren;
			else
				return false;

			return true;
		}
	};
}
