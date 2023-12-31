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
}