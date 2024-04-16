#pragma once

#include "Engine/Core/Timestep.h"
#include "Engine/Core/UUID.h"
#include "Engine/Renderer/EditorCamera.h"

#include "../../../vendor/entt/include/entt.hpp"
#include "EntityInfo.h"
#include "Hud.h"
//#include "../../Engine-Editor/src/Panels/LayersPanel.h"

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

		UUID GetUUIDFromEntity(entt::entity entity);

		int GetLayerFromEnttEntity(entt::entity entity);
		

		void OnSimulationStart();
		void OnSimulationStop();

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnUpdateRuntime(Timestep ts);
		void OnUpdateSimulation(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		void RenderScene(EditorCamera& camera);

		Entity DuplicateEntity(Entity entity);

		Entity FindEntityByName(const std::string_view& name);
		Entity GetEntityByUUID(UUID uuid);

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
		Hud* GetActiveHud() { return m_ActiveHud; }
		void SetActiveHud(Hud* hud) { m_ActiveHud = hud; }
		void Step(int frames = 1);
		const std::unordered_map<UUID, entt::entity>& GetEntityMap() const {
			return m_EntityMap;
		};
		const std::unordered_map<UUID, EntityInfo*>& GetEntityInfoMap() const {
			return m_EntityInfoMap;
		};
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		void OnPhysics2DStart();
		void OnPhysics2DStop();

	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;
		bool m_IsRunning = false;
		bool m_IsPaused = false;
		
		Hud* m_ActiveHud = nullptr;
		int m_StepFrames = 0;

		std::unordered_map<UUID, entt::entity> m_EntityMap;
		std::unordered_map<entt::entity, UUID> m_ReversedEntityMap;
		std::unordered_map<UUID, EntityInfo*> m_EntityInfoMap;
		//std::unorderem_map<entt::entity, int>;
		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
		friend class Hud;
	};

}