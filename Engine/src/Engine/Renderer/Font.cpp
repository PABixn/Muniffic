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

namespace eg {
	ConsolePanel consolePanel;
	template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
	static Ref<Texture2D> CreateAndCacheAtlas(const std::string& fontName, float fontSize, const std::vector<msdf_atlas::GlyphGeometry>& glyphs, const msdf_atlas::FontGeometry& fontGeometry, uint32_t width, uint32_t height)
	{
		msdf_atlas::GeneratorAttributes attributes;
		attributes.config.overlapSupport = true;
		attributes.scanlinePass = true;
		msdf_atlas::ImmediateAtlasGenerator<S, N, GenFunc, msdf_atlas::BitmapAtlasStorage<T, N>> generator(width, height);
		generator.setAttributes(attributes);
		generator.setThreadCount(8);
		generator.generate(glyphs.data(), (int)glyphs.size());

		msdfgen::BitmapConstRef<T, N> bitmap = (msdfgen::BitmapConstRef<T, N>)generator.atlasStorage();

		TextureSpecification spec;
		spec.Width = bitmap.width;
		spec.Height = bitmap.height;
		spec.Format = ImageFormat::RGB8;
		spec.GenerateMipmaps = false;


		Ref<Texture2D> texture = Texture2D::Create(spec);
		texture->SetData((void*)bitmap.pixels, bitmap.width * bitmap.height * 3);
		return texture;
	}

	Font::Font(const std::filesystem::path& font)
		: m_Data(new MSDFData())
	{
		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
		EG_CORE_ASSERT(ft);
		if (ft)
		{
			std::string fileString = font.string();
			// TODO: msdfgen::loadFontData loads from memory buffer which we'll need to implement
			msdfgen::FontHandle* fontHandle = msdfgen::loadFont(ft, fileString.c_str());
			if (!fontHandle)
			{
				EG_CORE_ERROR("Failed to load font: {0}", font.string());
				consolePanel.Log("Failed to load font: " + font.string(), ConsolePanel::LogType::Error);
				return;
			}

			struct CharsetRange
			{
				uint32_t Begin, End;
			};

			static const CharsetRange charsetRanges[]
			{
				{0x0020, 0x00FF},
			};

			msdf_atlas::Charset charset;

			for (CharsetRange range : charsetRanges)
				for (uint32_t i = range.Begin; i <= range.End; i++)
					charset.add(i);

			double fontScale = 1.0;
			m_Data->FontGeometry = msdf_atlas::FontGeometry(&m_Data->Glyphs);
			int glyphsLoaded = m_Data->FontGeometry.loadCharset(fontHandle, fontScale, charset);

			EG_CORE_INFO("Loaded {} glyphs from font (out of{})", glyphsLoaded, charset.size());
			consolePanel.Log("Loaded " + std::to_string(glyphsLoaded) + " glyphs from font (out of " + std::to_string(charset.size()) + ")", ConsolePanel::LogType::Info);

			double emSize = 40.0;

			msdf_atlas::TightAtlasPacker packer;
			packer.setPixelRange(2.0);
			packer.setMiterLimit(1.0);
			packer.setPadding(0);
			packer.setScale(emSize);
			int remaining = packer.pack(m_Data->Glyphs.data(), (int)m_Data->Glyphs.size());
			EG_CORE_ASSERT(remaining == 0, "Failed to pack glyphs into atlas");

			int width, height;
			packer.getDimensions(width, height);
			emSize = packer.getScale();

#define LCG_MULTIPLIER 6364136223846793005ULL
#define LCG_INCREMENT 1442695040888963407ULL
#define THREAD_COUNT 8
#define DEFAULT_ANGLE_THRESHOLD 3.0
			// if MSDF | MTSDF
			uint64_t coloringSeed = 0;
			bool expensiveColoring = false;
			if (expensiveColoring)
			{
				msdf_atlas::Workload([&glyphs = m_Data->Glyphs, &coloringSeed](int i, int threadNo) -> bool {
					unsigned long long glyphSeed = (LCG_MULTIPLIER * (coloringSeed ^ i) + LCG_INCREMENT) * !!coloringSeed;
					glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
					return true;
					}, m_Data->Glyphs.size()).finish(THREAD_COUNT);
			}
			else {
				unsigned long long glyphSeed = coloringSeed;
				for (msdf_atlas::GlyphGeometry& glyph : m_Data->Glyphs)
				{
					glyphSeed *= LCG_MULTIPLIER;
					glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
				}
			}

			m_AtlasTexture = CreateAndCacheAtlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>("Text", (float)emSize, m_Data->Glyphs, m_Data->FontGeometry, width, height);

			msdfgen::destroyFont(fontHandle);
			msdfgen::deinitializeFreetype(ft);
		}
	}

	Font::~Font()
	{
		delete m_Data;
	}

	Ref<Font> Font::GetDefaultFont()
	{
		static Ref<Font> s_DefaultFont;
		if (!s_DefaultFont)
			s_DefaultFont = CreateRef<Font>("assets/fonts/opensans/OpenSans-Regular.ttf");
		return s_DefaultFont;
	}
}