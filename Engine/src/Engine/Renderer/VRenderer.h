#pragma once
#include "EditorCamera.h"
#include "entt.hpp"
typedef void* ImTextureID;
namespace eg {
	struct extent {
		uint32_t width;
		uint32_t height;
	};
	// Class VRenderer is a wrapper. Static members communicate with the actual Renderer
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
		static void LoadScene(const entt::registry& m_Registry);
	private:
		static extent s_Extent;

	};
}