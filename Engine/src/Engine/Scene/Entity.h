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

		void SetParent(Entity entity)
		{
			auto& parent = m_Scene->m_EntityInfoMap[GetUUID()]->m_Parent;
			parent = entity.GetUUID();
		}

		void AddChild(Entity child)
		{
			if (!IsChild(child))
				m_Scene->m_EntityInfoMap[GetUUID()]->m_Children.push_back(child.GetUUID());
		}

		void RemoveChild(Entity child)
		{
			auto& children = m_Scene->m_EntityInfoMap[GetUUID()]->m_Children;
			auto it = std::find(children.begin(), children.end(), child.GetUUID());
			if (it != children.end())
				children.erase(it);
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

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}
