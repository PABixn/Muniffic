#pragma once
#include <string>
#include "Engine.h"
#include <variant>
#include <cstddef>

namespace eg
{
	class Commands
	{
		const static int MAX_COMMANDS = 200;

		using AllSavedComponents = std::tuple<std::optional<TagComponent>,
			std::optional<TransformComponent>,
			std::optional<SpriteRendererComponent>,
			std::optional<CircleRendererComponent>,
			std::optional<CameraComponent>,
			std::optional<ScriptComponent>,
			std::optional<RigidBody2DComponent>,
			std::optional<BoxCollider2DComponent>,
			std::optional<CircleCollider2DComponent>,
			std::optional<TextComponent>>;

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
			SavedEntity(const std::string& name, UUID uuid)
				: name(name), uuid(uuid) {  }

			SavedEntity() = default;

			std::string name;
			UUID uuid;
		};

		struct EntitySave
		{
			AllSavedComponents components;

			AllSavedComponents GetAllComponents()
			{
				return std::make_tuple(GetComponent<TagComponent>(),
					GetComponent<TransformComponent>(),
					GetComponent<SpriteRendererComponent>(),
					GetComponent<CircleRendererComponent>(),
					GetComponent<CameraComponent>(),
					GetComponent<ScriptComponent>(),
					GetComponent<RigidBody2DComponent>(),
					GetComponent<BoxCollider2DComponent>(),
					GetComponent<CircleCollider2DComponent>(),
					GetComponent<TextComponent>());
			}

			template<typename T>
			void SaveComponent(T& component)
			{
				std::get<std::optional<T>>(components) = component;
			}

			template <typename T>
			std::optional<T> GetComponent()
			{
				return std::get<std::optional<T>>(components);
			}
		};

		class Command
		{
		public:
			virtual void Undo() = 0;
			virtual void Execute(CommandArgs) = 0;
			virtual void Redo() = 0;
		};

		template<typename T>
		class ChangeValueCommand : public Command
		{
		public:
			ChangeValueCommand(std::function<void(T)> function, T value, T previousValue, const std::string label)
				: m_PreviousValue(previousValue), m_Label(label), m_Function(function), m_Value(value)
			{
				Commands::AddCommand(this);
			}

			void Execute(CommandArgs args) override {};

			void Undo() override
			{
				m_Function(m_PreviousValue);
				SetCurrentCommand(true);
			};

			void Redo() override
			{
				m_Function(m_Value);
				SetCurrentCommand(false);
			};

			const std::string GetLabel() const { return m_Label; }

		protected:
			T m_Value;
			T m_PreviousValue;
			const std::string m_Label;
			std::function<void(T)> m_Function;
		};

		template<typename T>
		class ChangeRawValueCommand : public Command
		{
		public:
			ChangeRawValueCommand(T* value_ptr, T previousValue, const std::string label)
				: m_ValuePtr(value_ptr), m_PreviousValue(previousValue), m_Label(label), m_Value(*value_ptr)
			{
				Commands::AddCommand(this);
			}

			void Execute(CommandArgs args) override {};

			void Undo() override
			{
				m_Value = *m_ValuePtr;
				*m_ValuePtr = m_PreviousValue;
				
				SetCurrentCommand(true);
			}

			void Redo() override
			{
				*m_ValuePtr = m_Value;

				SetCurrentCommand(false);
			}

			const std::string GetLabel() const { return m_Label; }

		protected:
			T* m_ValuePtr;
			T m_Value;
			T m_PreviousValue;
			const std::string m_Label;
		};

		template<typename T, typename Component>
		class ChangeRawComponentValueCommand : public Command
		{
		public:
			ChangeRawComponentValueCommand(T* value_ptr, T previousValue, Entity entity, const std::string label)
				: m_Entity(entity), m_ValuePtr(value_ptr), m_PreviousValue(previousValue), m_Label(label), m_Value(*value_ptr)
			{
				Commands::AddCommand(this);
			}

			void Execute(CommandArgs args) override {};

			void Undo() override
			{
				m_Value = *m_ValuePtr;
				*m_ValuePtr = m_PreviousValue;

				ApplyToChildren<T, Component>(m_ValuePtr, m_Entity);

				SetCurrentCommand(true);
			}

			void Redo() override
			{
				*m_ValuePtr = m_Value;

				ApplyToChildren<T, Component>(m_ValuePtr, m_Entity);

				SetCurrentCommand(false);
			}

			const std::string GetLabel() const { return m_Label; }

		protected:
			T* m_ValuePtr;
			T m_Value;
			T m_PreviousValue;
			Entity m_Entity;
			const std::string m_Label;
		};

		class ComponentCommand : public Command
		{
		public:
			ComponentCommand(Ref<Scene>& m_Context, Entity& selectionContext)
			: m_SelectionContext(selectionContext) {  }

			virtual void Execute(CommandArgs args) = 0;
			virtual void Undo() = 0;
			virtual void Redo() = 0;

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
				}
				SetCurrentCommand(true);
			}

			void Redo() override
			{
				if (!m_SelectionContext.HasComponent<T>())
					m_SelectionContext.AddComponent<T>();

				SetCurrentCommand(false);
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
					m_Component = m_SelectionContext.GetComponent<T>();
					m_SelectionContext.RemoveComponent<T>();
				}
			}

			void Undo() override
			{
				if (!m_SelectionContext.HasComponent<T>())
				{
					m_SelectionContext.AddComponent<T>();
					Commands::SetComponent(m_SelectionContext, &m_Component);
				}
				SetCurrentCommand(true);
			}

			void Redo() override
			{
				if (m_SelectionContext.HasComponent<T>())
				{
					m_Component = m_SelectionContext.GetComponent<T>();
					m_SelectionContext.RemoveComponent<T>();
				}

				SetCurrentCommand(false);
			}

		protected:
			T m_Component;
		};

		class EntityCommand : public Command
		{
		public:
			EntityCommand(Ref<Scene>& context, Entity& selectionContext)
				: m_Context(context), m_SelectionContext(selectionContext) {  }

			virtual void Execute(CommandArgs) = 0;
			virtual void Undo() = 0;
			virtual void Redo() = 0;

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
			void Redo() override;

		protected:
			SavedEntity m_CreatedEntity;
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

			void Redo() override;

		protected:
			SavedEntity m_DeletedEntity;
			EntitySave m_EntitySave;
		};

		static void SetCurrentCommand(bool isUndo);
		static void AddCommand(Command* command);
		static bool CanRevert(bool isUndo);
		static Command* GetCurrentCommand(int offset = 0);
		static int currentCommandIndex;
		template<typename T>
		static void SetComponent(Entity& entity, T* component);
		static EntitySave SaveEntity(Entity& entity);
		static void RestoreEntity(Entity& entity, EntitySave& entitySave);

		template<typename T, typename Component>
		static void ApplyToChildren(T* value_ptr, Entity& entity)
		{
			for (Entity e : entity.GetAnyChildren())
			{
				if (e.HasComponent<Component>())
				{
					Component& parentComp = entity.GetComponent<Component>();
					Component* parentCompPtr = &parentComp;
					int offset = reinterpret_cast<char*>(value_ptr) - reinterpret_cast<char*>(parentCompPtr);
					Component& childComp = e.GetComponent<Component>();
					Component* childCompPtr = &childComp;
					memcpy(reinterpret_cast<char*>(childCompPtr) + offset, value_ptr, sizeof(T));
				}
			}
		}

		template<typename T>
		static Command* ExecuteValueCommand(std::function<void(T)> function, T value, T previousValue, const std::string label, bool bypass = false)
		{
			function(value);

			Command* command = nullptr;
			ChangeValueCommand<T>* previousCommand = dynamic_cast<ChangeValueCommand<T>*>(GetCurrentCommand());
			if (bypass || previousCommand == nullptr || previousCommand->GetLabel() != label)
				Command* command = new ChangeValueCommand<T>(function, value, previousValue, label);

			return command;
		}

		template<typename T>
		static Command* ExecuteRawValueCommand(T* value_ptr, T previousValue, const std::string label, bool bypass = false)
		{
			Command* command = nullptr;
			ChangeRawValueCommand<T>* previousCommand = dynamic_cast<ChangeRawValueCommand<T>*>(GetCurrentCommand());
			if (bypass || previousCommand == nullptr || previousCommand->GetLabel() != label)
				Command* command = new ChangeRawValueCommand<T>(value_ptr, previousValue, label);

			return command;
		}

		template<typename T, typename Component>
		static Command* ExecuteRawValueCommand(T* value_ptr, T previousValue, Entity entity, const std::string label, bool bypass = false)
		{
			Command* command = nullptr;
			ChangeRawComponentValueCommand<T, Component>* previousCommand = dynamic_cast<ChangeRawComponentValueCommand<T, Component>*>(GetCurrentCommand());
			if (bypass || previousCommand == nullptr || previousCommand->GetLabel() != label)
				Command* command = new ChangeRawComponentValueCommand<T, Component>(value_ptr, previousValue, entity, label);

			ApplyToChildren<T, Component>(value_ptr, entity);

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