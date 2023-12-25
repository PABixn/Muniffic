#pragma once
#include <string>
#include "Engine.h"

namespace eg
{
	template<typename param>
	class Command
	{
	public:
		virtual void Execute(param) = 0;
	};


	template<typename T>
	class EntityCommand : public Command<T>
	{
	public:
		EntityCommand(Ref<Scene>& context)
			: m_Context(context) {}

	protected:
		Ref<Scene>& m_Context;
	};

	class CreateEntityCommand : public EntityCommand<const std::string&>
	{
	public:
		CreateEntityCommand(Ref<Scene>& context)
			: EntityCommand(context) {}

		void Execute(const std::string& name) override;

	protected:
		Entity m_CreatedEntity;
	};

	class DeleteEntityCommand : public EntityCommand<Entity>
	{
	public:
		DeleteEntityCommand(Ref<Scene>& context, Entity& m_SelectionContext)
			: EntityCommand(context), m_SelectionContext(m_SelectionContext) {}

		void Execute(Entity entity) override;

	protected:
		Entity& m_SelectionContext;
		Entity m_DeletedEntity;
	};
}