#pragma once
#include "Texture.h"
#include <glm/glm.hpp>

namespace eg {

	class SubTexture2D {
	public:
		SubTexture2D()
		{
			m_TexCoords[0] = { 0.0f, 0.0f };
			m_TexCoords[1] = { 1.0f, 0.0f };
			m_TexCoords[2] = { 1.0f, 1.0f };
			m_TexCoords[3] = { 0.0f, 1.0f };
			m_Texture = nullptr;
		};

		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
		static Ref<SubTexture2D> Create(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture2D> GetTexture() const { return m_Texture; }
		Ref<Texture2D> GetTexture() { return m_Texture; }

		const glm::vec2* GetTexCoords() const { return m_TexCoords; }
		const glm::vec2* GetCoordsPtr(int index) const { return &m_TexCoords[index]; }
		const glm::vec2 GetCoords(int index) { return m_TexCoords[index]; }
		glm::vec2* GetTexCoords() { return m_TexCoords; }
		const glm::vec2& GetMin() const { return m_TexCoords[0]; }
		const glm::vec2& GetMax() const { return m_TexCoords[2]; }

		inline void SetTexCoords(int index, glm::vec2 coords) { if(index < 4) m_TexCoords[index] = coords; };
		inline void SetTexture(const Ref<Texture2D>& texture) { m_Texture = texture; };
		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1, 1 });
		private:
			Ref<Texture2D> m_Texture;
			glm::vec2 m_TexCoords[4];
			std::filesystem::path m_FilePath;
	};
}