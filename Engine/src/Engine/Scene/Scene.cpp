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

namespace eg {
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}
	
	

	template<typename... Component>
	static void CopyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
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
		CopyComponent<Component...>(dst, src, enttMap);
	}

	template<typename... Component>
	static void CopyComponentIfExists(Entity dst, Entity src)
	{
		([&]()
			{
				if (src.HasComponent<Component>())
					dst.AddOrReplaceComponent<Component>(src.GetComponent<Component>());
			}(), ...);
	}

	template<typename... Component>
	static void CopyComponentIfExists(ComponentGroup<Component...>, Entity dst, Entity src)
	{
		CopyComponentIfExists<Component...>(dst, src);
	}

	Ref<Scene> Scene::Copy(Ref<Scene>& other)
	{
		Ref<Scene> scene = CreateRef<Scene>();
		scene->m_ViewportWidth = other->m_ViewportWidth;
		scene->m_ViewportHeight = other->m_ViewportHeight;

		std::unordered_map<UUID, entt::entity> enttMap;
		
		auto& srcSceneRegistry = other->m_Registry;
		auto& dstSceneRegistry = scene->m_Registry;
		auto idView = srcSceneRegistry.view<IDComponent>();

		// Copy entities
		for(auto entity : idView)
		{
			UUID uuid = srcSceneRegistry.get<IDComponent>(entity).ID;
			const auto& tag = srcSceneRegistry.get<TagComponent>(entity).Tag;
			Entity newEntity = scene->CreateEntityWithID(uuid, tag);
			enttMap[uuid] = (entt::entity)newEntity;
			scene->m_EntityInfoMap[uuid] = other->m_EntityInfoMap[uuid];
		}

		// Copy components (except IDComponent and TagComponent)
		CopyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);
		ConsolePanel::Log("File: Scene.cpp - Scene copied", ConsolePanel::LogType::Info);
		return scene;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		return CreateEntityWithID(UUID(), name);
	}

	Entity Scene::CreateEntityWithID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		m_EntityMap[uuid] = (entt::entity)entity;
		m_EntityInfoMap[uuid] = new EntityInfo(NULL);
		ConsolePanel::Log("File: Scene.cpp - Entity created: " + tag.Tag, ConsolePanel::LogType::Info);
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		if (entity.GetParent().has_value())
			entity.GetParent().value().RemoveChild(entity);
		m_EntityMap.erase(entity.GetUUID());
		m_EntityInfoMap.erase(entity.GetUUID());
		m_Registry.destroy(entity);
		ConsolePanel::Log("File: Scene.cpp - Entity destroyed", ConsolePanel::LogType::Info);
	}

	void Scene::OnRuntimeStart()
	{
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
	}

	void Scene::OnRuntimeStop()
	{
		OnPhysics2DStop();
		m_IsRunning = false;

		ScriptEngine::OnRuntimeStop();
	}

	void Scene::OnSimulationStart()
	{
		OnPhysics2DStart();
	}

	void Scene::OnSimulationStop()
	{
		OnPhysics2DStop();
	}

	

	

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& camera)
	{
		RenderScene(camera);
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
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
				auto group = m_Registry.view<SpriteRendererSTComponent, AnimatorComponent>();
				for (auto entity : group)
				{
					auto [sprite, animator] = group.get<SpriteRendererSTComponent, AnimatorComponent>(entity);
					sprite.SubTexture = animator.Animator2D->GetCurrentAnimation()->GetFrame();
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

	}

	void Scene::OnUpdateSimulation(Timestep ts, EditorCamera& camera)
	{
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

		////Set Animations
		//{
		//	auto group = m_Registry.group<SpriteRendererSTComponent, AnimatorComponent>();
		//	for (auto entity : group)
		//	{
		//		auto [sprite, animator] = group.get<SpriteRendererSTComponent, AnimatorComponent>(entity);
		//		sprite.SubTexture = animator.Animator2D->GetCurrentAnimation()->GetFrame();
		//	}
		//}

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

	void Scene::RenderAxis()
	{
		//X: red
		//y: green
		//z: blue
		
	}

	Entity Scene::DuplicateEntity(Entity entity)
	{
		std::string name = entity.GetName();
		Entity newEntity = CreateEntity(name);
		CopyComponentIfExists(AllComponents{}, newEntity, entity);
		return newEntity;
	}

	Entity Scene::FindEntityByName(const std::string_view& name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const auto& tag = view.get<TagComponent>(entity).Tag;
			if (tag == name)
				return Entity{ entity, this };
		}
		return {};
	}

	Entity Scene::GetEntityByUUID(UUID uuid)
	{
		//TODO: Maybe should assert
		if(m_EntityMap.find(uuid) != m_EntityMap.end())
			return Entity{ m_EntityMap[uuid], this };
		return {};
	}

	bool Scene::EntityExists(UUID uuid)
	{
		return m_EntityMap.find(uuid) != m_EntityMap.end();
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
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
		m_StepFrames = frames;
	}

	void Scene::AwakeRuntimeBody(Entity entity)
	{
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

			body->CreateFixture(&fixtureDef);
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
			body->CreateFixture(&fixtureDef);
		}
	}

	void* Scene::StartRuntimeBody(Entity entity)
	{
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
		m_PhysicsWorld = new b2World({ 0.0f, -9.81f });
		auto view = m_Registry.view<RigidBody2DComponent>();
		for (auto entity : view)
		{
			Entity e{ entity, this };
			auto& transform = e.GetComponent<TransformComponent>();

			b2Body* body = (b2Body*)StartRuntimeBody(e);

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

				body->CreateFixture(&fixtureDef);
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
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void Scene::OnPhysics2DStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(sizeof(T) == 0);
	}

	

	template<>
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<TextComponent>(Entity entity, TextComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererSTComponent>(Entity entity, SpriteRendererSTComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<AnimatorComponent>(Entity entity, AnimatorComponent& component)
	{
	}

}