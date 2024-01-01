#include "Commands.h"

namespace eg
{
	std::vector<Commands::Command*> Commands::commandHistory;
	int Commands::currentCommandIndex;

	void Commands::CreateEntityCommand::Execute(CommandArgs arg)
	{
		UUID e = m_Context->CreateEntity(arg.name).GetUUID();
		m_CreatedEntity = e;
	}

	void Commands::CreateEntityCommand::Undo()
	{
		Entity e = m_Context->GetEntityByUUID(m_CreatedEntity);
		if (e.HasComponent<TagComponent>())
		{
			if (m_SelectionContext == e)
				m_SelectionContext = {};
			m_Context->DestroyEntity(e);
			SetCurrentCommand(true);
		}
	}

	void Commands::DeleteEntityCommand::Execute(CommandArgs arg)
	{
		if (arg.entity.HasComponent<TagComponent>())
		{
			if (m_SelectionContext == arg.entity)
				m_SelectionContext = {};
			m_DeletedEntity = { arg.entity.GetName(), arg.entity.GetUUID() };
			m_Context->DestroyEntity(arg.entity);
		}
	}

	void Commands::DeleteEntityCommand::Undo()
	{
		m_Context->CreateEntityWithID(m_DeletedEntity.uuid, m_DeletedEntity.name);
		SetCurrentCommand(true);
	}

	void Commands::AddCommand(Command* command)
	{
		if(currentCommandIndex == -1)
			commandHistory.clear();
		else if (commandHistory.size() > 0 && currentCommandIndex != commandHistory.size() - 1)
			commandHistory.erase(commandHistory.begin() + currentCommandIndex + 1, commandHistory.end());

		commandHistory.push_back(command);
		currentCommandIndex = commandHistory.size() - 1;
	}

	Commands::Command* Commands::GetCurrentCommand(int offset)
	{
		offset = offset < 0 ? 0 : offset;
		if (currentCommandIndex - offset >= 0)
			return commandHistory[currentCommandIndex];
		else
			return nullptr;
	}

	void Commands::SetCurrentCommand(bool isUndo)
	{
		currentCommandIndex += isUndo ? -1 : 1;
	}

	bool Commands::CanRevert(bool isUndo)
	{
		return isUndo ? currentCommandIndex >= 0 : currentCommandIndex < commandHistory.size() - 1;
	}

	template<>
	void Commands::SetComponent<TagComponent>(Entity& entity, TagComponent* component)
	{
		entity.GetComponent<TagComponent>().Tag = component->Tag;
	}

	template<>
	void Commands::SetComponent<TransformComponent>(Entity& entity, TransformComponent* component)
	{
		entity.GetComponent<TransformComponent>().Translation = component->Translation;
		entity.GetComponent<TransformComponent>().Rotation = component->Rotation;
		entity.GetComponent<TransformComponent>().Scale = component->Scale;
	}

	template<>
	void Commands::SetComponent<SpriteRendererComponent>(Entity& entity, SpriteRendererComponent* component)
	{
		entity.GetComponent<SpriteRendererComponent>().Color = component->Color;
		entity.GetComponent<SpriteRendererComponent>().Texture = component->Texture;
		entity.GetComponent<SpriteRendererComponent>().TilingFactor = component->TilingFactor;
	}

	template<>
	void Commands::SetComponent<CameraComponent>(Entity& entity, CameraComponent* component)
	{
		entity.GetComponent<CameraComponent>().Primary = component->Primary;
		entity.GetComponent<CameraComponent>().FixedAspectRatio = component->FixedAspectRatio;
		entity.GetComponent<CameraComponent>().Camera.SetProjectionType(component->Camera.GetProjectionType());
		entity.GetComponent<CameraComponent>().Camera.SetPerspective(component->Camera.GetPerspectiveVerticalFOV(), component->Camera.GetPerspectiveNearClip(), component->Camera.GetPerspectiveFarClip());
		entity.GetComponent<CameraComponent>().Camera.SetOrthographic(component->Camera.GetOrthographicSize(), component->Camera.GetOrthographicNearClip(), component->Camera.GetOrthographicFarClip());
	}

	template<>
	void Commands::SetComponent<ScriptComponent>(Entity& entity, ScriptComponent* component)
	{
		entity.GetComponent<ScriptComponent>().Name = component->Name;
	}

	template<>
	void Commands::SetComponent<CircleRendererComponent>(Entity& entity, CircleRendererComponent* component)
	{
		entity.GetComponent<CircleRendererComponent>().Color = component->Color;
		entity.GetComponent<CircleRendererComponent>().Fade = component->Fade;
		entity.GetComponent<CircleRendererComponent>().Thickness = component->Thickness;
	}

	template<>
	void Commands::SetComponent<BoxCollider2DComponent>(Entity& entity, BoxCollider2DComponent* component)
	{
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
		entity.GetComponent<RigidBody2DComponent>().Type = component->Type;
		entity.GetComponent<RigidBody2DComponent>().FixedRotation = component->FixedRotation;
	}

}