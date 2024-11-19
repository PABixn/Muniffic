#include "SceneRenderData.h"
#include <vector>
#include "Engine/Scene/Components.h"
#include "Engine/Scene/Entity.h"
void eg::SceneRenderData::loadScene(Scene* scene)
{/*
#ifdef MUNI_ENGINE_BUILD 

    // Renderer will store data on vertices the fastest way- assuming scene vertices are changing frequently.

    auto group = entityRegistry.view<SpriteRendererComponent, TransformComponent>();
    for (auto entity : group)
    {
        SpriteRendererComponent spriteRenderer = group.get<SpriteRendererComponent>(entity);
        
        spriteRenderer.m_RenderData = addSquare(group.get<TransformComponent>(entity).GetTransform());
    }
    m_Loaded = true;
#else
    EG_ASSERT(false);
#endif // MUNI_BUILD_ENGINE*/
    m_Scene = scene;
    m_Loaded = true;
}

void eg::SceneRenderData::unloadScene()
{
    if (m_VertexBuffer.m_Buffer.m_Buffer != nullptr)
        m_VertexBuffer.destroy();

    if (m_IndexBuffer.m_Buffer.m_Buffer != nullptr)
        m_IndexBuffer.destroy();

    if (m_SSBO.m_Buffer.m_Buffer != nullptr)
        m_SSBO.destroy();

    if (m_DescriptorHelper.getDescriptorSetLayout() != nullptr)
        m_DescriptorHelper.cleanup();

    m_Loaded = false;
}

void eg::SceneRenderData::createBuffers(size_t verticesPerObject, size_t numberOfObjects)
{
    m_DescriptorHelper.init();
    m_VertexBuffer.create(verticesPerObject * numberOfObjects * sizeof(VulkanBasicMeshVertex), VertexType_BasicMesh);
    m_IndexBuffer.create(verticesPerObject  * numberOfObjects * sizeof(uint32_t)* 1.5);
    m_SSBO.create(numberOfObjects * sizeof(ObjectMatrixData));
    m_DescriptorHelper.bindSSBO(&m_SSBO);
}

eg::ObjectRenderData eg::SceneRenderData::addSquare(const glm::vec3 color, const glm::mat4& transform)
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
    ObjectMatrixData matrixData = { color, 0, transform };
    AddObjectToBuffers(objRenderData, SpritesVertices.data(), indices.data(), &matrixData);
    return objRenderData;
}

void eg::SceneRenderData::addSquare(Entity& ent,const entt::registry& registry)
{
    TransformComponent tc = registry.get<TransformComponent>(ent);
    m_EntityRenderInfoMap[m_LastRenderData++] = addSquare((ent.HasComponent<SpriteRendererComponent>() ? registry.get<SpriteRendererComponent>(ent).Color : glm::vec3(1.f,1.f,1.f)), tc.GetTransform());
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
        //matrixData.model = glm::transpose(matrixData.model);
        m_SSBO.addBasic2DObjectModelMatrix(&objRenderData, &matrixData);
    }
    else
    {
        m_SSBO.addBasic2DObjectModelMatrix(&objRenderData, matricesData);
    }
    objRenderData.m_Update &= ~RenderUpdate::Created;
}

void eg::SceneRenderData::UpdateMatrixData(Entity& ent, void* upadtedMatricesData)
{
   ObjectRenderData objRenderData = m_EntityRenderInfoMap[(uint32_t)ent];
   //EG_ASSERT(to_bool(objRenderData.m_Update & RenderUpdate::MatrixValueChanged));
   uint32_t offset = offsetof(ObjectMatrixData, model);
   void* updatedMatrixLocation = static_cast<void*>(static_cast<char*>(m_SSBO.m_Mapped) + objRenderData.m_MatricesBufferOffset + offset);
   memcpy(updatedMatrixLocation, upadtedMatricesData, sizeof(glm::mat4));
}

