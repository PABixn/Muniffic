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
			CommandArgs(const std::string& name, Entity entity,
			Ref<Scene>& context,
			Entity& selectionContext)
				: name(name), entity(entity), m_Context(context), selectionContext(selectionContext) {  }

			const std::string& name;
			Entity entity;
			Ref<Scene>& m_Context;
			Entity& selectionContext;
		};

		struct SavedEntity
		{
		public:
			std::string name;
			UUID uuid;
		};

		class Command
		{
		public:
			virtual void Undo() = 0;
			virtual void Execute(CommandArgs) = 0;
		};

		template<typename T>
		class ChangeRawValueCommand : public Command
		{
		public:
			ChangeRawValueCommand(T* value_ptr, T previousValue, const std::string label)
				: m_ValuePtr(value_ptr), m_PreviousValue(previousValue), m_Label(label)
			{
				Commands::AddCommand(this);
			}

			void Execute(CommandArgs args) override {};

			void Undo() override
			{
				*m_ValuePtr = m_PreviousValue;
				SetCurrentCommand(true);
			}

			const std::string GetLabel() const { return m_Label; }

		protected:
			T* m_ValuePtr;
			T m_PreviousValue;
			const std::string m_Label;
		};

		class ComponentCommand : public Command
		{
		public:
			ComponentCommand(Ref<Scene>& m_Context, Entity& selectionContext)
			: m_SelectionContext(selectionContext) {  }

		protected:
			Entity m_SelectionContext;
		};

		template<typename T>
		class AddComponentCommand : public ComponentCommand
		{
		public:
			AddComponentCommand(Ref<Scene>& m_Context, Entity& selectionContext)
				: ComponentCommand(m_Context, selectionContext)
			{
				Commands::AddCommand(this);
			}

			void Execute(CommandArgs args) override 
			{
				if(!m_SelectionContext.HasComponent<T>())
					m_SelectionContext.AddComponent<T>();
			}

			void Undo() override
			{
				if (m_SelectionContext.HasComponent<T>())
				{
					m_SelectionContext.RemoveComponent<T>();
					SetCurrentCommand(true);
				}
			}
		};

		template<typename T>
		class RemoveComponentCommand : public ComponentCommand
		{
		public:
			RemoveComponentCommand(Ref<Scene>& m_Context, Entity& selectionContext)
				: ComponentCommand(m_Context, selectionContext)
			{
				Commands::AddCommand(this);
			}

			void Execute(CommandArgs args) override
			{
				if (m_SelectionContext.HasComponent<T>())
				{
					m_SelectionContext.RemoveComponent<T>();
				}
			}

			void Undo() override
			{
				if (!m_SelectionContext.HasComponent<T>())
				{
					m_SelectionContext.AddComponent<T>();
					SetCurrentCommand(true);
				}
			}
		};

		class EntityCommand : public Command
		{
		public:
			EntityCommand(Ref<Scene>& context, Entity& selectionContext)
				: m_Context(context), m_SelectionContext(selectionContext) {  }

			virtual void Execute(CommandArgs) = 0;

			virtual void Undo() = 0;

		protected:
			Ref<Scene>& m_Context;
			Entity& m_SelectionContext;
		};

		class CreateEntityCommand : public EntityCommand
		{
		public:
			CreateEntityCommand(Ref<Scene>& context, Entity& selectionContext)
				: EntityCommand(context, selectionContext)
			{
				Commands::AddCommand(this);
			}

			void Execute(CommandArgs arg) override;

			void Undo() override;

		protected:
			UUID m_CreatedEntity;	
		};


		class DeleteEntityCommand : public EntityCommand
		{
		public:
			DeleteEntityCommand(Ref<Scene>& context, Entity& selectionContext)
				: EntityCommand(context, selectionContext)
			{
				Commands::AddCommand(this);
			}

			void Execute(CommandArgs arg) override;

			void Undo() override;

		protected:
			SavedEntity m_DeletedEntity;
		};

		static void SetCurrentCommand(bool isUndo);
		static void AddCommand(Command* command);
		static bool CanRevert(bool isUndo);
		static Command* GetCurrentCommand(int offset = 0);
		static int currentCommandIndex;

		template<typename T>
		static Command* ExecuteRawValueCommand(T* value_ptr, T previousValue, const std::string label)
		{
			Command* command = nullptr;
			ChangeRawValueCommand<T>* previousCommand = dynamic_cast<ChangeRawValueCommand<T>*>(GetCurrentCommand(1));
			if (previousCommand == nullptr || previousCommand->GetLabel() != label)
				Command* command = new ChangeRawValueCommand<T>(value_ptr, previousValue, label);

			return command;
		}

		template<typename T>
		static Command* ExecuteCommand(CommandArgs args)
		{
			Command* command = new T(args.m_Context, args.selectionContext);
			command->Execute(args);
			return command;
		}

	private:
		static std::vector<Commands::Command*> commandHistory;
	};
}
