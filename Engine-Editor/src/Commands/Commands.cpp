#include "Commands.h"
#include "Engine/Core/Application.h"
namespace eg
{
	std::vector<Commands::Command*> Commands::commandHistory;
	int Commands::currentCommandIndex;
	bool isSaved = true;
	void SetIsSaved(bool val) {
		if (!isSaved == val) {
			Application::Get().ChangeNameWithCurrentProject(val);
			isSaved = val;
		}
	}
	bool GetIsSaved() {
		return isSaved;
	}

	void Commands::CreateEntityCommand::Execute(CommandArgs arg)
	{
		UUID e = m_Context->CreateEntity(arg.name).GetUUID();
		m_CreatedEntity = SavedEntity(arg.name, e);
	}

	void Commands::CreateEntityCommand::Undo()
	{
		Entity e = m_Context->GetEntityByUUID(m_CreatedEntity.uuid);
		if (e.HasComponent<TagComponent>())
		{
			if (m_SelectionContext == e)
				m_SelectionContext = {};
			m_Context->DestroyEntity(e);
			SetCurrentCommand(true);
		}
	}

	void Commands::CreateEntityCommand::Redo()
	{
		m_Context->CreateEntityWithID(m_CreatedEntity.uuid, m_CreatedEntity.name);

		SetCurrentCommand(false);
	}

	void Commands::DeleteEntityCommand::Execute(CommandArgs arg)
	{
		if (arg.entity.HasComponent<TagComponent>())
		{
			if (m_SelectionContext == arg.entity)
				m_SelectionContext = {};
			m_DeletedEntity = SavedEntity(arg.entity.GetName(), arg.entity.GetUUID());
			m_EntitySave = SaveEntity(arg.entity);
			m_Context->DestroyEntity(arg.entity);
		}
	}

	void Commands::DeleteEntityCommand::Undo()
	{
		Entity e = m_Context->CreateEntityWithID(m_DeletedEntity.uuid, m_DeletedEntity.name);
		RestoreEntity(e, m_EntitySave);
		SetCurrentCommand(true);
	}

	void Commands::DeleteEntityCommand::Redo()
	{
		Entity entity = m_Context->GetEntityByUUID(m_DeletedEntity.uuid);

		if (entity.HasComponent<TagComponent>())
		{
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
			m_DeletedEntity = { entity.GetName(), entity.GetUUID() };
			m_EntitySave = SaveEntity(entity);
			m_Context->DestroyEntity(entity);
		}

		SetCurrentCommand(false);
	}

	void Commands::AddCommand(Command* command)
	{
		if(currentCommandIndex == -1)
			commandHistory.clear();
		else if (commandHistory.size() > 0 && currentCommandIndex != commandHistory.size() - 1)
			commandHistory.erase(commandHistory.begin() + currentCommandIndex + 1, commandHistory.end());

		commandHistory.push_back(command);
		currentCommandIndex = commandHistory.size() - 1;
		SetIsSaved(false);
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
		SetIsSaved(false);
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

		entity.GetComponent<TagComponent>().Tag = component->Tag;
	}

	template<>
	void Commands::SetComponent<TransformComponent>(Entity& entity, TransformComponent* component)
	{
		if (!entity.HasComponent<TransformComponent>())
			entity.AddComponent<TransformComponent>();

		entity.GetComponent<TransformComponent>().Translation = component->Translation;
		entity.GetComponent<TransformComponent>().Rotation = component->Rotation;
		entity.GetComponent<TransformComponent>().Scale = component->Scale;
	}

	template<>
	void Commands::SetComponent<SpriteRendererComponent>(Entity& entity, SpriteRendererComponent* component)
	{
		if (!entity.HasComponent<SpriteRendererComponent>())
			entity.AddComponent<SpriteRendererComponent>();

		entity.GetComponent<SpriteRendererComponent>().Color = component->Color;
		entity.GetComponent<SpriteRendererComponent>().Texture = component->Texture;
		entity.GetComponent<SpriteRendererComponent>().TilingFactor = component->TilingFactor;
	}

	template<>
	void Commands::SetComponent<CameraComponent>(Entity& entity, CameraComponent* component)
	{
		if (!entity.HasComponent<CameraComponent>())
			entity.AddComponent<CameraComponent>();

		entity.GetComponent<CameraComponent>().Primary = component->Primary;
		entity.GetComponent<CameraComponent>().FixedAspectRatio = component->FixedAspectRatio;
		entity.GetComponent<CameraComponent>().Camera.SetProjectionType(component->Camera.GetProjectionType());
		entity.GetComponent<CameraComponent>().Camera.SetPerspective(component->Camera.GetPerspectiveVerticalFOV(), component->Camera.GetPerspectiveNearClip(), component->Camera.GetPerspectiveFarClip());
		entity.GetComponent<CameraComponent>().Camera.SetOrthographic(component->Camera.GetOrthographicSize(), component->Camera.GetOrthographicNearClip(), component->Camera.GetOrthographicFarClip());
	}

	template<>
	void Commands::SetComponent<ScriptComponent>(Entity& entity, ScriptComponent* component)
	{
		if (!entity.HasComponent<ScriptComponent>())
			entity.AddComponent<ScriptComponent>();

		entity.GetComponent<ScriptComponent>().Name = component->Name;
	}

	template<>
	void Commands::SetComponent<CircleRendererComponent>(Entity& entity, CircleRendererComponent* component)
	{
		if (!entity.HasComponent<CircleRendererComponent>())
			entity.AddComponent<CircleRendererComponent>();

		entity.GetComponent<CircleRendererComponent>().Color = component->Color;
		entity.GetComponent<CircleRendererComponent>().Fade = component->Fade;
		entity.GetComponent<CircleRendererComponent>().Thickness = component->Thickness;
	}

	template<>
	void Commands::SetComponent<BoxCollider2DComponent>(Entity& entity, BoxCollider2DComponent* component)
	{
		if (!entity.HasComponent<BoxCollider2DComponent>())
			entity.AddComponent<BoxCollider2DComponent>();

		entity.GetComponent<BoxCollider2DComponent>().Offset = component->Offset;
		entity.GetComponent<BoxCollider2DComponent>().Size = component->Size;
		entity.GetComponent<BoxCollider2DComponent>().Density = component->Density;
		entity.GetComponent<BoxCollider2DComponent>().Friction = component->Friction;
		entity.GetComponent<BoxCollider2DComponent>().Restitution = component->Restitution;
		entity.GetComponent<BoxCollider2DComponent>().RestitutionThreshold = component->RestitutionThreshold;
	}

	template<>
	void Commands::SetComponent<CircleCollider2DComponent>(Entity& entity, CircleCollider2DComponent* component)
	{
		if (!entity.HasComponent<CircleCollider2DComponent>())
			entity.AddComponent<CircleCollider2DComponent>();

		entity.GetComponent<CircleCollider2DComponent>().Offset = component->Offset;
		entity.GetComponent<CircleCollider2DComponent>().Radius = component->Radius;
		entity.GetComponent<CircleCollider2DComponent>().Density = component->Density;
		entity.GetComponent<CircleCollider2DComponent>().Friction = component->Friction;
		entity.GetComponent<CircleCollider2DComponent>().Restitution = component->Restitution;
		entity.GetComponent<CircleCollider2DComponent>().RestitutionThreshold = component->RestitutionThreshold;
	}

	template<>
	void Commands::SetComponent<RigidBody2DComponent>(Entity& entity, RigidBody2DComponent* component)
	{
		if (!entity.HasComponent<RigidBody2DComponent>())
			entity.AddComponent<RigidBody2DComponent>();

		entity.GetComponent<RigidBody2DComponent>().Type = component->Type;
		entity.GetComponent<RigidBody2DComponent>().FixedRotation = component->FixedRotation;
	}

	template<>
	void Commands::SetComponent<TextComponent>(Entity& entity, TextComponent* component)
	{
		if (!entity.HasComponent<TextComponent>())
			entity.AddComponent<TextComponent>();

		entity.GetComponent<TextComponent>().Color = component->Color;
		entity.GetComponent<TextComponent>().FontAsset = component->FontAsset;
		entity.GetComponent<TextComponent>().Kerning = component->Kerning;
		entity.GetComponent<TextComponent>().LineSpacing = component->LineSpacing;
		entity.GetComponent<TextComponent>().TextString = component->TextString;
	
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