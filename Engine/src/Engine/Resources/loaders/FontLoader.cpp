#include "egpch.h"
#include "FontLoader.h"
#include "Engine/Renderer/MSDFData.h"
#include "Engine/Renderer/Texture.h"

#include "FontGeometry.h"
#include "GlyphGeometry.h"
#include "msdf-atlas-gen.h"
#include "msdfgen.h"
#include "Engine/Renderer/Font.h"

namespace eg
{
	template<typename T, typename S, int N, msdf_atlas::GeneratorFunction<S, N> GenFunc>
	static Ref<Texture2D> CreateAndCacheAtlas(const std::string& fontName, float fontSize, const std::vector<msdf_atlas::GlyphGeometry>& glyphs, const msdf_atlas::FontGeometry& fontGeometry, uint32_t width, uint32_t height)
	{
        EG_PROFILE_FUNCTION();
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

	bool fontLoaderLoad(ResourceLoader* loader, std::string name, Resource* outResource)
	{
        EG_PROFILE_FUNCTION();
		if (!loader || name.empty() || !outResource)
		{
			return false;
		}

		MSDFData* data = new MSDFData();
		Ref<Texture2D> atlasTexture;

		std::filesystem::path font = name;

		msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
		EG_CORE_ASSERT(ft);
		if (ft)
		{
			std::string fileString = font.string();
			msdfgen::FontHandle* fontHandle = msdfgen::loadFont(ft, fileString.c_str());
			if (!fontHandle)
			{
				EG_CORE_ERROR("Failed to load font: {0}", font.string());
				return false;
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
			data->FontGeometry = msdf_atlas::FontGeometry(&data->Glyphs);
			int glyphsLoaded = data->FontGeometry.loadCharset(fontHandle, fontScale, charset);

			//EG_CORE_INFO("Loaded {} glyphs from font (out of{})", glyphsLoaded, charset.size());

			double emSize = 40.0;

			msdf_atlas::TightAtlasPacker packer;
			packer.setPixelRange(2.0);
			packer.setMiterLimit(1.0);
			packer.setPadding(0);
			packer.setScale(emSize);
			int remaining = packer.pack(data->Glyphs.data(), (int)data->Glyphs.size());
			EG_CORE_ASSERT(remaining == 0, "Failed to pack glyphs into atlas");

			int width, height;
			packer.getDimensions(width, height);
			emSize = packer.getScale();

#define LCG_MULTIPLIER 6364136223846793005ULL
#define LCG_INCREMENT 1442695040888963407ULL
#define THREAD_COUNT 8
#define DEFAULT_ANGLE_THRESHOLD 3.0

			uint64_t coloringSeed = 0;
			bool expensiveColoring = false;
			if (expensiveColoring)
			{
				msdf_atlas::Workload([&glyphs = data->Glyphs, &coloringSeed](int i, int threadNo) -> bool {
					unsigned long long glyphSeed = (LCG_MULTIPLIER * (coloringSeed ^ i) + LCG_INCREMENT) * !!coloringSeed;
					glyphs[i].edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
					return true;
				}, data->Glyphs.size()).finish(THREAD_COUNT);
			}
			else {
				unsigned long long glyphSeed = coloringSeed;
				for (msdf_atlas::GlyphGeometry& glyph : data->Glyphs)
				{
					glyphSeed *= LCG_MULTIPLIER;
					glyph.edgeColoring(msdfgen::edgeColoringInkTrap, DEFAULT_ANGLE_THRESHOLD, glyphSeed);
				}
			}

			atlasTexture = CreateAndCacheAtlas<uint8_t, float, 3, msdf_atlas::msdfGenerator>("Text", (float)emSize, data->Glyphs, data->FontGeometry, width, height);

			msdfgen::destroyFont(fontHandle);
			msdfgen::deinitializeFreetype(ft);
		}

		Font* fontResourceData = new Font(data, atlasTexture);

		outResource->Data = fontResourceData;
		outResource->DataSize = sizeof(Font);
		outResource->Name = font.filename().string();
		outResource->Path = font.parent_path().string();

		EG_CORE_INFO("Loaded font: {0}", outResource->Name);
		return true;
	}

	void fontLoaderUnload(ResourceLoader* loader, Resource* resource)
	{
        EG_PROFILE_FUNCTION();
		if (!loader || !resource)
		{
			EG_ERROR("Invalid resource or loader passed to fontLoaderUnload");
			return;
		}

		if (resource->Data)
		{
			delete resource->Data;
			resource->Data = 0;
			resource->DataSize = 0;
			resource->LoaderId = 0;
		}
	}

	ResourceLoader fontResourceLoaderCreate()
	{
        EG_PROFILE_FUNCTION();
		ResourceLoader loader;
		loader.Type = ResourceType::Font;
		loader.load = fontLoaderLoad;
		loader.unload = fontLoaderUnload;
		loader.TypePath = "Fonts";

		return loader;
	}
}
