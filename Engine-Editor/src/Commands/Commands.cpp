#include "Commands.h"

namespace eg
{
	void CreateEntityCommand::Execute(const std::string& name)
	{
		Entity e = m_Context->CreateEntity(name);
		m_CreatedEntity = e;
	}

	void DeleteEntityCommand::Execute(Entity entity)
	{
		if (m_SelectionContext == entity)
			m_SelectionContext = {};
		m_Context->DestroyEntity(entity);
		m_DeletedEntity = entity;
	}


}