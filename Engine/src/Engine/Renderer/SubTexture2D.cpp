#include "egpch.h"
#include "SubTexture2D.h"
#include "Engine/Resources/ResourceSerializer.h"

namespace eg {
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
	{
		return CreateRef<SubTexture2D>(texture, min, max);
	}

	Ref<SubTexture2D> SubTexture2D::Create(UUID id)
	{
		SubTextureResourceData* subTexData = ResourceSerializer::SubTextureResourceDataCache.at(id);
		if (!subTexData)
			return nullptr;
		Ref<SubTexture2D> subTex = CreateRef<SubTexture2D>();
		subTex->m_TexCoords[0] = subTexData->TexCoords[0];
		subTex->m_TexCoords[1] = subTexData->TexCoords[1];
		subTex->m_TexCoords[2] = subTexData->TexCoords[2];
		subTex->m_TexCoords[3] = subTexData->TexCoords[3];
		subTex->m_Texture = Texture2D::Create(subTexData->Texture);
		return subTex;
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize /*= { 1, 1 }*/)
	{

		glm::vec2 min = { (coords.x * cellSize.x) / texture->GetWidth(), (coords.y * cellSize.y) / texture->GetHeight() };
		glm::vec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / texture->GetWidth(), ((coords.y + spriteSize.y) * cellSize.y) / texture->GetHeight() };
		return CreateRef<SubTexture2D>(texture, min, max);
	}
}