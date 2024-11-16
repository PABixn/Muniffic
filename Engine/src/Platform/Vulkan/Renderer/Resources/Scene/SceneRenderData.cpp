#include "SceneRenderData.h"
#include <vector>
#include "Engine/Scene/Components.h"
void eg::SceneRenderData::loadScene(const entt::registry& entityRegistry)
{
#ifdef MUNI_ENGINE_BUILD 

    // Renderer will store data on vertices the fastest way- assuming scene vertices are changing extremely frequently.

    auto group = entityRegistry.view<SpriteRendererComponent>();
    for (auto entity : group)
    {
        SpriteRendererComponent spriteRenderer = group.get<SpriteRendererComponent>(entity);
        spriteRenderer.m_RenderData = addSquare();
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

void eg::SceneRenderData::createBuffers(size_t verticesPerObject, size_t numberOfObjects)
{
    m_DescriptorHelper.init();
    m_VertexBuffer.create(verticesPerObject * numberOfObjects * sizeof(VulkanBasicMeshVertex), VertexType_BasicMesh);
    m_IndexBuffer.create(verticesPerObject  * numberOfObjects * sizeof(uint32_t)* 1.5);
    m_SSBO.create(numberOfObjects * sizeof(ObjectMatrixData));
    m_DescriptorHelper.bindSSBO(&m_SSBO);
}

eg::ObjectRenderData eg::SceneRenderData::addSquare()
{
    ObjectRenderData objRenderData = {};
    objRenderData.m_VerticesCount = 0;
    objRenderData.m_Update |= RenderUpdate::Created;
    std::vector<VulkanBasicMeshVertex> SpritesVertices(4);
    SpritesVertices[objRenderData.m_VerticesCount] = { { -0.5f, -0.5f, 0.f },  { 0.f, 0.f, 1.f } , { 1.0f, 0.0f } };
    objRenderData.m_VerticesCount += 1;
    SpritesVertices[objRenderData.m_VerticesCount] = { {0.5f, -0.5f, 0.f},  { 0.f, 0.f, 1.f }, { 0.0f, 0.0f } };
    objRenderData.m_VerticesCount += 1;
    SpritesVertices[objRenderData.m_VerticesCount] = { {0.5f, 0.5f, 0.f}, { 0.f, 0.f, 1.f }, { 0.0f, 1.0f } };
    objRenderData.m_VerticesCount += 1;
    SpritesVertices[objRenderData.m_VerticesCount] = { {-0.5f, 0.5f, 0.f}, { 0.f, 0.f, 1.f }, { 1.0f, 1.0f } };
    objRenderData.m_VerticesCount += 1;


    std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };
    for (uint32_t index : indices) {
        index += m_VertexBuffer.m_VerticesCount;
    }
    objRenderData.m_IndicesCount = 6;
    AddObjectToBuffers(objRenderData, SpritesVertices.data(), indices.data());
    return objRenderData;
}

void eg::SceneRenderData::AddObjectToBuffers(ObjectRenderData& objRenderData, void* verticesData, void* indicesData, void* matricesData)
{
    m_VertexBuffer.addBasic2DObjectVertices(&objRenderData, verticesData);
    m_IndexBuffer.addBasic2DObjectIndices(&objRenderData, indicesData);
    if (matricesData == nullptr)
    {
        ObjectMatrixData matrixData = {};
        matrixData.color = glm::vec3(1.f, 1.f, 1.f);
        matrixData.model = glm::mat4(1);
        m_SSBO.addBasic2DObjectModelMatrix(&objRenderData, &matrixData);
    }
    else
    {
        m_SSBO.addBasic2DObjectModelMatrix(&objRenderData, matricesData);
    }
    objRenderData.m_Update &= ~RenderUpdate::Created;
}
