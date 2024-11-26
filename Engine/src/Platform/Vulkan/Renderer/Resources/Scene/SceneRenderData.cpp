#include "SceneRenderData.h"
#include <vector>
#include "Engine/Scene/Components.h"
#include "Engine/Scene/Entity.h"
#include "VulkanRenderer.h"
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

    m_FreeObjectRenderDataBlocks.sizeOfVerticesPerObject = verticesPerObject * sizeof(VulkanBasicMeshVertex);
    m_FreeObjectRenderDataBlocks.sizeOfIndicesPerObject = verticesPerObject * sizeof(uint32_t) * 1.5;
    m_FreeObjectRenderDataBlocks.sizeOfMatrixDataPerObject = sizeof(ObjectMatrixData);
    m_FreeObjectRenderDataBlocks.freeBlocks = std::vector<ObjectRenderData>(0);
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

eg::ObjectRenderData eg::SceneRenderData::addSquare(const glm::vec3 color, const glm::mat4& transform)
{
    if (m_FreeObjectRenderDataBlocks.freeBlocks.size() == 0) { // we need to create the obj render data at the end of each buffer 
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
        for (size_t i = 0; i < indices.size();i++) {
            indices[i] += m_VertexBuffer.m_VerticesCount;
        }
        objRenderData.m_IndicesCount = 6;
        ObjectMatrixData matrixData = { color, 0, transform };
        AddObjectToBuffers(objRenderData, SpritesVertices.data(), indices.data(), &matrixData);
        return objRenderData;
    }
    else // we take the already existing render data space in buffers and fill it in with the data we need
    {
        ObjectRenderData objRenderData = m_FreeObjectRenderDataBlocks.freeBlocks.back();
        m_FreeObjectRenderDataBlocks.freeBlocks.pop_back();

        objRenderData.m_VerticesCount = 0;
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
            index += objRenderData.m_FirstVertexCount;
        }
        objRenderData.m_IndicesCount = 6;
        ObjectMatrixData matrixData = { color, 0, transform };

        {
            void* where = (void*)((char*)objRenderData.m_IndexBuffer->m_Mapped + objRenderData.m_IndexBufferOffset);
            memcpy(where, indices.data(), indices.size() * sizeof(indices[0]));
            void* where1 = (void*)((char*)objRenderData.m_VertexBuffer->m_Mapped + objRenderData.m_VertexBufferOffset);
            memcpy(where1, SpritesVertices.data(), SpritesVertices.size() * sizeof(SpritesVertices[0]));
            void* updatedMatrixLocation = (void*)((char*)objRenderData.m_MatricesBuffer->m_Mapped + objRenderData.m_MatricesBufferOffset);
            memcpy(updatedMatrixLocation, &matrixData, sizeof(ObjectMatrixData));
        }
        return objRenderData;
    }
}

void eg::SceneRenderData::addSquare(Entity& ent, entt::registry* registry)
{
    EG_ASSERT(ent.HasComponent<SpriteRendererComponent>());
    if (!registry)
    {
        registry = &m_Scene->m_Registry;
    }
    TransformComponent tc = registry->get<TransformComponent>(ent);
    m_EntityRenderInfoMap[(uint32_t)ent] = addSquare( registry->get<SpriteRendererComponent>(ent).Color, tc.GetTransform());
}

void eg::SceneRenderData::removeSquare(Entity entity)
{
    ObjectRenderData objRenderData = m_EntityRenderInfoMap[(uint32_t)entity];
    m_FreeObjectRenderDataBlocks.freeBlocks.push_back(objRenderData);
    void* ptr = (void*)((char*)objRenderData.m_IndexBuffer->m_Mapped + objRenderData.m_IndexBufferOffset);
    memset(ptr, -1, sizeof(uint32_t)*objRenderData.m_IndicesCount);
    void* ptr1 = (void*)((char*)objRenderData.m_VertexBuffer->m_Mapped + objRenderData.m_VertexBufferOffset);
    memset(ptr1, 0, sizeof(VulkanBasicMeshVertex) * objRenderData.m_VerticesCount);
    m_EntityRenderInfoMap.erase((uint32_t)entity);
}

void eg::SceneRenderData::UpdateSpriteRenderComponentData(Entity& ent, entt::registry* registry)
{
    if (!registry)
    {
        registry = &m_Scene->m_Registry;
    }
    glm::vec3 colorus = registry->get<SpriteRendererComponent>(ent).Color;
    ObjectRenderData objRenderData = m_EntityRenderInfoMap[(uint32_t)ent];
    void* updatedMatrixLocation = static_cast<void*>(static_cast<char*>(m_SSBO.m_Mapped) + objRenderData.m_MatricesBufferOffset);
    memcpy(updatedMatrixLocation, &colorus, sizeof(glm::vec3));

}

void eg::SceneRenderData::UpdateMatrixData(Entity& ent)
{
   glm::mat4 matrixData = m_Scene->m_Registry.get<TransformComponent>(ent).GetTransform();
   ObjectRenderData objRenderData = m_EntityRenderInfoMap[(uint32_t)ent];
   uint32_t offset = offsetof(ObjectMatrixData, model);
   void* updatedMatrixLocation = static_cast<void*>(static_cast<char*>(m_SSBO.m_Mapped) + objRenderData.m_MatricesBufferOffset + offset);
   memcpy(updatedMatrixLocation, &matrixData, sizeof(glm::mat4));
}
