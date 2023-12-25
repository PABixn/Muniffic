#pragma once
#include <string>
#include "Engine.h"
#include <variant>

namespace eg
{
	class Commands
	{
	public:
		struct CommandArgs
		{
		public:
			const std::string& name;
			Entity entity;
		};

		class Command
		{
		public:
			virtual ~Command() = 0 {};
			virtual void Undo() = 0;
			virtual void Execute(CommandArgs) = 0;
		};


		class EntityCommand : public Command
		{
		public:
			EntityCommand(Ref<Scene>& context)
				: m_Context(context) {  }

			virtual void Execute(CommandArgs) = 0;

			virtual void Undo() = 0;

			~EntityCommand() override
			{
				delete& m_Context;
			}

		protected:
			Ref<Scene>& m_Context;
		};

		class CreateEntityCommand : public EntityCommand
		{
		public:
			CreateEntityCommand(Ref<Scene>& context)
				: EntityCommand(context) {}

			void Execute(CommandArgs arg) override;

			void Undo() override;

			~CreateEntityCommand()
			{
				delete& m_CreatedEntity;
			}

		protected:
			Entity m_CreatedEntity;	
		};


		class DeleteEntityCommand : public EntityCommand
		{
		public:
			DeleteEntityCommand(Ref<Scene>& context, Entity& m_SelectionContext)
				: EntityCommand(context), m_SelectionContext(m_SelectionContext) {}

			void Execute(CommandArgs arg) override;

			void Undo() override;

			~DeleteEntityCommand()
			{
				delete& m_DeletedEntity;
				delete& m_SelectionContext;
			}

		protected:
			Entity& m_SelectionContext;
			Entity m_DeletedEntity;
		};

		static void AddCommand(Command* command);
		static Command* GetCurrentCommand();
		static Command* currentCommand;

	private:
		static std::vector<Commands::Command*> commandHistory;
	};
}
