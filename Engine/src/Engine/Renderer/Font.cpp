#include "egpch.h"
#include "Font.h"
#include "Engine/Renderer/Texture.h"
#include "MSDFData.h"
#include "../Engine-Editor/src/Panels/ConsolePanel.h"

#undef INFINITE
#include "FontGeometry.h"
#include "GlyphGeometry.h"
#include "msdf-atlas-gen.h"
#include "msdfgen.h"
#include "Engine/Resources/Systems/ResourceSystem.h"
#include "Engine/Resources/ResourceDatabase.h"
#include "Engine/Resources/AssetDirectoryManager.h"

namespace eg
{
	Font::Font(MSDFData* data, Ref<Texture2D> atlasTexture)
		: m_Data(data), m_AtlasTexture(atlasTexture) {
        EG_PROFILE_FUNCTION();
        }

	Font::Font(Font* font)
		: m_Data(font->m_Data), m_AtlasTexture(font->m_AtlasTexture) {
        EG_PROFILE_FUNCTION();
        }

	Font::~Font()
	{
        EG_PROFILE_FUNCTION();
	}

	UUID Font::s_DefaultFontUUID;

	void Font::LoadFont(const std::filesystem::path& path)
	{
        EG_PROFILE_FUNCTION();
		FontResourceData* data = new FontResourceData();
		data->ParentDirectory = AssetDirectoryManager::GetRootAssetTypeDirectory(ResourceType::Font);
		data->ResourceName = path.stem().string();
		data->Extension = path.extension().string();

		UUID font = ResourceDatabase::AddResource(path.string(), data, ResourceType::Font);
		Font* loaded = (Font*)ResourceDatabase::LoadRuntimeResource(font, ResourceType::Font);
		Font::s_DefaultFontUUID = font;
	}
}
