#pragma once

#include <glm/glm.hpp>

#include "SceneCamera.h"
#include "Engine/Core/UUID.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/SubTexture2D.h"
#include "Engine/Renderer/Animator.h"
#include "Engine/Renderer/Font.h"
#include "Engine/Audio/BasicAudio.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/exponential.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include "Engine/Resources/ResourceDatabase.h"

#include "Platform/Vulkan/Renderer/Resources/Scene/SceneRenderData.h"

namespace eg
{

	struct Component
	{
		bool isInheritedInChildren = false;
		bool isInherited = false;
		UUID EntityID = 0;
	};

	struct IDComponent
	{
		UUID ID;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
		IDComponent(const UUID& uuid)
			: ID(uuid) {}
	};

	struct TagComponent : Component
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const UUID& entityID) { EntityID = entityID; }
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}
	};

	struct TransformComponent : Component
	{
	//private:
		glm::vec3 Translation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 PrevTranslation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale{ 1.0f, 1.0f, 1.0f };
	//public:
		TransformComponent() { isInheritedInChildren = true; isInherited = true; };
		TransformComponent(const UUID& entityID) { EntityID = entityID; isInheritedInChildren = true; isInherited = true; };
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3 & translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent: Component
	{
		ObjectRenderData m_RenderData;
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		UUID TextureUUID = 0;
		float TilingFactor = 1.0f;


		SpriteRendererComponent() = default;
		SpriteRendererComponent(const UUID& entityID) { EntityID = entityID; }
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)  
			: Color(color) {}
	};

	struct SpriteRendererSTComponent : Component
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<SubTexture2D> SubTexture;
		UUID SubTextureUUID;
		float TilingFactor = 1.0f;

		SpriteRendererSTComponent()
		{
			SubTexture = CreateRef<SubTexture2D>();
		};
		SpriteRendererSTComponent(const UUID& entityID){}
		SpriteRendererSTComponent(const SpriteRendererSTComponent&) = default;
		SpriteRendererSTComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct AnimatorComponent : Component
	{
		AnimatorComponent()
		{
			Animator2D = CreateRef<Animator>();
			Animator2D->SetEntityID(EntityID);
		};
		AnimatorComponent(const UUID& entityID)
		{
			Animator2D = CreateRef<Animator>();
			Animator2D->SetEntityID(entityID);
		};
		AnimatorComponent(const AnimatorComponent& animator)
			: Animator2D(animator.Animator2D) {
			Animator2D->SetEntityID(EntityID);
		};
		AnimatorComponent(const Ref<Animator>& animator)
			: Animator2D(animator) {
			Animator2D->SetEntityID(EntityID);
		}

		Ref<Animator> Animator2D;
	};

	struct CircleRendererComponent: Component
	{
		glm::vec4 Color { 1.0f, 1.0f, 1.0f, 1.0f };
		float Thickness = 1.0f;
		float Fade = 0.005f;
		bool isSensor = false;

		CircleRendererComponent() = default;
		CircleRendererComponent(const UUID& entityID) { EntityID = entityID; }
		CircleRendererComponent(const CircleRendererComponent&) = default;
		CircleRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct CameraComponent: Component
	{
		SceneCamera Camera;
		bool Primary = true; // TODO: think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const UUID& entityID) { EntityID = entityID; }
		CameraComponent(const CameraComponent&) = default;
	};

	struct ScriptComponent: Component
	{
		std::vector<UUID> Scripts;

		ScriptComponent() = default;
		ScriptComponent(const UUID& entityID) { EntityID = entityID; }
		ScriptComponent(const ScriptComponent&) = default;
	};

	class ScriptableEntity;

	struct NativeScriptComponent : Component
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};

	struct EntityTypeComponent
	{
		EntityTypeComponent() = default;
		EntityTypeComponent(const EntityTypeComponent&) = default;
		EntityTypeComponent(int type)
			: Type(type) {}

		int Type;
		operator int() { return Type; }
	};


	//Physics 2D

	struct RigidBody2DComponent: Component
	{
		enum class BodyType
		{
			Static = 0,
			Dynamic = 1,
			Kinematic = 2
		};

		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		//Storage for runtime
		void* RuntimeBody = nullptr;
		float GravityMultiplier = 1.1f;

		RigidBody2DComponent() = default;
		RigidBody2DComponent(const UUID& entityID) { EntityID = entityID; }
		RigidBody2DComponent(const RigidBody2DComponent&) = default;

	};

	struct BoxCollider2DComponent: Component
	{

		glm::vec2 Offset = { 0.0f, 0.0f };
		glm::vec2 Size = { 0.5f, 0.5f };

		// TODO: Move this to a physics material component
		float Density = 1.0f;
		float Friction = 0.5f;
		//Bounciness
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		bool IsSensor = false;

		void* RuntimeFixture = nullptr;
		
		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const UUID& entityID) { EntityID = entityID; }
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

	struct CircleCollider2DComponent: Component
	{
		glm::vec2 Offset = { 0.0f, 0.0f };
		float Radius = 0.5f;

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshold = 0.5f;

		bool IsSensor = false;

		//Storage for runtime
		void* RuntimeFixture = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const UUID& entityID) { EntityID = entityID; }
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;

		#pragma region Functionality

		#pragma endregion
	};

	struct TextComponent : Component
	{
		std::string TextString;
		UUID FontAsset = Font::GetDefaultFontUUID();
		Ref<Font> RuntimeFont = ResourceDatabase::GetFontRuntimeResource(Font::GetDefaultFontUUID());
		glm::vec4 Color { 1.0f };
		float Kerning = 0.0f;
		float LineSpacing = 0.0f;

		TextComponent() = default;
		TextComponent(const UUID& entityID) { EntityID = entityID; }
	};

	struct AudioSourceComponent : Component
	{
		AudioSourceComponent()
		{
			Audio = CreateRef<BasicAudio>();
		};
		AudioSourceComponent(const UUID& entityID)
		{
			Audio = CreateRef<BasicAudio>();
			EntityID = entityID;
		};
		AudioSourceComponent(const AudioSourceComponent&) = default;
		AudioSourceComponent(const Ref<BasicAudio>& audio)
			: Audio(audio) {}

		Ref<BasicAudio> Audio;
		UUID AudioUUID = 0;
	};

	template<typename... Component>
	struct ComponentGroup
	{
	};

	using InheritableComponents =
		ComponentGroup<TransformComponent, SpriteRendererComponent,
		CircleRendererComponent, CameraComponent, ScriptComponent,
		RigidBody2DComponent, BoxCollider2DComponent,
		CircleCollider2DComponent, TextComponent, AnimatorComponent, AudioSourceComponent>;

	using AllComponents =
		ComponentGroup<TransformComponent, SpriteRendererComponent,
		CircleRendererComponent, CameraComponent, ScriptComponent,
		NativeScriptComponent, RigidBody2DComponent, BoxCollider2DComponent,
		CircleCollider2DComponent, TextComponent, SpriteRendererSTComponent,
		AnimatorComponent, AudioSourceComponent>;
}