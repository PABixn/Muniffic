#include "Commands.h"
#include "Engine/Core/Application.h"
namespace eg
{
	std::vector<Commands::Command*> Commands::commandHistory;
	int Commands::currentCommandIndex;
	int LastSavedCommandIndex = -1;
	bool isSaved = true;
	void SetIsSaved(bool val) {
		if (!isSaved == val) {
			Application::Get().ChangeNameWithCurrentProject(val);
			isSaved = val;
		}
		if (val)LastSavedCommandIndex = Commands::currentCommandIndex;
	}
	bool GetIsSaved() {
		return isSaved;
	}

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
		if (LastSavedCommandIndex == currentCommandIndex) {
			SetIsSaved(true);
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

	void Commands::DeleteResourceCommand::Undo()
	{
		if(m_ResourceType == ResourceType::Image)
			ResourceSerializer::TextureResourceDataCache[m_KeyPath] = (TextureResourceData*)m_Resource;

		SetCurrentCommand(true);
	}

	void Commands::DeleteResourceCommand::Redo()
	{
		ResourceSerializer::DeleteCachedResource(m_KeyPath, m_ResourceType, m_DeleteFile);

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
		if (isUndo && (LastSavedCommandIndex == currentCommandIndex)) {
			SetIsSaved(true);
		}
		else SetIsSaved(false);
	}

	bool Commands::CanRevert(bool isUndo)
	{
		return isUndo ? currentCommandIndex >= 0 : currentCommandIndex < static_cast<int>(commandHistory.size()) - 1;
	}

	template<typename T>
	void TrySetComponent(Entity& entity, std::optional<T>* component)
	{
		if (component->has_value())
			Entity::SetComponent(entity, &component->value());
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