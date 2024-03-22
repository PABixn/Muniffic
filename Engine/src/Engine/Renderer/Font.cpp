#include "egpch.h"
#include "Font.h"
#include "Engine/Renderer/Texture.h"
#include "MSDFData.h"

#undef INFINITE
#include "FontGeometry.h"
#include "GlyphGeometry.h"
#include "msdf-atlas-gen.h"
#include "msdfgen.h"
#include "Engine/Resources/Systems/ResourceSystem.h"
#include "Engine/Resources/ResourceDatabase.h"

namespace eg
{
	Font::Font(MSDFData* data, Ref<Texture2D> atlasTexture)
		: m_Data(data), m_AtlasTexture(atlasTexture) {}

	Font::~Font()
	{
		delete m_Data;
	}

	Ref<Font> Font::s_DefaultFont;

	void Font::LoadDefaultFont(const std::filesystem::path& path)
	{
		FontResourceData* data = new FontResourceData();
		data->ResourcePath = "Fonts";
		data->FontName = path.stem().string();
		data->Extension = path.extension().string();

		UUID font = ResourceDatabase::AddResource(path.string(), data, ResourceType::Font);
		Font* loaded = (Font*)ResourceDatabase::LoadRuntimeResource(font, ResourceType::Font);
		Font::s_DefaultFont = CreateRef<Font>(loaded->GetData(), loaded->GetAtlasTexture());
	}
}