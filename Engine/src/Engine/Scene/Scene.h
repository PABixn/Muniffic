#pragma once

#include "Engine/Core/Timestep.h"
#include "Engine/Core/UUID.h"
#include "Engine/Renderer/EditorCamera.h"

#include "../../../vendor/entt/include/entt.hpp"
#include "EntityInfo.h"
#include "vector"

class b2World;

namespace eg {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();
			
		static Ref<Scene> Copy(Ref<Scene>& other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnSimulationStart();
		void OnSimulationStop();

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts, std::chrono::steady_clock::time_point& oldTime);
		void OnUpdateSimulation(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		void RenderScene(EditorCamera& camera);

		void RenderAxis();

		void AddEntityToDestroy(Entity entity);

		Entity DuplicateEntity(Entity entity);

		Entity FindEntityByName(const std::string_view& name);
		void FindEntitiesByName(const std::string_view& name, std::vector<Entity>& outVec);
		Entity GetEntityByUUID(UUID uuid);
		bool EntityExists(UUID uuid);
		void AwakeRuntimeBody(Entity entity);

		Entity GetPrimaryCameraEntity();

		template<typename... Components>
		auto GetEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

		entt::registry& GetRegistry()
		{
			return m_Registry;
		}

		bool IsRunning() const { return m_IsRunning; }
		bool IsPaused() const { return m_IsPaused; }

		void SetPaused(bool paused) { m_IsPaused = paused; }

		void Step(int frames = 1);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		void OnPhysics2DStart();
		void OnPhysics2DStop();

		void FixedUpdate();


		void* StartRuntimeBody(Entity entity);

	private:
		entt::registry m_Registry;
		std::vector<Entity> m_EntitiesToDestroy;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;
		bool m_IsRunning = false;
		bool m_IsPaused = false;
		bool m_ViewportNeedsResize = true;

		int m_StepFrames = 0;

		//fixedUpdate
		std::chrono::time_point<std::chrono::high_resolution_clock> m_NewTime;
		unsigned long m_Delta;
		float m_TimePassed = 0;
		float m_FixedFramerate = 1000.0f / 720.0f;

		std::unordered_map<UUID, entt::entity> m_EntityMap;
		//Change to shared_ptr if doesn't couse any problems
		std::unordered_map<UUID, EntityInfo*> m_EntityInfoMap;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}