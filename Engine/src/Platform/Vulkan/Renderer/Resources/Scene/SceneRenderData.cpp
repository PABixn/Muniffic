#include "SceneRenderData.h"
#include <vector>
#include "Engine/Scene/Components.h"
void eg::SceneRenderData::loadScene(const entt::registry& entityRegistry)
{
#ifdef MUNI_ENGINE_BUILD 
    m_VertexBuffer.create(50 * sizeof(VulkanVertex));
    m_IndexBuffer.create(75 * sizeof(uint32_t));
    // Renderer will store data on vertices the fastest way- assuming scene vertices are changing extremely frequently.

    auto group = entityRegistry.view<SpriteRendererComponent>();
    for (auto entity : group)
    {
        SpriteRendererComponent spriteRenderer = group.get<SpriteRendererComponent>(entity);
        spriteRenderer.m_RenderData = addSquare(spriteRenderer.Color);
    }
    m_Loaded = true;
#else
    EG_ASSERT(false);
#endif // MUNI_BUILD_ENGINE
}

void eg::SceneRenderData::unloadScene()
{
    if (m_VertexBuffer.m_Buffer.m_Buffer != nullptr)
        m_VertexBuffer.destroy();

    if (m_IndexBuffer.m_Buffer.m_Buffer != nullptr)
        m_IndexBuffer.destroy();
}

eg::ObjectRenderData eg::SceneRenderData::addSquare(const glm::vec3& color)
{
    ObjectRenderData objRenderData = {};
    objRenderData.m_VerticesCount = 0;
    objRenderData.m_Update = RenderUpdate_Created;
    std::vector<VulkanVertex> SpritesVertices(4);

    SpritesVertices[objRenderData.m_VerticesCount] = { { -0.5f, -0.5f }, color };
    objRenderData.m_VerticesCount += 1;
    SpritesVertices[objRenderData.m_VerticesCount] = { {0.5f, -0.5f}, color };
    objRenderData.m_VerticesCount += 1;
    SpritesVertices[objRenderData.m_VerticesCount] = { {0.5f, 0.5f}, color };
    objRenderData.m_VerticesCount += 1;
    SpritesVertices[objRenderData.m_VerticesCount] = { {-0.5f, 0.5f}, color };
    objRenderData.m_VerticesCount += 1;

    m_VertexBuffer.addBasic2DObjectVertices(&objRenderData, SpritesVertices.data());

    std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };
    for (uint32_t index : indices) {
        index += m_VertexBuffer.m_VerticesCount;
    }
    objRenderData.m_IndicesCount = 6;
    m_IndexBuffer.addBasic2DObjectIndices(&objRenderData, indices.data());

    return objRenderData;
}
