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

    for (uint32_t i = 0; i < m_IndexBuffer.size(); i++)
    {
        if (m_IndexBuffer[i].m_Buffer.m_Buffer != nullptr)
            m_IndexBuffer[i].destroy();
    }

    if (m_SSBO.m_Buffer.m_Buffer != nullptr)
        m_SSBO.destroy();

    if (m_DescriptorHelper.getDescriptorSetLayout() != nullptr)
        m_DescriptorHelper.cleanup();

    if (m_TextureDescriptorHelper.getDescriptorSetLayout() != nullptr)
        m_TextureDescriptorHelper.cleanup();

    m_Loaded = false;
}

void eg::SceneRenderData::createBuffers(size_t verticesPerObject, size_t numberOfObjects)
{
    m_DescriptorHelper.init();
    m_TextureDescriptorHelper.init();
    m_VertexBuffer.create(verticesPerObject * sizeof(VulkanBasicMeshVertex), VertexType_BasicMesh);
    m_IndexBuffer.resize(numberOfIndexBuffers);
    for (uint32_t i = 0; i< numberOfIndexBuffers;i++)
        m_IndexBuffer[i].create(verticesPerObject * numberOfObjects * sizeof(uint32_t) * 1.5);
    m_SSBO.create(numberOfObjects * sizeof(ObjectMatrixData));
    m_DescriptorHelper.bindSSBO(&m_SSBO);
    m_SamplerArray.create(MAX_SAMPLER_ARRAY_SIZE * sizeof(VkSampler));
    m_TextureDescriptorHelper.bindSamplerArray(&m_SamplerArray);


    std::vector<VulkanBasicMeshVertex> SpritesVertices(4);
    SpritesVertices[0] = { { -0.5f, -0.5f, 0.f },  { 0.f, 0.f, 1.f } , { 1.0f, 0.0f } };
    SpritesVertices[1] = { {0.5f, -0.5f, 0.f},  { 0.f, 0.f, 1.f }, { 0.0f, 0.0f } };
    SpritesVertices[2] = { {0.5f, 0.5f, 0.f}, { 0.f, 0.f, 1.f }, { 0.0f, 1.0f } };
    SpritesVertices[3] = { {-0.5f, 0.5f, 0.f}, { 0.f, 0.f, 1.f }, { 1.0f, 1.0f } };
    memcpy(m_VertexBuffer.m_Mapped, SpritesVertices.data(), SpritesVertices.size() * sizeof(VulkanBasicMeshVertex));
    m_VertexBuffer.m_VerticesCount = 4;
    UpdateDrawCallsInfo();
}


void eg::SceneRenderData::AddObjectToBuffers(ObjectRenderData& objRenderData, void* indicesData, void* matricesData)
{
    for  (uint32_t i = 0;  i < numberOfIndexBuffers;i++)
    {
        if (m_IndexBuffer[i].addBasic2DObjectIndices(&objRenderData, indicesData)) {
            break;
        }

    }
    if (matricesData == nullptr)
    {
        ObjectMatrixData matrixData = {};
        matrixData.color = glm::vec3(1.f, 1.f, 1.f);
        matrixData.model = glm::mat4(1);
        //matrixData.model = glm::transpose(matrixData.model);
        m_SSBO.addBasic2DObjectSRComponentData(&objRenderData, &matrixData);
    }
    else
    {
        m_SSBO.addBasic2DObjectSRComponentData(&objRenderData, matricesData);
    }
    objRenderData.m_Update &= ~RenderUpdate::Created;
}

eg::ObjectRenderData eg::SceneRenderData::addSquare(const glm::vec3 color, const glm::mat4& transform)
{
    ObjectRenderData objRenderData = {};
    objRenderData.m_Update |= RenderUpdate::Created;
    objRenderData.m_VerticesCount = 4;

    std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };
    objRenderData.m_VertexBuffer = &m_VertexBuffer;
    objRenderData.m_VerticesCount = 4;
    objRenderData.m_VertexBufferOffset = 0;
    objRenderData.m_IndicesCount = 6;
    ObjectMatrixData matrixData = { color, 0, transform };
    AddObjectToBuffers(objRenderData, indices.data(), &matrixData);
    return objRenderData;
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
    if (m_SSBO.m_InstancesCount!=0)
    {
        uint32_t lastRenderingEntity = findLastRenderData();
        ObjectRenderData& objThatIsGoingToBeDeleted = m_EntityRenderInfoMap[(uint32_t)entity];
        if ((uint32_t)entity != lastRenderingEntity)
        {
            ObjectRenderData& LastObj = m_EntityRenderInfoMap[(uint32_t)lastRenderingEntity];
            LastObj.m_IndexBufferOffset = objThatIsGoingToBeDeleted.m_IndexBufferOffset;
            LastObj.m_MatricesBufferOffset = objThatIsGoingToBeDeleted.m_MatricesBufferOffset;
            Entity ent((entt::entity)lastRenderingEntity, m_Scene);
            UpdateMatrixData(ent);
            UpdateSpriteRenderComponentData(ent);
        }
        m_SSBO.m_InstancesCount -= 1;
        m_SSBO.m_LastOffset -= sizeof(ObjectMatrixData);
        objThatIsGoingToBeDeleted.m_IndexBuffer->m_IndicesCount -= 6;
        objThatIsGoingToBeDeleted.m_IndexBuffer->m_LastOffset -= 6 * sizeof(uint32_t);        
        m_EntityRenderInfoMap.erase((uint32_t)entity);
    }

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

void eg::SceneRenderData::UpdateMatrixData(const Entity& ent)
{
   glm::mat4 matrixData = m_Scene->m_Registry.get<TransformComponent>(ent).GetTransform();
   ObjectRenderData objRenderData = m_EntityRenderInfoMap[(uint32_t)ent];
   uint32_t offset = offsetof(ObjectMatrixData, model);
   void* updatedMatrixLocation = static_cast<void*>(static_cast<char*>(m_SSBO.m_Mapped) + objRenderData.m_MatricesBufferOffset + offset);
   memcpy(updatedMatrixLocation, &matrixData, sizeof(glm::mat4));
}

void eg::SceneRenderData::UpdateAllMatrixData(Scene* scene)
{
    auto group = scene->m_Registry.view<TransformComponent, SpriteRendererComponent>();
    for (auto entity : group)
    {
        auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
        Entity ent = {entity, nullptr};
        UpdateMatrixData(ent);
        UpdateSpriteRenderComponentData(ent);
        //Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
    }
    
}

void eg::SceneRenderData::UpdateDrawCallsInfo()
{
    m_DrawCallInfos = std::vector<DrawCallsInfo>(1);
    m_DrawCallInfos[0].drawCallCount = 1;
    m_DrawCallInfos[0].graphicsPipeline = VRen::get().getGraphicsPipeline(GraphicsPipelineType::Type_BasicMeshWithColor);
    m_DrawCallInfos[0].vertexBuffer = { &m_VertexBuffer };
    m_DrawCallInfos[0].indexBuffer = { &m_IndexBuffer[0]};
    m_DrawCallInfos[0].instancesCount = { &m_SSBO.m_InstancesCount };
    m_DrawCallInfos[0].descriptorSets = std::vector<std::vector<VkDescriptorSet*>>(2);
    m_DrawCallInfos[0].descriptorSets[0] = { &VRen::get().getResourceManager().getDescriptorSets()[0], &VRen::get().getSceneRenderData().getMatrixBufferDescriptorHelper().m_DescriptorSet };
    m_DrawCallInfos[0].descriptorSets[1] = { &VRen::get().getResourceManager().getDescriptorSets()[1], &VRen::get().getSceneRenderData().getMatrixBufferDescriptorHelper().m_DescriptorSet };


}

eg::UUID eg::SceneRenderData::findLastRenderData() {
    ObjectRenderData* maxRenderData;
    uint64_t maxs = 0;

    for (const auto& [uuid, renderData] : m_EntityRenderInfoMap) {
        if (renderData.m_IndexBufferOffset > maxs) {
            maxs = uuid;
        }
    }
    return maxs;
}