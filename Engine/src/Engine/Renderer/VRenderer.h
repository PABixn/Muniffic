#pragma once
#include "EditorCamera.h"
#include "entt.hpp"
#include "Engine/Scene/Entity.h"
typedef void* ImTextureID;
namespace eg {
	class TransformComponent;
	class ObjectRenderData;
	class Scene;
	struct extent {
		uint32_t width;
		uint32_t height;
	};
	// Class VRenderer is a wrapper. Static members to communicate with the actual Renderer
	class VRenderer
	{
	public:
		static void Init();
		static void SetEditorCamera(EditorCamera* editor);
		static void Render();
		static void Shutdown();
		static void ImGuiInit();
		static void DeviceWaitIdle();
		static void OnWindowResize();
		static ImTextureID GetSceneRenderImageID();
		static void ResizeViewport(const std::pair<uint32_t, uint32_t>& viewportSize, bool recreate);
		static void LoadScene(Scene* scene);
		static void UpdateTransformData(Entity& renderData, const TransformComponent& transform);
		static void UpdateTransformData(Entity& entity);
		static void AddSquare(Entity& entity, const entt::registry& sceneRegistry);
	private:
		static extent s_Extent;

	};
}