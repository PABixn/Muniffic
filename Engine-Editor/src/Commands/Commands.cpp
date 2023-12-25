#include "Commands.h"

namespace eg
{
	Commands::Command* Commands::currentCommand;
	std::vector<Commands::Command*> Commands::commandHistory;

	void Commands::CreateEntityCommand::Execute(CommandArgs arg)
	{
		Entity e = m_Context->CreateEntity(arg.name);
		m_CreatedEntity = e;
	}

	void Commands::CreateEntityCommand::Undo()
	{
		m_Context->DestroyEntity(m_CreatedEntity);
	}

	void Commands::DeleteEntityCommand::Execute(CommandArgs arg)
	{
		if (m_SelectionContext == arg.entity)
			m_SelectionContext = {};
		m_Context->DestroyEntity(arg.entity);
		m_DeletedEntity = arg.entity;
	}

	void Commands::DeleteEntityCommand::Undo()
	{
		m_Context->CreateEntity(m_DeletedEntity.GetName());
	}

	void Commands::AddCommand(Command* command)
	{
		commandHistory.push_back(command);
		currentCommand = commandHistory.at(commandHistory.size() - 1);
	}

	Commands::Command* Commands::GetCurrentCommand()
	{
		return currentCommand;
	}
}