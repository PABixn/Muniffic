#pragma once
#include <string>
#include "Engine.h"
#include <variant>
#include <cstddef>
#include "Engine/Resources/ResourceDatabase.h"
#include "Engine/Resources/ResourceUtils.h"

namespace eg
{
	void SetIsSaved(bool val);
	bool GetIsSaved();
	class Commands
	{
	public:

		const static int MAX_COMMANDS = 200;

		enum class InheritanceCommandType
		{
			COPY_COMPONENT,
			COPY_COMPONENT_VALUES,
			COPY_COMPONENT_AND_VALUES
		};

		using AllSavedComponents = std::tuple<std::optional<TagComponent>,
			std::optional<TransformComponent>,
			std::optional<SpriteRendererComponent>,
			std::optional<CircleRendererComponent>,
			std::optional<CameraComponent>,
			std::optional<ScriptComponent>,
			std::optional<RigidBody2DComponent>,
			std::optional<BoxCollider2DComponent>,
			std::optional<CircleCollider2DComponent>,
			std::optional<TextComponent>,
			std::optional<SpriteRendererSTComponent>>;

		struct CommandArgs
		{
		public:
			CommandArgs(const std::string& name, Entity entity,
				Ref<Scene>& context,
				Entity& selectionContext, std::optional<Entity> parent = std::nullopt)
				: m_Name(name), m_Entity(entity), m_Context(context), m_SelectionContext(selectionContext), m_Parent(parent) {  }

			const std::string& m_Name;
			Entity m_Entity;
			Ref<Scene>& m_Context;
			Entity& m_SelectionContext;
			std::optional<Entity> m_Parent;
		};

		struct EntitySave
		{
		public:
			EntitySave(const std::string& name, UUID uuid)
				: m_Name(name), m_UUID(uuid) {  }

			EntitySave() = default;

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
					GetComponent<TextComponent>(),
					GetComponent<SpriteRendererSTComponent>());
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

			std::string m_Name;
			UUID m_UUID;
			UUID m_Parent;
		};

		class Command
		{
		public:
			virtual void Undo() = 0;
			virtual void Execute(CommandArgs) = 0;
			virtual void Redo() = 0;
		};

		class RenameDirectoryCommand : public Command
		{
		public:
			RenameDirectoryCommand(const std::filesystem::path& path, const std::string& newName)
				: m_NewName(newName), m_Path(path)
			{
				Commands::AddCommand(this);

				m_OldName = path.filename().string();

				ResourceDatabase::RenameDirectory(path, newName);
			}

			void Execute(CommandArgs args) override {};
			void Undo() override;
			void Redo() override;

		protected:
			std::filesystem::path m_Path;
			std::string m_OldName;
			std::string m_NewName;
		};

		class RenameResourceCommand : public Command
		{
		public:
			RenameResourceCommand(UUID uuid, const std::string& newName)
				: m_UUID(uuid), m_NewName(newName)
			{
				Commands::AddCommand(this);

				m_OldName = ResourceDatabase::GetResourceName(uuid);

				ResourceDatabase::RenameResource(uuid, newName);
			}

			void Execute(CommandArgs args) override {};
			void Undo() override;
			void Redo() override;

		protected:
			UUID m_UUID;
			std::string m_NewName;
			std::string m_OldName;
		};

		class MoveResourceCommand : public Command
		{
		public:
			MoveResourceCommand(UUID uuid, const std::filesystem::path& path)
				: m_UUID(uuid), m_Path(path)
			{
				Commands::AddCommand(this);

				m_OldPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / ResourceDatabase::GetResourcePath(uuid);

				ResourceDatabase::MoveResource(uuid, path);
			}

			void Execute(CommandArgs args) override {};
			void Undo() override;
			void Redo() override;

		protected:
			UUID m_UUID;
			std::filesystem::path m_Path;
			std::filesystem::path m_OldPath;
		};

		class LoadResourceCommand : public Command
		{
		public:
			LoadResourceCommand(const std::filesystem::path& path)
				: m_Path(path)
			{
				Commands::AddCommand(this);

				ResourceDatabase::LoadResource(path);
			}

			void Execute(CommandArgs args) override {};
			void Undo() override;
			void Redo() override;

		protected:
			std::filesystem::path m_Path;
		};

		class DeleteDirectoryCommand : public Command
		{
		public:
			DeleteDirectoryCommand(const std::filesystem::path& directory)
				: m_Directory(directory)
			{
				Commands::AddCommand(this);

				ResourceDatabase::DeleteDirectory(directory);
			}

			void Execute(CommandArgs args) override {};
			void Undo() override;
			void Redo() override;

		protected:
			std::filesystem::path m_Directory;
		};

		class DeleteResourceCommand : public Command
		{
		public:
			DeleteResourceCommand(UUID uuid, ResourceType resourceType, bool deleteFile = false)
				: m_UUID(uuid), m_ResourceType(resourceType), m_DeleteFile(deleteFile), m_Resource(ResourceUtils::GetResourcePointer(uuid, resourceType))
			{
				if(m_DeleteFile == false)
					Commands::AddCommand(this);

				ResourceDatabase::RemoveResource(m_UUID, resourceType, deleteFile);
			}

			void Execute(CommandArgs args) override {};
			void Undo() override;
			void Redo() override;

		protected:
			UUID m_UUID;
			ResourceType m_ResourceType;
			bool m_DeleteFile;
			void* m_Resource;
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

				m_Entity.ApplyValueToChildren<T, Component>(m_ValuePtr);

				SetCurrentCommand(true);
			}

			void Redo() override
			{
				*m_ValuePtr = m_Value;

				m_Entity.ApplyValueToChildren<T, Component>(m_ValuePtr);

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
					Entity::SetComponent(m_SelectionContext, &m_Component);
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
			EntitySave m_CreatedEntity;
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
			EntitySave m_DeletedEntity;
			std::vector<EntitySave> m_Children;
		};

		class ChangeParentCommand : public Command
		{
		public:
			ChangeParentCommand(Entity& entity, std::optional<Entity> parent, Ref<Scene>& context)
				: m_Entity(entity.GetUUID()), m_Context(context)
			{
				Commands::AddCommand(this);

				m_Parent = parent.has_value() ? std::optional(parent.value().GetUUID()) : std::nullopt;
				m_PreviousParent = entity.GetParent().has_value() ? std::optional(entity.GetParent().value().GetUUID()) : std::nullopt;

				ChangeParent(entity, parent);
			}

			void Execute(CommandArgs arg) override {};
			void Undo() override;
			void Redo() override;

		protected:
			UUID m_Entity;
			std::optional<UUID> m_Parent;
			std::optional<UUID> m_PreviousParent;
			Ref<Scene>& m_Context;
		};

		template<typename Component>
		class InheritComponentCommand : public Command
		{
		public:
			InheritComponentCommand(Entity& entity, Ref<Scene>& context, bool isUndo, bool applyToEntity)
				: m_Entity(entity.GetUUID()), m_isUndo(isUndo), m_applyToEntity(applyToEntity), m_Context(context)
			{
				Commands::AddCommand(this);

				entity.InheritComponentInChildren<Component>(m_applyToEntity, m_isUndo);
			}

			void Execute(CommandArgs arg) override {};

			void Undo() override
			{
				Entity entity = m_Context->GetEntityByUUID(m_Entity);

				entity.InheritComponentInChildren<Component>(m_applyToEntity, !m_isUndo);

				SetCurrentCommand(true);
			}

			void Redo() override
			{
				Entity entity = m_Context->GetEntityByUUID(m_Entity);

				entity.InheritComponentInChildren<Component>(m_applyToEntity, m_isUndo);

				SetCurrentCommand(false);
			}


		protected:
			UUID m_Entity;
			bool m_isUndo;
			bool m_applyToEntity;
			Ref<Scene>& m_Context;
		};

		template<typename T>
		class ManageComponentInheritanceCommand : public Command
		{
		public:
			ManageComponentInheritanceCommand(Entity& entity, Ref<Scene>& context, InheritanceCommandType commandType, bool isUndo)
				: m_Entity(entity.GetUUID()), m_CommandType(commandType), m_isUndo(isUndo), m_Context(context)
			{
				switch (commandType)
				{
					case InheritanceCommandType::COPY_COMPONENT: entity.CopyComponentToChildren<T>(isUndo); break;
					case InheritanceCommandType::COPY_COMPONENT_VALUES: entity.CopyComponentValuesToChildren<T>(m_PreviousValues); break;
					case InheritanceCommandType::COPY_COMPONENT_AND_VALUES: 
					{
						entity.CopyComponentToChildren<T>(isUndo);
						entity.CopyComponentValuesToChildren<T>(m_PreviousValues);
						break;
					}
				}

				Commands::AddCommand(this);
			}

			void Execute(CommandArgs arg) override {};

			void Undo() override
			{
				Entity entity = m_Context->GetEntityByUUID(m_Entity);

				switch (m_CommandType)
				{
					case InheritanceCommandType::COPY_COMPONENT: entity.CopyComponentToChildren<T>(!m_isUndo); break;
					case InheritanceCommandType::COPY_COMPONENT_VALUES: entity.RevertComponentValuesInChildren<T>(m_PreviousValues); break;
					case InheritanceCommandType::COPY_COMPONENT_AND_VALUES:
					{
						entity.CopyComponentToChildren<T>(!m_isUndo);
						entity.RevertComponentValuesInChildren<T>(m_PreviousValues);
						break;
					}
					
				}

				SetCurrentCommand(true);
			}

			void Redo() override
			{
				Entity entity = m_Context->GetEntityByUUID(m_Entity);

				switch (m_CommandType)
				{
					case InheritanceCommandType::COPY_COMPONENT: entity.CopyComponentToChildren<T>(m_isUndo); break;
					case InheritanceCommandType::COPY_COMPONENT_VALUES: entity.CopyComponentValuesToChildren<T>(m_PreviousValues); break;
					case InheritanceCommandType::COPY_COMPONENT_AND_VALUES:
					{
						entity.CopyComponentToChildren<T>(m_isUndo);
						entity.CopyComponentValuesToChildren<T>(m_PreviousValues);
						break;
					}
				}

				SetCurrentCommand(false);
			}

		protected:
			UUID m_Entity;
			InheritanceCommandType m_CommandType;
			bool m_isUndo;
			Ref<Scene>& m_Context;
			std::unordered_map<UUID, T> m_PreviousValues;
		};

		enum class VectorCommandType
		{
			ADD,
			REMOVE_LAST,
			ADD_AT,
			REMOVE_AT
		};

		template<typename T>
		class VectorCommand : public Command
		{

			public:
				VectorCommand(Ref<std::vector<T>> vector, VectorCommandType revertCommand, VectorCommandType forwardCommand, T oldValue, T newValue, size_t index = 0)
					: m_Vector(vector), m_RevertCommand(revertCommand), m_ForwardCommand(forwardCommand), m_OldValue(oldValue), m_NewValue(newValue), m_Index(index)
				{
				Commands::AddCommand(this);
			}

			void Execute(CommandArgs arg) override {};

			void Undo() override
			{
				switch (m_RevertCommand)
				{
				case VectorCommandType::ADD: m_Vector->push_back(m_OldValue); break;
				case VectorCommandType::REMOVE_LAST: m_Vector->pop_back(); break;
				case VectorCommandType::ADD_AT: m_Vector->insert(m_Vector->begin() + m_Index, m_OldValue); break;
				case VectorCommandType::REMOVE_AT: m_Vector->erase(m_Vector->begin() + m_Index); break;
				}

				SetCurrentCommand(true);
			}

			void Redo() override
			{
				switch (m_ForwardCommand)
				{
					case VectorCommandType::ADD: m_Vector->push_back(m_NewValue); break;
					case VectorCommandType::REMOVE_LAST: m_Vector->pop_back(); break;
					case VectorCommandType::ADD_AT: m_Vector->insert(m_Vector->begin() + m_Index, m_NewValue); break;
					case VectorCommandType::REMOVE_AT: m_Vector->erase(m_Vector->begin() + m_Index); break;
				}

				SetCurrentCommand(false);
			}

			protected:
				Ref<std::vector<T>> m_Vector;
				VectorCommandType m_RevertCommand;
				VectorCommandType m_ForwardCommand;
				T m_OldValue;
				T m_NewValue;
				int m_Index;
		};

		static void SetCurrentCommand(bool isUndo);
		static void AddCommand(Command* command);
		static bool CanRevert(bool isUndo);
		static Command* GetCurrentCommand(int offset = 0);
		static int currentCommandIndex;
		static EntitySave SaveEntity(Entity& entity);
		static void RestoreEntity(Entity& entity, EntitySave& entitySave);
		static void ChangeParent(Entity& entity, std::optional<Entity> parent);

		template<typename... Component>
		static void SetInheritedComponents(ComponentGroup<Component...>, Entity& entity, std::optional<Entity> parent)
		{
			(TryInheritInChildren<Component>(entity, parent), ...);
		}

		template<typename Component>
		static void TryInheritInChildren(Entity& entity, std::optional<Entity> parent)
		{
			if (parent.has_value())
			{
				if (entity.HasComponent<Component>() && parent.value().HasComponent<Component>())
				{
					entity.InheritComponentInChildren<Component>(true, !parent.value().GetInheritableComponent<Component>()->isInheritedInChildren);
				}
			}
			else
			{
				if(entity.HasComponent<Component>())
					entity.GetInheritableComponent<Component>()->isInherited = false;
			}
		}

		static Command* ExecuteRenameDirectoryCommand(const std::filesystem::path& path, const std::string& newName)
		{
			Command* command = new RenameDirectoryCommand(path, newName);
			return command;
		}

		static Command* ExecuteRenameResourceCommand(UUID uuid, const std::string& newName)
		{
			Command* command = new RenameResourceCommand(uuid, newName);
			return command;
		}

		static Command* ExecuteMoveResourceCommand(UUID uuid, const std::filesystem::path& path)
		{
			Command* command = new MoveResourceCommand(uuid, path);
			return command;
		}

		static Command* ExecuteLoadResourceCommand(const std::filesystem::path& path)
		{
			Command* command = new LoadResourceCommand(path);
			return command;
		}

		static Command* ExecuteDeleteResourceCommand(UUID uuid, ResourceType resourceType, bool deleteFile = false)
		{
			Command* command = new DeleteResourceCommand(uuid, resourceType, deleteFile);
			return command;
		}

		static Command* ExecuteDeleteDirectoryCommand(const std::filesystem::path& directory)
		{
			Command* command = new DeleteDirectoryCommand(directory);
			return command;
		}

		template<typename T>
		static Command* ExecuteValueCommand(std::function<void(T)> function, T value, T previousValue, const std::string label, bool bypass = false)
		{
			function(value);

			Command* command = nullptr;
			ChangeValueCommand<T>* previousCommand = dynamic_cast<ChangeValueCommand<T>*>(GetCurrentCommand());
			if (bypass || previousCommand == nullptr || previousCommand->GetLabel() != label)
				Command* command = new ChangeValueCommand<T>(function, value, previousValue, label);
			else {
				if (GetIsSaved()) {
					SetIsSaved(false);
				}
			}
			return command;
		}

		template<typename T>
		static Command* ExecuteRawValueCommand(T* value_ptr, T previousValue, const std::string label, bool bypass = false)
		{
			Command* command = nullptr;
			ChangeRawValueCommand<T>* previousCommand = dynamic_cast<ChangeRawValueCommand<T>*>(GetCurrentCommand());
			if (bypass || previousCommand == nullptr || previousCommand->GetLabel() != label)
				Command* command = new ChangeRawValueCommand<T>(value_ptr, previousValue, label);
			else {
				if (GetIsSaved()) {
					SetIsSaved(false);
				}
			}

			return command;
		}

		template<typename T, typename Component>
		static Command* ExecuteRawValueCommand(T* value_ptr, T previousValue, Entity entity, const std::string label, bool bypass = false)
		{
			Command* command = nullptr;
			ChangeRawComponentValueCommand<T, Component>* previousCommand = dynamic_cast<ChangeRawComponentValueCommand<T, Component>*>(GetCurrentCommand());
			if (bypass || previousCommand == nullptr || previousCommand->GetLabel() != label)
				Command* command = new ChangeRawComponentValueCommand<T, Component>(value_ptr, previousValue, entity, label);

			entity.ApplyValueToChildren<T, Component>(value_ptr);

			return command;
		}

		static Command* ExecuteChangeParentCommand(Entity& entity, std::optional<Entity> parent, Ref<Scene>& context)
		{
			Command* command = new ChangeParentCommand(entity, parent, context);
			return command;
		}

		template<typename Component>
		static Command* ExecuteInheritComponentCommand(Entity& entity, Ref<Scene>& context, bool isUndo = false, bool applyToEntity = false)
		{
			Command* command = new InheritComponentCommand<Component>(entity, context, isUndo, applyToEntity);
			return command;
		}

		template<typename Component>
		static Command* ExecuteManageComponentInheritanceCommand(Entity& entity, Ref<Scene>& context, InheritanceCommandType commandType, bool isUndo = false)
		{
			Command* command = new ManageComponentInheritanceCommand<Component>(entity, context, commandType, isUndo);
			return command;
		}

		template<typename T>
		static Command* ExecuteVectorCommand(Ref<std::vector<T>> vector, VectorCommandType revertCommand, VectorCommandType forwardCommand, T oldValue, T newValue)
		{
			Command* command = new VectorCommand<T>(vector, revertCommand, forwardCommand, oldValue, newValue);
			return command;
		}

		template<typename T>
		static Command* ExecuteCommand(CommandArgs args)
		{
			Command* command = new T(args.m_Context, args.m_SelectionContext);
			command->Execute(args);
			return command;
		}
		 
	private:
		static std::vector<Commands::Command*> commandHistory;

	};
}