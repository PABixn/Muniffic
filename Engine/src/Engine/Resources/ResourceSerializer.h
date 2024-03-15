#pragma once

#include <filesystem>
#include "resourceTypes.h"
#include <string>
#include "Engine/Core/UUID.h"

namespace eg {

	class ResourceSerializer
	{
	public:
		ResourceSerializer() = default;
		~ResourceSerializer() = default;

		static void CacheTexture(UUID uuid, TextureResourceData* data);
		static void CacheSubTexture(UUID uuid, SubTextureResourceData* data);
		static void CacheAnimation(UUID uuid, AnimationResourceData* data);
		static void CacheSpriteAtlas(UUID uuid, SpriteAtlasResourceData* data);
		static void SerializeResourceCache();
		static bool DeserializeResourceCache();

		static std::unordered_map<UUID, TextureResourceData*> TextureResourceDataCache;
		static std::unordered_map<UUID, AnimationResourceData*> AnimationResourceDataCache;
		static std::unordered_map<UUID, SpriteAtlasResourceData*> SpriteAtlasResourceDataCache;
		static std::unordered_map<UUID, SubTextureResourceData*> SubTextureResourceDataCache;
		static std::unordered_map<UUID, ResourceType> ResourceTypeInfo;
	};
}