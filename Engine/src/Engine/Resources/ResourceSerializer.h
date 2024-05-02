#pragma once

#include <filesystem>
#include "resourceTypes.h"
#include <string>
#include "Engine/Core/UUID.h"

namespace eg {

	class ResourceSerializer
	{
	public:
		friend class ResourceDatabase;

		ResourceSerializer() = default;
		~ResourceSerializer() = default;

		static void CacheTexture(UUID uuid, TextureResourceData* data);
		static void CacheSubTexture(UUID uuid, SubTextureResourceData* data);
		static void CacheAnimation(UUID uuid, AnimationResourceData* data);
		static void CacheSpriteAtlas(UUID uuid, SpriteAtlasResourceData* data);
		static void CacheFont(UUID uuid, FontResourceData* data);
		static void CacheAudio(UUID uuid, AudioResourceData* data);
		static void SerializeResourceCache();
		static bool DeserializeResourceCache();

	private:
		static std::unordered_map<UUID, TextureResourceData*> TextureResourceDataCache;
		static std::unordered_map<UUID, AnimationResourceData*> AnimationResourceDataCache;
		static std::unordered_map<UUID, SpriteAtlasResourceData*> SpriteAtlasResourceDataCache;
		static std::unordered_map<UUID, SubTextureResourceData*> SubTextureResourceDataCache;
		static std::unordered_map<UUID, FontResourceData*> FontResourceDataCache;
		static std::unordered_map<UUID, AudioResourceData*> AudioResourceDataCache;
		static std::unordered_map<UUID, ResourceType> ResourceTypeInfo;
	};
}