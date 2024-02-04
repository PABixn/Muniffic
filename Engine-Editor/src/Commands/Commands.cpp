#include "Commands.h"

namespace eg
{
	std::vector<Commands::Command*> Commands::commandHistory;
	int Commands::currentCommandIndex;

	void Commands::CreateEntityCommand::Execute(CommandArgs arg)
	{
		Entity e = m_Context->CreateEntity(arg.m_Name);

		if (arg.m_Parent.has_value())
			ChangeParent(e, arg.m_Parent);

		m_CreatedEntity = SaveEntity(e);
	}

	void Commands::CreateEntityCommand::Undo()
	{
		Entity e = m_Context->GetEntityByUUID(m_CreatedEntity.m_UUID);

		if (e.Exists())
		{
			if (m_SelectionContext == e)
				m_SelectionContext = {};
			m_Context->DestroyEntity(e);
			SetCurrentCommand(true);
		}
	}

	void Commands::CreateEntityCommand::Redo()
	{
		m_Context->CreateEntityWithID(m_CreatedEntity.m_UUID, m_CreatedEntity.m_Name);

		SetCurrentCommand(false);
	}

	void Commands::DeleteEntityCommand::Execute(CommandArgs arg)
	{
		if (arg.m_Entity.Exists())
		{
			if (m_SelectionContext == arg.m_Entity)
				m_SelectionContext = {};

			m_DeletedEntity = SaveEntity(arg.m_Entity);

			for (auto& child : arg.m_Entity.GetAnyChildren())
			{
				EntitySave saved = SaveEntity(child);
				m_Children.push_back(saved);
			}

			arg.m_Entity.RemoveAnyChildren();

			m_Context->DestroyEntity(arg.m_Entity);
		}
	}

	void Commands::DeleteEntityCommand::Undo()
	{
		Entity e = m_Context->CreateEntityWithID(m_DeletedEntity.m_UUID, m_DeletedEntity.m_Name);
		RestoreEntity(e, m_DeletedEntity);
		
		if (m_DeletedEntity.m_Parent != NULL)
		{
			Entity parent = m_Context->GetEntityByUUID(m_DeletedEntity.m_Parent);
			ChangeParent(e, parent);
		}
		else
			ChangeParent(e, std::nullopt);

		for (auto& child : m_Children)
		{
			Entity childrenEntity = m_Context->CreateEntityWithID(child.m_UUID, child.m_Name);
			RestoreEntity(childrenEntity, child);
			if (child.m_Parent != NULL)
			{
				Entity parent = m_Context->GetEntityByUUID(child.m_Parent);
				ChangeParent(childrenEntity, parent);
			}
			else
				ChangeParent(childrenEntity, std::nullopt);
		}

		SetCurrentCommand(true);
	}

	void Commands::DeleteEntityCommand::Redo()
	{
		Entity entity = m_Context->GetEntityByUUID(m_DeletedEntity.m_UUID);

		if (entity.Exists())
		{
			if (m_SelectionContext == entity)
				m_SelectionContext = {};

			m_DeletedEntity = SaveEntity(entity);

			for (auto& child : entity.GetAnyChildren())
			{
				EntitySave saved = SaveEntity(child);
				m_Children.push_back(saved);
				m_Context->DestroyEntity(child);
			}

			m_Context->DestroyEntity(entity);
		}

		SetCurrentCommand(false);
	}

	void Commands::ChangeParent(Entity& entity, std::optional<Entity> parent)
	{
		if (parent.has_value())
		{
			Entity parentEntity = parent.value();
			if (entity != parentEntity && !parentEntity.IsChild(entity) && !entity.IsChildOfAny(parentEntity))
			{
				parentEntity.AddChild(entity);
				if (entity.GetParent().has_value())
					entity.GetParent().value().RemoveChild(entity);
				entity.SetParent(parentEntity);
			}
		}
		else
		{
			if (entity.GetParent().has_value())
				entity.GetParent().value().RemoveChild(entity);
			entity.SetParent(std::nullopt);
		}

		Commands::SetInheritedComponents(InheritableComponents{}, entity, parent);
	}

	void Commands::ChangeParentCommand::Undo()
	{
		Entity entity = m_Context->GetEntityByUUID(m_Entity);
        std::optional<Entity> previousParent = m_PreviousParent.has_value() ? std::optional<Entity>(m_Context->GetEntityByUUID(m_PreviousParent.value())) : std::nullopt;

		ChangeParent(entity, previousParent);

		SetCurrentCommand(true);
	}

	void Commands::ChangeParentCommand::Redo()
	{
		Entity entity = m_Context->GetEntityByUUID(m_Entity);
		std::optional<Entity> parent = m_Parent.has_value() ? std::optional<Entity>(m_Context->GetEntityByUUID(m_Parent.value())) : std::nullopt;

		ChangeParent(entity, parent);

		SetCurrentCommand(false);
	}

	void Commands::AddCommand(Command* command)
	{
		if(currentCommandIndex == -1)
			commandHistory.clear();
		else if (commandHistory.size() > 0 && currentCommandIndex != commandHistory.size() - 1)
			commandHistory.erase(commandHistory.begin() + currentCommandIndex + 1, commandHistory.end());

		if (commandHistory.size() >= Commands::MAX_COMMANDS)
		{
			commandHistory.erase(commandHistory.begin());
		}

		commandHistory.push_back(command);
		currentCommandIndex = commandHistory.size() - 1;
	}

	Commands::Command* Commands::GetCurrentCommand(int offset)
	{
		if (currentCommandIndex + offset >= 0 && currentCommandIndex + offset < static_cast<int>(commandHistory.size()))
			return commandHistory[currentCommandIndex + offset];
		else
			return nullptr;
	}

	void Commands::SetCurrentCommand(bool isUndo)
	{
		currentCommandIndex += isUndo ? -1 : 1;
	}

	bool Commands::CanRevert(bool isUndo)
	{
		return isUndo ? currentCommandIndex >= 0 : currentCommandIndex < static_cast<int>(commandHistory.size()) - 1;
	}

	template<>
	void Commands::SetComponent<TagComponent>(Entity& entity, TagComponent* component)
	{
		if (!entity.HasComponent<TagComponent>())
			entity.AddComponent<TagComponent>();

		auto& tagComponent = entity.GetComponent<TagComponent>();

		tagComponent.Tag = component->Tag;
	}

	template<>
	void Commands::SetComponent<TransformComponent>(Entity& entity, TransformComponent* component)
	{
		if (!entity.HasComponent<TransformComponent>())
			entity.AddComponent<TransformComponent>();

		auto& transformComponent = entity.GetComponent<TransformComponent>();

		transformComponent.Translation = component->Translation;
		transformComponent.Rotation = component->Rotation;
		transformComponent.Scale = component->Scale;
		transformComponent.isInherited = component->isInherited;
		transformComponent.isInheritedInChildren = component->isInheritedInChildren;
	}

	template<>
	void Commands::SetComponent<SpriteRendererComponent>(Entity& entity, SpriteRendererComponent* component)
	{
		if (!entity.HasComponent<SpriteRendererComponent>())
			entity.AddComponent<SpriteRendererComponent>();

		auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();

		spriteRendererComponent.Color = component->Color;
		spriteRendererComponent.Texture = component->Texture;
		spriteRendererComponent.TilingFactor = component->TilingFactor;
		spriteRendererComponent.isInherited = component->isInherited;
		spriteRendererComponent.isInheritedInChildren = component->isInheritedInChildren;
	}

	template<>
	void Commands::SetComponent<CameraComponent>(Entity& entity, CameraComponent* component)
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
	}

	template<>
	void Commands::SetComponent<ScriptComponent>(Entity& entity, ScriptComponent* component)
	{
		if (!entity.HasComponent<ScriptComponent>())
			entity.AddComponent<ScriptComponent>();

		auto& scriptComponent = entity.GetComponent<ScriptComponent>();

		scriptComponent.Name = component->Name;
		scriptComponent.isInherited = component->isInherited;
		scriptComponent.isInheritedInChildren = component->isInheritedInChildren;
	}

	template<>
	void Commands::SetComponent<CircleRendererComponent>(Entity& entity, CircleRendererComponent* component)
	{
		if (!entity.HasComponent<CircleRendererComponent>())
			entity.AddComponent<CircleRendererComponent>();

		auto& circleRendererComponent = entity.GetComponent<CircleRendererComponent>();

		circleRendererComponent.Color = component->Color;
		circleRendererComponent.Fade = component->Fade;
		circleRendererComponent.Thickness = component->Thickness;
		circleRendererComponent.isInherited = component->isInherited;
		circleRendererComponent.isInheritedInChildren = component->isInheritedInChildren;
	}

	template<>
	void Commands::SetComponent<BoxCollider2DComponent>(Entity& entity, BoxCollider2DComponent* component)
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
	}

	template<>
	void Commands::SetComponent<CircleCollider2DComponent>(Entity& entity, CircleCollider2DComponent* component)
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
	}

	template<>
	void Commands::SetComponent<RigidBody2DComponent>(Entity& entity, RigidBody2DComponent* component)
	{
		if (!entity.HasComponent<RigidBody2DComponent>())
			entity.AddComponent<RigidBody2DComponent>();

		auto& rigidBody2DComponent = entity.GetComponent<RigidBody2DComponent>();

		rigidBody2DComponent.Type = component->Type;
		rigidBody2DComponent.FixedRotation = component->FixedRotation;
		rigidBody2DComponent.isInherited = component->isInherited;
		rigidBody2DComponent.isInheritedInChildren = component->isInheritedInChildren;
	}

	template<>
	void Commands::SetComponent<TextComponent>(Entity& entity, TextComponent* component)
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
	}

	template<typename T>
	void TrySetComponent(Entity& entity, std::optional<T>* component)
	{
		if (component->has_value())
			Commands::SetComponent(entity, &component->value());
	}

	template<typename T>
	void TrySaveComponent(Entity& entity, Commands::EntitySave& entitySave, std::optional<T>& component)
	{
		using type = std::optional<T>::value_type;
		if (entity.HasComponent<type>())
			entitySave.SaveComponent<type>(entity.GetComponent<type>());
	}

	Commands::EntitySave Commands::SaveEntity(Entity& entity)
	{
		EntitySave entitySave;

		entitySave.m_UUID = entity.GetUUID();
		entitySave.m_Name = entity.GetName();
		entitySave.m_Parent = entity.GetParentUUID();

		Commands::AllSavedComponents components = entitySave.GetAllComponents();

		std::apply([&entity, &entitySave](auto&&... args) {((TrySaveComponent(entity, entitySave, args)), ...); }, components);

		return entitySave;
	}

	void Commands::RestoreEntity(Entity& entity, EntitySave& entitySave)
	{

		Commands::AllSavedComponents components = entitySave.GetAllComponents();

		std::apply([&entity](auto&&... args) {(( TrySetComponent(entity, &args)), ...); }, components);
	}
}