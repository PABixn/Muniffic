#include "egpch.h"
#include "Entity.h"
#include "../Engine-Editor/src/Panels/ConsolePanel.h"
namespace eg {
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

	template<>
	void Entity::SetComponent<TagComponent>(Entity& entity, TagComponent* component)
	{
		if (!entity.HasComponent<TagComponent>())
			entity.AddComponent<TagComponent>();

		auto& tagComponent = entity.GetComponent<TagComponent>();

		tagComponent.Tag = component->Tag;
		ConsolePanel::Log("File: Entity.cpp - Tag component set successfully", ConsolePanel::LogType::Info);
	}

	template<>
	void Entity::SetComponent<TransformComponent>(Entity& entity, TransformComponent* component)
	{
		if (!entity.HasComponent<TransformComponent>())
			entity.AddComponent<TransformComponent>();

		auto& transformComponent = entity.GetComponent<TransformComponent>();

		transformComponent.Translation = component->Translation;
		transformComponent.Rotation = component->Rotation;
		transformComponent.Scale = component->Scale;
		transformComponent.isInherited = component->isInherited;
		transformComponent.isInheritedInChildren = component->isInheritedInChildren;
		ConsolePanel::Log("File: Entity.cpp - Transform component set successfully", ConsolePanel::LogType::Info);
	}

	template<>
	void Entity::SetComponent<SpriteRendererComponent>(Entity& entity, SpriteRendererComponent* component)
	{
		if (!entity.HasComponent<SpriteRendererComponent>())
			entity.AddComponent<SpriteRendererComponent>();

		auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();

		spriteRendererComponent.TextureUUID = component->TextureUUID;
		spriteRendererComponent.Color = component->Color;
		spriteRendererComponent.Texture = component->Texture;
		spriteRendererComponent.TilingFactor = component->TilingFactor;
		spriteRendererComponent.isInherited = component->isInherited;
		spriteRendererComponent.isInheritedInChildren = component->isInheritedInChildren;
		ConsolePanel::Log("File: Entity.cpp - Sprite Renderer component set successfully", ConsolePanel::LogType::Info);
	}

	template<>
	void Entity::SetComponent<SpriteRendererSTComponent>(Entity& entity, SpriteRendererSTComponent* component)
	{
		if (!entity.HasComponent<SpriteRendererSTComponent>())
			entity.AddComponent<SpriteRendererSTComponent>();

		auto& spriteRendererSTComponent = entity.GetComponent<SpriteRendererSTComponent>();

		spriteRendererSTComponent.SubTextureUUID = component->SubTextureUUID;
		spriteRendererSTComponent.Color = component->Color;
		spriteRendererSTComponent.SubTexture = component->SubTexture;
		spriteRendererSTComponent.TilingFactor = component->TilingFactor;
		spriteRendererSTComponent.isInherited = component->isInherited;
		spriteRendererSTComponent.isInheritedInChildren = component->isInheritedInChildren;
	}

	template<>
	void Entity::SetComponent<CameraComponent>(Entity& entity, CameraComponent* component)
	{
		if (!entity.HasComponent<CameraComponent>())
			entity.AddComponent<CameraComponent>();

		auto& cameraComponent = entity.GetComponent<CameraComponent>();

		cameraComponent.Primary = component->Primary;
		cameraComponent.FixedAspectRatio = component->FixedAspectRatio;
		cameraComponent.Camera.SetProjectionType(component->Camera.GetProjectionType());
		cameraComponent.Camera.SetPerspective(component->Camera.GetPerspectiveVerticalFOV(), component->Camera.GetPerspectiveNearClip(), component->Camera.GetPerspectiveFarClip());
		cameraComponent.Camera.SetOrthographic(component->Camera.GetOrthographicSize(), component->Camera.GetOrthographicNearClip(), component->Camera.GetOrthographicFarClip());
		cameraComponent.isInherited = component->isInherited;
		cameraComponent.isInheritedInChildren = component->isInheritedInChildren;
		ConsolePanel::Log("File: Entity.cpp - Camera component set successfully", ConsolePanel::LogType::Info);
	}

	template<>
	void Entity::SetComponent<ScriptComponent>(Entity& entity, ScriptComponent* component)
	{
		if (!entity.HasComponent<ScriptComponent>())
			entity.AddComponent<ScriptComponent>();

		auto& scriptComponent = entity.GetComponent<ScriptComponent>();

		scriptComponent.Scripts = component->Scripts;
		scriptComponent.isInherited = component->isInherited;
		scriptComponent.isInheritedInChildren = component->isInheritedInChildren;
		ConsolePanel::Log("File: Entity.cpp - Script component set successfully", ConsolePanel::LogType::Info);
	}

	template<>
	void Entity::SetComponent<NativeScriptComponent>(Entity& entity, NativeScriptComponent* component)
	{
		if (!entity.HasComponent<NativeScriptComponent>())
			entity.AddComponent<NativeScriptComponent>();

		auto& nativeScriptComponent = entity.GetComponent<NativeScriptComponent>();
		ConsolePanel::Log("File: Entity.cpp - Native Script component set successfully", ConsolePanel::LogType::Info);
	}

	template<>
	void Entity::SetComponent<CircleRendererComponent>(Entity& entity, CircleRendererComponent* component)
	{
		if (!entity.HasComponent<CircleRendererComponent>())
			entity.AddComponent<CircleRendererComponent>();

		auto& circleRendererComponent = entity.GetComponent<CircleRendererComponent>();

		circleRendererComponent.Color = component->Color;
		circleRendererComponent.Fade = component->Fade;
		circleRendererComponent.Thickness = component->Thickness;
		circleRendererComponent.isInherited = component->isInherited;
		circleRendererComponent.isInheritedInChildren = component->isInheritedInChildren;
		ConsolePanel::Log("File: Entity.cpp - Circle Renderer component set successfully", ConsolePanel::LogType::Info);
	}

	template<>
	void Entity::SetComponent<BoxCollider2DComponent>(Entity& entity, BoxCollider2DComponent* component)
	{
		if (!entity.HasComponent<BoxCollider2DComponent>())
			entity.AddComponent<BoxCollider2DComponent>();

		auto& boxCollider2DComponent = entity.GetComponent<BoxCollider2DComponent>();

		boxCollider2DComponent.Offset = component->Offset;
		boxCollider2DComponent.Size = component->Size;
		boxCollider2DComponent.Density = component->Density;
		boxCollider2DComponent.Friction = component->Friction;
		boxCollider2DComponent.Restitution = component->Restitution;
		boxCollider2DComponent.RestitutionThreshold = component->RestitutionThreshold;
		boxCollider2DComponent.isInherited = component->isInherited;
		boxCollider2DComponent.isInheritedInChildren = component->isInheritedInChildren;
		ConsolePanel::Log("File: Entity.cpp - Box Collider 2D component set successfully", ConsolePanel::LogType::Info);
	}

	template<>
	void Entity::SetComponent<CircleCollider2DComponent>(Entity& entity, CircleCollider2DComponent* component)
	{
		if (!entity.HasComponent<CircleCollider2DComponent>())
			entity.AddComponent<CircleCollider2DComponent>();

		auto& circleCollider2DComponent = entity.GetComponent<CircleCollider2DComponent>();

		circleCollider2DComponent.Offset = component->Offset;
		circleCollider2DComponent.Radius = component->Radius;
		circleCollider2DComponent.Density = component->Density;
		circleCollider2DComponent.Friction = component->Friction;
		circleCollider2DComponent.Restitution = component->Restitution;
		circleCollider2DComponent.RestitutionThreshold = component->RestitutionThreshold;
		circleCollider2DComponent.isInherited = component->isInherited;
		circleCollider2DComponent.isInheritedInChildren = component->isInheritedInChildren;
		ConsolePanel::Log("File: Entity.cpp - Circle Collider 2D component set successfully", ConsolePanel::LogType::Info);
	}

	template<>
	void Entity::SetComponent<RigidBody2DComponent>(Entity& entity, RigidBody2DComponent* component)
	{
		if (!entity.HasComponent<RigidBody2DComponent>())
			entity.AddComponent<RigidBody2DComponent>();

		auto& rigidBody2DComponent = entity.GetComponent<RigidBody2DComponent>();

		rigidBody2DComponent.Type = component->Type;
		rigidBody2DComponent.FixedRotation = component->FixedRotation;
		rigidBody2DComponent.isInherited = component->isInherited;
		rigidBody2DComponent.isInheritedInChildren = component->isInheritedInChildren;
		ConsolePanel::Log("File: Entity.cpp - Rigid Body 2D component set successfully", ConsolePanel::LogType::Info);
	}

	template<>
	void Entity::SetComponent<TextComponent>(Entity& entity, TextComponent* component)
	{
		if (!entity.HasComponent<TextComponent>())
			entity.AddComponent<TextComponent>();

		auto& textComponent = entity.GetComponent<TextComponent>();

		textComponent.Color = component->Color;
		textComponent.FontAsset = component->FontAsset;
		textComponent.Kerning = component->Kerning;
		textComponent.LineSpacing = component->LineSpacing;
		textComponent.TextString = component->TextString;
		textComponent.isInherited = component->isInherited;
		textComponent.isInheritedInChildren = component->isInheritedInChildren;
		ConsolePanel::Log("File: Entity.cpp - Text component set successfully", ConsolePanel::LogType::Info);
	}

	template<>
	void Entity::SetComponent<AudioSourceComponent>(Entity& entity, AudioSourceComponent* component)
	{
		if (!entity.HasComponent<AudioSourceComponent>())
			entity.AddComponent<AudioSourceComponent>();

		auto& audioSourceComponent = entity.GetComponent<AudioSourceComponent>();

		audioSourceComponent.Audio = component->Audio;
	}

	template<>
	void Entity::SetComponent<AnimatorComponent>(Entity& entity, AnimatorComponent* component)
	{
		if (!entity.HasComponent<AnimatorComponent>())
			entity.AddComponent<AnimatorComponent>();

		auto& animatorComponent = entity.GetComponent<AnimatorComponent>();

		animatorComponent.Animator2D = component->Animator2D;
		animatorComponent.isInherited = component->isInherited;
		animatorComponent.isInheritedInChildren = component->isInheritedInChildren;
	}
}