#pragma once

#include "Engine/Core/UUID.h"
#include "Scene.h"
#include "Engine/Scene/Components.h"

#include "../../../../Engine/vendor/entt/include/entt.hpp"


namespace eg
{

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			EG_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			EG_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		Component* GetInheritableComponent()
		{
			EG_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");

			return dynamic_cast<Component*>(&m_Scene->m_Registry.get<T>(m_EntityHandle));
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			EG_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		bool Exists()
		{
			return m_Scene->m_Registry.valid(m_EntityHandle);
		}

		bool IsDrawable()
		{
			return m_Scene->m_Registry.all_of<IDComponent>(m_EntityHandle);
		}

		bool IsChild(Entity& child)
		{
			auto& children = m_Scene->m_EntityInfoMap[GetUUID()]->m_Children;
			auto it = std::find(children.begin(), children.end(), child.GetUUID());

			if (it != children.end())
				return true;
			else
				return false;
		}

		bool IsChildOfAny(Entity& child)
		{
			if (IsChild(child))
				return true;

			auto& children = m_Scene->m_EntityInfoMap[GetUUID()]->m_Children;
			for (auto& childUUID : children)
			{
				auto childEntity = m_Scene->GetEntityByUUID(childUUID);
				if (childEntity.IsChild(child))
					return true;
				else
					return childEntity.IsChildOfAny(child);
			}
			return false;
		}

		std::vector<Entity> GetChildren()
		{
			auto& children = m_Scene->m_EntityInfoMap[GetUUID()]->m_Children;
			std::vector<Entity> entities;
			for (auto& child : children)
			{
				entities.push_back(m_Scene->GetEntityByUUID(child));
			}
			return entities;
		}

		std::vector<UUID> GetChildrenUUID()
		{
			return m_Scene->m_EntityInfoMap[GetUUID()]->m_Children;
		}

		std::vector<Entity> GetAnyChildren()

		{
			auto& children = m_Scene->m_EntityInfoMap[GetUUID()]->m_Children;
			std::vector<Entity> entities;
			for (auto& child : children)
			{
				entities.push_back(m_Scene->GetEntityByUUID(child));
				auto childEntity = m_Scene->GetEntityByUUID(child);
				auto childChildren = childEntity.GetAnyChildren();
				entities.insert(entities.end(), childChildren.begin(), childChildren.end());
			}
			return entities;
		}

		std::optional<Entity> GetParent()
		{
			auto& parent = m_Scene->m_EntityInfoMap[GetUUID()]->m_Parent;
			if(parent == NULL)
				return std::nullopt;
			else
				return m_Scene->GetEntityByUUID(parent);
		}

		UUID GetParentUUID()
		{
			return m_Scene->m_EntityInfoMap[GetUUID()]->m_Parent;
		}

		void SetParent(std::optional<Entity> entity)
		{
			auto& parent = m_Scene->m_EntityInfoMap[GetUUID()]->m_Parent;
			if(entity.has_value())
				if(entity.value().GetUUID() != GetUUID())
					parent = entity.value().GetUUID();
			else
				parent = NULL;
		}

		void SetParent(UUID entity)
		{
			auto& parent = m_Scene->m_EntityInfoMap[GetUUID()]->m_Parent;
			parent = entity;
		}

		void AddChild(Entity child)
		{
			if (!IsChild(child) && child.GetUUID() != GetUUID())
				m_Scene->m_EntityInfoMap[GetUUID()]->m_Children.push_back(child.GetUUID());
		}

		void AddChild(UUID child)
		{
			if(child != GetUUID())
				m_Scene->m_EntityInfoMap[GetUUID()]->m_Children.push_back(child);
		}

		void RemoveChild(Entity child)
		{
			auto& children = m_Scene->m_EntityInfoMap[GetUUID()]->m_Children;
			auto it = std::find(children.begin(), children.end(), child.GetUUID());
			if (it != children.end())
				children.erase(it);
		}

		void RemoveAnyChildren()
		{
			auto& children = m_Scene->m_EntityInfoMap[GetUUID()]->m_Children;
			for (auto& child : children)
			{
				auto childEntity = m_Scene->GetEntityByUUID(child);
				childEntity.RemoveAnyChildren();
				m_Scene->DestroyEntity(childEntity);
			}
		}

		template<typename Component>
		void RevertComponentValuesInChildren(std::unordered_map<UUID, Component>& previousValues)
		{
			for (Entity e : GetAnyChildren())
			{
				if (e.HasComponent<Component>())
				{
					Entity::SetComponent(e, &previousValues[e.GetUUID()]);
				}
			}
		}

		template<typename Component>
		void CopyComponentValuesToChildren()
		{
			for (Entity e : GetAnyChildren())
			{
				if (e.HasComponent<Component>())
				{
					Component& parentComp = GetComponent<Component>();
					Component childComp = e.GetComponent<Component>();
					Entity::SetComponent(e, &parentComp);
				}
			}
		}

		template<typename Component>
		void CopyComponentValuesToChildren(std::unordered_map<UUID, Component>& previousValues)
		{
			for (Entity e : GetAnyChildren())
			{
				if (e.HasComponent<Component>())
				{
					Component& parentComp = GetComponent<Component>();
					Component childComp = e.GetComponent<Component>();
					previousValues[e.GetUUID()] = childComp;
					Entity::SetComponent(e, &parentComp);
				}
			}
		}

		template<typename Component>
		void CopyComponentToChildren(bool isUndo)
		{
			for (Entity e : GetAnyChildren())
			{
				if (e.HasComponent<Component>() && isUndo)
					e.RemoveComponent<Component>();
				else if (!e.HasComponent<Component>() && !isUndo)
					e.AddComponent<Component>();
			}
		}

		template<typename Component>
		void InheritComponentInChildren(bool applyToEntity, bool isUndo)
		{
			if (GetInheritableComponent<Component>() == nullptr)
				return;


			GetInheritableComponent<Component>()->isInheritedInChildren = !isUndo;

			if (applyToEntity)
				GetInheritableComponent<Component>()->isInherited = !isUndo;

			for (Entity& e : GetAnyChildren())
			{
				if (e.HasComponent<Component>())
				{
					e.GetInheritableComponent<Component>()->isInherited = !isUndo;
					e.GetInheritableComponent<Component>()->isInheritedInChildren = !isUndo;
				}
			}
		}

		template<typename T, typename Component>
		void ApplyValueToChildren(T* value_ptr)
		{
			for (Entity e : GetAnyChildren())
			{
				if (e.HasComponent<Component>())
				{
					if (e.GetInheritableComponent<Component>()->isInherited)
					{
						Component& parentComp = GetComponent<Component>();
						Component* parentCompPtr = &parentComp;
						int offset = reinterpret_cast<char*>(value_ptr) - reinterpret_cast<char*>(parentCompPtr);
						Component& childComp = e.GetComponent<Component>();
						Component* childCompPtr = &childComp;
						memcpy(reinterpret_cast<char*>(childCompPtr) + offset, value_ptr, sizeof(T));
					}
				}
			}
		}

		EntityInfo* GetEntityInfo()
		{
			return m_Scene->m_EntityInfoMap[GetUUID()];
		}

		UUID GetUUID()  { return GetComponent<IDComponent>().ID; }
		const std::string GetName() { return GetComponent<TagComponent>().Tag; }
		void SetName(const std::string& name) { GetComponent<TagComponent>().Tag = name; }
		entt::entity GetHandle() { return m_EntityHandle; }
		Scene* GetScene() { return m_Scene; }

		operator bool() const { return m_EntityHandle != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator entt::entity() const { return m_EntityHandle; }

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}
		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

		template<typename T>
		static void SetComponent(Entity& entity, T* component);

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}
