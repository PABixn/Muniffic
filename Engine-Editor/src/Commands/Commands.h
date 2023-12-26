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
			EntityCommand(Ref<Scene>& context, Entity& selectionContext)
				: m_Context(context), m_SelectionContext(selectionContext) {  }

			virtual void Execute(CommandArgs) = 0;

			virtual void Undo() = 0;

			~EntityCommand() override
			{
				delete& m_Context;
			}

		protected:
			Ref<Scene>& m_Context;
			Entity& m_SelectionContext;
		};

		class CreateEntityCommand : public EntityCommand
		{
		public:
			CreateEntityCommand(Ref<Scene>& context, Entity& selectionContext)
				: EntityCommand(context, selectionContext) {}

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
			DeleteEntityCommand(Ref<Scene>& context, Entity& selectionContext)
				: EntityCommand(context, selectionContext) {}

			void Execute(CommandArgs arg) override;

			void Undo() override;

			~DeleteEntityCommand()
			{
				delete& m_DeletedEntity;
				delete& m_SelectionContext;
			}

		protected:
			std::string m_DeletedEntity;
		};

		static void SetCurrentCommand(bool isUndo);
		static void AddCommand(Command* command);
		static bool CanRevert(bool isUndo);
		static Command* GetCurrentCommand();
		static Command* currentCommand;
		static int currentCommandIndex;

	private:
		static std::vector<Commands::Command*> commandHistory;
	};
}
