#include "Commands.h"

namespace eg
{
	Commands::Command* Commands::currentCommand;
	std::vector<Commands::Command*> Commands::commandHistory;
	int Commands::currentCommandIndex;

	void Commands::CreateEntityCommand::Execute(CommandArgs arg)
	{
		Entity e = m_Context->CreateEntity(arg.name);
		m_CreatedEntity = e;
	}

	void Commands::CreateEntityCommand::Undo()
	{
		if (m_CreatedEntity.HasComponent<TagComponent>())
		{
			if (m_SelectionContext == m_CreatedEntity)
				m_SelectionContext = {};
			m_Context->DestroyEntity(m_CreatedEntity);
			SetCurrentCommand(true);
		}
	}

	void Commands::DeleteEntityCommand::Execute(CommandArgs arg)
	{
		if (arg.entity.HasComponent<TagComponent>())
		{
			if (m_SelectionContext == arg.entity)
				m_SelectionContext = {};
			m_DeletedEntity = arg.entity.GetName();
			m_Context->DestroyEntity(arg.entity);
		}
	}

	void Commands::DeleteEntityCommand::Undo()
	{
		m_Context->CreateEntity(m_DeletedEntity);
		SetCurrentCommand(true);
	}

	void Commands::AddCommand(Command* command)
	{
		if (commandHistory.size() > 0 && currentCommandIndex != commandHistory.size() - 1)
			commandHistory.erase(commandHistory.begin() + currentCommandIndex + 1, commandHistory.end());

		commandHistory.push_back(command);
		currentCommandIndex = commandHistory.size() - 1;
		currentCommand = commandHistory.at(commandHistory.size() - 1);
	}

	Commands::Command* Commands::GetCurrentCommand()
	{
		return currentCommand;
	}

	void Commands::SetCurrentCommand(bool isUndo)
	{
		currentCommandIndex += currentCommandIndex == 0 ? 0 : isUndo ? -1 : 1;
		currentCommand = commandHistory.at(currentCommandIndex);
	}

	bool Commands::CanRevert(bool isUndo)
	{
		return isUndo ? currentCommandIndex >= 0 : currentCommandIndex < commandHistory.size() - 1;
	}
}