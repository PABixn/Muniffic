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
		~Font();

		MSDFData* GetData() const { return m_Data; }
		Ref<Texture2D> GetAtlasTexture() const { return m_AtlasTexture; }

		static Ref<Font> GetDefaultFont() { return s_DefaultFont; }
		static void LoadDefaultFont(const std::filesystem::path& path);

	private:
		MSDFData* m_Data = nullptr;
		Ref<Texture2D> m_AtlasTexture;
		static Ref<Font> s_DefaultFont;
	};
}
