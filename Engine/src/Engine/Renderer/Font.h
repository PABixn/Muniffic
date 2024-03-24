#pragma once

#include <filesystem>
#include <Engine/Core/Core.h>
#include "Engine/Renderer/Texture.h"

namespace eg
{
	struct MSDFData;

	class Font
	{
	public:
		Font(MSDFData* data, Ref<Texture2D> atlasTexture);
		Font(Font* font);
		~Font();

		MSDFData* GetData() const { return m_Data; }
		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

		static UUID GetDefaultFontUUID() { return s_DefaultFontUUID; }
		static Ref<Font> GetDefaultFont() { return s_DefaultFont; }
		static void LoadDefaultFont(const std::filesystem::path& path);
		static void SetDefaultFont(UUID fontUUID, Ref<Font> font) { s_DefaultFontUUID = fontUUID, s_DefaultFont = font; }

	private:
		MSDFData* m_Data = nullptr;
		Ref<Texture2D> m_AtlasTexture;
		static UUID s_DefaultFontUUID;
		static Ref<Font> s_DefaultFont;
	};
}
