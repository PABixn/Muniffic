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

namespace eg
{
	Font::Font(MSDFData* data, Ref<Texture2D> atlasTexture)
		: m_Data(data), m_AtlasTexture(atlasTexture) {}

	Font::~Font()
	{
		delete m_Data;
	}

	Ref<Font> Font::GetDefaultFont()
	{
		Resource* loadedResource = new Resource();
		bool resourceLoad = resourceSystemLoad("assets/fonts/opensans/OpenSans-Regular.ttf", ResourceType::Font, loadedResource);

		if (!resourceLoad)
		{
			EG_CORE_ERROR("Failed to load default font");
			return nullptr;
		}

		Font* font = (Font*)loadedResource->Data;

		return CreateRef<Font>(font->m_Data, font->m_AtlasTexture);
	}
}