#include "VRenderer.h"
#include "Platform/Vulkan/VulkanRenderer.h"
#include "Engine/Core/Application.h"
#include "Platform/Vulkan/Renderer/Resources/Scene/ObjectRenderData.h"
void eg::VRenderer::Init()
{
	VRen::get().init();
}

void eg::VRenderer::SetEditorCamera(EditorCamera* editor)
{
	VRen::get().getResourceManager().m_DescriptorManager.setEditorCamera(editor);
}

void eg::VRenderer::Render()
{
	VRen::get().render();
}

void eg::VRenderer::Shutdown()
{
	VRen::get().cleanUp();
}

void eg::VRenderer::ImGuiInit()
{
#if WIN32
	GLFWwindow* win = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
#else
	EG_CORE_ASSERT(false);// FOR NOW THE APP IS ONLY FOR WIDNOWS USING GLFW
#endif // WIN32
	VRen::get().initImGui(win);
}

void eg::VRenderer::DeviceWaitIdle()
{
	vkDeviceWaitIdle(VRen::get().getNativeDevice());
}

void eg::VRenderer::OnWindowResize()
{
#if WIN32
	GLFWwindow* win = (GLFWwindow*)Application::Get().GetWindow().GetNativeWindow();
#else
	EG_CORE_ASSERT(false);// FOR NOW THE APP IS ONLY FOR WIDNOWS USING GLFW
#endif // WIN32
	VRen::get().getSwapChain().recreate(win);
}

ImTextureID eg::VRenderer::GetSceneRenderImageID()
{
	return VRen::get().getResourceManager().m_FrameManager.getSceneRendererID();
}

void eg::VRenderer::ResizeViewport(const std::pair<uint32_t, uint32_t>& viewportSize, bool recreate)
{
	if (recreate)VRen::get().m_ResourceManager.m_FrameManager.resizeViewport(viewportSize);
}

void eg::VRenderer::LoadScene(Scene* scene)
{
	VRen::get().LoadSceneData(scene);
}

void eg::VRenderer::FreeEntityRenderData(Entity entity)
{
	VRen::get().m_CurrentSceneRenderData.removeSquare(entity);
}

void eg::VRenderer::UpdateAllMatricesData(Scene* scene) {
	VRen::get().m_CurrentSceneRenderData.UpdateAllMatrixData(scene);
}

void eg::VRenderer::UpdateTransformData(const Entity& entity)
{
	VRen::get().m_CurrentSceneRenderData.UpdateMatrixData(entity);
}

void eg::VRenderer::AddSquare(Entity& entity, entt::registry* reg)
{
	VRen::get().m_CurrentSceneRenderData.addSquare(entity, reg);
}

void eg::VRenderer::UpdateSpriteRenderComponentData(Entity entity, entt::registry* reg)
{
	VRen::get().m_CurrentSceneRenderData.UpdateSpriteRenderComponentData(entity, reg);
}