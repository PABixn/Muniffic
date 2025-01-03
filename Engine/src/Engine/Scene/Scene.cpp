#include "egpch.h"

#include "Scene.h"
#include "Components.h"
#include "ScriptableEntity.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Physics/Physics2D.h"

#include <glm/glm.hpp>
#include "../Engine-Editor/src/Panels/ConsolePanel.h"
#include "Entity.h"

// Box2D
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

#include <Mmsystem.h>
#include <mciapi.h>
#pragma comment(lib, "Winmm.lib")

namespace eg {
	bool EvaluateSceneAudio = true;

	Scene::Scene()
	{
        EG_PROFILE_FUNCTION();
	}

	Scene::~Scene()
	{
        EG_PROFILE_FUNCTION();
	}



	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
        EG_PROFILE_FUNCTION();
		([&]()
			{
			auto view = src.view<Component>();
			for (auto srcEntity : view)
			{
			entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).ID);

			auto& srcComponent = src.get<Component>(srcEntity);
			dst.emplace_or_replace<Component>(dstEntity, srcComponent);
			}
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponent(ComponentGroup<Component...>, entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
        EG_PROFILE_FUNCTION();
		CopyComponent<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
        EG_PROFILE_FUNCTION();
		([&]()
			{
				if (src.HasComponent<Component>())
					dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
        EG_PROFILE_FUNCTION();
		CopyComponentIfExists<Component...>(dst, src);
	}

	Ref<Scene> Scene::Copy(Ref<Scene>& other)
	{
        EG_PROFILE_FUNCTION();
		Ref<Scene> scene = CreateRef<Scene>();
		scene->m_ViewportWidth = other->m_ViewportWidth;
		scene->m_ViewportHeight = other->m_ViewportHeight;

		std::unordered_map<UUID, entt::entity> enttMap;

		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = scene->m_Registry;
		auto idView = srcSceneRegistry.view<IDComponent>();
		EG_CORE_INFO("Entities count: {0}", idView.size());
		// Copy entities
		for(auto entity : idView)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(entity).ID;
			const auto& tag = srcSceneRegistry.get<TagComponent>(entity).Tag;
			Entity newEntity = scene->CreateEntityWithID(uuid, tag);
			enttMap[uuid] = (entt::entity)newEntity;
			//Copy the values of the entity info not the pointer
			EntityInfo* info = new EntityInfo(other->m_EntityInfoMap[uuid]->m_Parent);
			for (auto child : other->m_EntityInfoMap[uuid]->m_Children)
			{
				info->m_Children.push_back(child);
			}
			scene->m_EntityInfoMap[uuid] = info;
		}

		// Copy components (except IDComponent and TagComponent)
		CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);
		ConsolePanel::Log("File: Scene.cpp - Scene copied", ConsolePanel::LogType::Info);
		return scene;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
        EG_PROFILE_FUNCTION();
		return CreateEntityWithID(UUID(), name);
	}

	Entity Scene::CreateEntityWithID(UUID uuid, const std::string& name)
	{
        EG_PROFILE_FUNCTION();
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>(uuid);
		auto& tag = entity.AddComponent<TagComponent>(uuid);
		tag.Tag = name.empty() ? "Entity" : name;

		m_EntityMap[uuid] = (entt::entity)entity;
		m_EntityInfoMap[uuid] = new EntityInfo(NULL);
		ConsolePanel::Log("File: Scene.cpp - Entity created: " + tag.Tag, ConsolePanel::LogType::Info);
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
        EG_PROFILE_FUNCTION();
		if (entity.GetParent().has_value())
			entity.GetParent().value().RemoveChild(entity);
		if (m_IsRunning && entity.HasComponent<RigidBody2DComponent>())
		{
			b2Body* entityBody = (b2Body*)entity.GetComponent<RigidBody2DComponent>().RuntimeBody;
			m_PhysicsWorld->DestroyBody(entityBody);
		}
		m_EntityMap.erase(entity.GetUUID());
		m_EntityInfoMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
		ConsolePanel::Log("File: Scene.cpp - Entity destroyed", ConsolePanel::LogType::Info);
	}

	void Scene::OnRuntimeStart()
	{
        EG_PROFILE_FUNCTION();
		OnPhysics2DStart();
		m_IsRunning = true;
		//Scripting
		{
			ScriptEngine::OnRuntimeStart(this);
			//Instantiate all script components

			auto view = m_Registry.view<ScriptComponent>();
			for(auto entity : view)
			{
				Entity e{ entity, this };
				ScriptEngine::OnCreateEntity(e);
			}
		}
		if (EvaluateSceneAudio) {
			auto ASourceView = m_Registry.view<AudioSourceComponent>();
			for (auto f : ASourceView)
			{
				ASourceView.get<AudioSourceComponent>(f).Audio->LoadCurrentAudio();
				if (ASourceView.get<AudioSourceComponent>(f).Audio->IsPlayingFromStart()) {
					ASourceView.get<AudioSourceComponent>(f).Audio->Play();
				}
			}
		}
	}

	void Scene::OnRuntimeStop()
	{
        EG_PROFILE_FUNCTION();
		OnPhysics2DStop();
		m_IsRunning = false;
		if (EvaluateSceneAudio) {
			auto ASourceView = m_Registry.view<AudioSourceComponent>();
			for (auto f : ASourceView)
			{


				ASourceView.get<AudioSourceComponent>(f).Audio->Stop();

			}
		}
		ScriptEngine::OnRuntimeStop();
	}

	void Scene::OnSimulationStart()
	{
        EG_PROFILE_FUNCTION();
		OnPhysics2DStart();
		// Audio
		if (EvaluateSceneAudio) {
				auto ASourceView = m_Registry.view<AudioSourceComponent>();
				for (auto f : ASourceView)
				{
					ASourceView.get<AudioSourceComponent>(f).Audio->LoadCurrentAudio();
					if (ASourceView.get<AudioSourceComponent>(f).Audio->IsPlayingFromStart()) {
						ASourceView.get<AudioSourceComponent>(f).Audio->Play();
					}
				}
		}
	}

	void Scene::OnSimulationStop()
	{
        EG_PROFILE_FUNCTION();
		OnPhysics2DStop();
		if (EvaluateSceneAudio) {
			auto ASourceView = m_Registry.view<AudioSourceComponent>();
			for (auto f : ASourceView)
			{


					ASourceView.get<AudioSourceComponent>(f).Audio->Stop();

			}
		}
	}


	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
        EG_PROFILE_FUNCTION();
		RenderScene(camera);
	}

	void Scene::OnUpdateRuntime(Timestep ts,std::chrono::steady_clock::time_point& oldTime)
	{
        EG_PROFILE_FUNCTION();
		if (!m_IsPaused || m_StepFrames-- > 0)
		{
			// Update Native scripts
			{
				//C# Entity OnUpdate
				auto view = m_Registry.view<ScriptComponent>();
				for (auto entity : view)
				{
					Entity e{ entity, this };
					ScriptEngine::OnUpdateEntity(e, ts);
				}

				m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
				{
					// TODO: Move to Scene::OnScenePlay
					if (!nsc.Instance)
					{
						nsc.Instance = nsc.InstantiateScript();
						nsc.Instance->m_Entity = Entity{ entity, this };
						nsc.Instance->OnCreate();
					}

					nsc.Instance->OnUpdate(ts);
				});
			}

			// Physics
			{
				const int32_t velocityIterations = 6;
				const int32_t positionIterations = 2;
				m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

				auto view = m_Registry.view<RigidBody2DComponent>();
				{
					for (auto e : view)
					{
						Entity entity{ e, this };
						auto& rb = entity.GetComponent<RigidBody2DComponent>();
						auto& transform = entity.GetComponent<TransformComponent>();
						auto* body = (b2Body*)rb.RuntimeBody;

						transform.Translation.x = body->GetPosition().x;
						transform.Translation.y = body->GetPosition().y;
						transform.Rotation.z = body->GetAngle();
					}
				}
			}

		}


		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4 cameraTransform;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = transform.GetTransform();
					break;
				}
			}
		}

		// Update Animation
		{
			auto t = m_Registry.view<AnimatorComponent>();
			for (auto entity : t)
			{
				auto animator = t.get<AnimatorComponent>(entity);
				animator.Animator2D->Update(ts);
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), cameraTransform);
			// Draw sprites
			{
				auto group = m_Registry.view<TransformComponent, SpriteRendererComponent>();
				for (auto entity : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

					Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
				}
			}

			//Set Animations


			{
				auto group = m_Registry.view<TransformComponent, AnimatorComponent>();
				for (auto entity : group)
				{
					auto [transform, animator] = group.get<TransformComponent, AnimatorComponent>(entity);
					if (animator.Animator2D->GetCurrentAnimation() == nullptr || animator.Animator2D->GetCurrentAnimation()->GetFrameCount() == 0)
						continue;
					Ref<SubTexture2D> texture = animator.Animator2D->GetCurrentAnimation()->GetFrame()->GetSubTexture();

					Renderer2D::DrawQuad(transform.GetTransform(), texture, 1, glm::vec4(1.0f), (int)entity);
				}
			}

			// Draw Subtexture sprites
			{
				auto group = m_Registry.view<TransformComponent, SpriteRendererSTComponent>();
				for (auto entity : group)
				{
					auto [transform, sprite] = group.get<TransformComponent, SpriteRendererSTComponent>(entity);

					Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
				}
			}

			// Draw circles
			{
				auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
				for (auto entity : view)
				{
					auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

					Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
				}
			}

			// Draw text
			{
				auto view = m_Registry.view<TransformComponent, TextComponent>();
				for (auto entity : view)
				{
					auto [transform, text] = view.get<TransformComponent, TextComponent>(entity);

					Renderer2D::DrawString(text.TextString, transform.GetTransform(), text, (int)entity);
				}
			}

			Renderer2D::EndScene();
		}

		for(auto& entity : m_EntitiesToDestroy)
			DestroyEntity(entity);
		m_EntitiesToDestroy.clear();
	}

	void Scene::OnUpdateSimulation(Timestep ts, EditorCamera& camera)
	{
        EG_PROFILE_FUNCTION();

		if (!m_IsPaused || m_StepFrames-- > 0)
			// Physics
		{
			{
				const int32_t velocityIterations = 6;
				const int32_t positionIterations = 2;
				m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

				// Retrieve transform from Box2D
				auto view = m_Registry.view<RigidBody2DComponent>();
				for (auto e : view)
				{
					Entity entity = { e, this };
					auto& transform = entity.GetComponent<TransformComponent>();
					auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

					b2Body* body = (b2Body*)rb2d.RuntimeBody;
					const auto& position = body->GetPosition();
					transform.Translation.x = position.x;
					transform.Translation.y = position.y;
					transform.Rotation.z = body->GetAngle();
				}
			}
		}

		// Render
		RenderScene(camera);
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
        EG_PROFILE_FUNCTION();
		if (m_ViewportHeight == height && m_ViewportWidth == width)
			return;

		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}

	}

	void Scene::RenderScene(EditorCamera& camera)
	{
        EG_PROFILE_FUNCTION();
		Renderer2D::BeginScene(camera);
		RenderAxis();

		// Draw sprites
		{
			auto group = m_Registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

				Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			}
		}

		//Draw Animations
		{
			auto group = m_Registry.group<TransformComponent, AnimatorComponent>();
			for (auto entity : group)
			{
				auto [transform, animator] = group.get<TransformComponent, AnimatorComponent>(entity);
				if(animator.Animator2D->GetCurrentAnimation() == nullptr || animator.Animator2D->GetCurrentAnimation()->GetFrameCount() == 0)
					continue;
				Ref<SubTexture2D> texture = animator.Animator2D->GetCurrentAnimation()->GetFrame()->GetSubTexture();

				Renderer2D::DrawQuad(transform.GetTransform(), texture, 1, glm::vec4(1.0f), (int)entity);
			}
		}

		// Draw Subtexture sprites
		{
			auto group = m_Registry.view<TransformComponent, SpriteRendererSTComponent>();
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererSTComponent>(entity);
				bool canBeDisplayed = sprite.SubTexture && sprite.SubTexture->GetTexture() && sprite.SubTexture->GetTexture()->IsLoaded();
				if(canBeDisplayed)
					Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
				else
					Renderer2D::DrawQuad(transform.GetTransform(), { 1.0f, 0.0f, 1.0f, 1.0f }, (int)entity);
			}
		}

		// Draw circles
		{
			auto view = m_Registry.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);

				Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
			}
		}

		// Draw text
		{
			auto view = m_Registry.view<TransformComponent, TextComponent>();
			for (auto entity : view)
			{
				auto [transform, text] = view.get<TransformComponent, TextComponent>(entity);

				if(text.RuntimeFont)
					Renderer2D::DrawString(text.TextString, transform.GetTransform(), text, (int)entity);
			}
		}

		Renderer2D::EndScene();
	}

	void Scene::RenderAxis()
	{
        EG_PROFILE_FUNCTION();
		//X: red
		//y: green
		//z: blue

	}

	void Scene::AddEntityToDestroy(Entity entity)
	{
        EG_PROFILE_FUNCTION();
		m_EntitiesToDestroy.push_back(entity);
	}

	Entity Scene::DuplicateEntity(Entity entity)
	{
        EG_PROFILE_FUNCTION();
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);
		CopyComponentIfExists(AllComponents{}, newEntity, entity);
		return newEntity;
	}

	Entity Scene::FindEntityByName(const std::string_view& name)
	{
        EG_PROFILE_FUNCTION();
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const auto& tag = view.get<TagComponent>(entity).Tag;
			if (tag == name)
				return Entity{ entity, this };
		}
		return {};
	}

	void Scene::FindEntitiesByName(const std::string_view& name, std::vector<Entity>& outVec) {
        EG_PROFILE_FUNCTION();
		auto view  = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const auto& tag = view.get<TagComponent>(entity).Tag;
			if (tag == name)
				outVec.emplace_back( entity, this );
		}
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
        EG_PROFILE_FUNCTION();
		//TODO: Maybe should assert
		if(m_EntityMap.find(uuid) != m_EntityMap.end())
			return Entity{ m_EntityMap[uuid], this };
		return {};
	}

	bool Scene::EntityExists(UUID uuid)
	{
        EG_PROFILE_FUNCTION();
		return m_EntityMap.find(uuid) != m_EntityMap.end();
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
        EG_PROFILE_FUNCTION();
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.Primary)
				return Entity{ entity, this };
		}
		return {};
	}

	void Scene::Step(int frames)
	{
        EG_PROFILE_FUNCTION();
		m_StepFrames = frames;
	}

	void Scene::AwakeRuntimeBody(Entity entity)
	{
        EG_PROFILE_FUNCTION();
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rb = entity.GetComponent<RigidBody2DComponent>();

		b2BodyDef bodyDef;
		bodyDef.type = Utils::RigidBody2DTypeToBox2DBody(rb.Type);
		bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
		bodyDef.angle = transform.Rotation.z;

		b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rb.FixedRotation);
		rb.RuntimeBody = body;

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			auto& bc = entity.GetComponent<BoxCollider2DComponent>();
			b2PolygonShape shape;
			shape.SetAsBox(bc.Size.x * transform.Scale.x, bc.Size.y * transform.Scale.y, { bc.Offset.x, bc.Offset.y }, 0.0f);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.density = bc.Density;
			fixtureDef.friction = bc.Friction;
			fixtureDef.restitution = bc.Restitution;
			fixtureDef.restitutionThreshold = bc.RestitutionThreshold;
			fixtureDef.userData.pointer = entity.GetUUID();
			fixtureDef.isSensor = bc.IsSensor;

			b2Fixture* fixture = body->CreateFixture(&fixtureDef);
			bc.RuntimeFixture = fixture;
		}

		if (entity.HasComponent<CircleCollider2DComponent>())
		{
			auto& cc = entity.GetComponent<CircleCollider2DComponent>();
			b2CircleShape shape;
			shape.m_p.Set(cc.Offset.x, cc.Offset.y);

			shape.m_radius = (transform.Scale.x) * cc.Radius;

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.density = cc.Density;
			fixtureDef.friction = cc.Friction;
			fixtureDef.restitution = cc.Restitution;
			fixtureDef.restitutionThreshold = cc.RestitutionThreshold;
			fixtureDef.userData.pointer = entity.GetUUID();
			fixtureDef.isSensor = cc.IsSensor;

			b2Fixture* fixture = body->CreateFixture(&fixtureDef);
			cc.RuntimeFixture = fixture;
		}
	}

	void* Scene::StartRuntimeBody(Entity entity)
	{
        EG_PROFILE_FUNCTION();
		auto& transform = entity.GetComponent<TransformComponent>();
		auto& rb = entity.GetComponent<RigidBody2DComponent>();

		b2BodyDef bodyDef;
		bodyDef.type = Utils::RigidBody2DTypeToBox2DBody(rb.Type);
		bodyDef.position.Set(transform.Translation.x, transform.Translation.y);
		bodyDef.angle = transform.Rotation.z;

		b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rb.FixedRotation);
		rb.RuntimeBody = body;

		return body;
	}

	void Scene::OnPhysics2DStart()
	{
        EG_PROFILE_FUNCTION();
		m_PhysicsWorld = new b2World({ 0.0f, -9.81f });

		ContactListener* listener = new ContactListener();

		m_PhysicsWorld->SetContactListener(listener);

		auto view = m_Registry.view<RigidBody2DComponent>();
		for (auto entity : view)
		{
			Entity e{ entity, this };
			auto& transform = e.GetComponent<TransformComponent>();
			auto& rb = e.GetComponent<RigidBody2DComponent>();

			b2Body* body = (b2Body*)StartRuntimeBody(e);
			body->SetGravityScale(rb.GravityMultiplier);


			if (e.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc = e.GetComponent<BoxCollider2DComponent>();
				b2PolygonShape shape;
				shape.SetAsBox(bc.Size.x * transform.Scale.x, bc.Size.y * transform.Scale.y, { bc.Offset.x, bc.Offset.y }, 0.0f);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &shape;
				fixtureDef.density = bc.Density;
				fixtureDef.friction = bc.Friction;
				fixtureDef.restitution = bc.Restitution;
				fixtureDef.restitutionThreshold = bc.RestitutionThreshold;
				fixtureDef.userData.pointer = e.GetUUID();
				fixtureDef.isSensor = bc.IsSensor;

				b2Fixture* fixture = body->CreateFixture(&fixtureDef);
				bc.RuntimeFixture = fixture;
			}

			if (e.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc = e.GetComponent<CircleCollider2DComponent>();
				b2CircleShape shape;
				shape.m_p.Set(cc.Offset.x, cc.Offset.y);

				shape.m_radius = (transform.Scale.x) * cc.Radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &shape;
				fixtureDef.density = cc.Density;
				fixtureDef.friction = cc.Friction;
				fixtureDef.restitution = cc.Restitution;
				fixtureDef.restitutionThreshold = cc.RestitutionThreshold;
				fixtureDef.userData.pointer = e.GetUUID();
				fixtureDef.isSensor = cc.IsSensor;

				b2Fixture* fixture = body->CreateFixture(&fixtureDef);
				cc.RuntimeFixture = fixture;
			}
		}
	}

	void Scene::OnPhysics2DStop()
	{
        EG_PROFILE_FUNCTION();
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
        EG_PROFILE_FUNCTION();
		static_assert(sizeof(T) == 0);
	}



	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{
        EG_PROFILE_FUNCTION();
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
        EG_PROFILE_FUNCTION();
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
        EG_PROFILE_FUNCTION();
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
        EG_PROFILE_FUNCTION();
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
	{
        EG_PROFILE_FUNCTION();
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
        EG_PROFILE_FUNCTION();
	}

	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
	{
        EG_PROFILE_FUNCTION();
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
        EG_PROFILE_FUNCTION();
	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
        EG_PROFILE_FUNCTION();
	}

	template<>
	void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component)
	{
        EG_PROFILE_FUNCTION();
	}

	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{
        EG_PROFILE_FUNCTION();
	}

	template<>
	void Scene::OnComponentAdded<TextComponent>(Entity entity, TextComponent& component)
	{
        EG_PROFILE_FUNCTION();
	}
	template<>
	void Scene::OnComponentAdded<AudioSourceComponent>(Entity entity, AudioSourceComponent& component)
	{
        EG_PROFILE_FUNCTION();
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererSTComponent>(Entity entity, SpriteRendererSTComponent& component)
	{
        EG_PROFILE_FUNCTION();
	}

	template<>
	void Scene::OnComponentAdded<AnimatorComponent>(Entity entity, AnimatorComponent& component)
	{
        EG_PROFILE_FUNCTION();
	}

}
