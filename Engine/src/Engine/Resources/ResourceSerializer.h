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
		static void CacheAnimation(UUID uuid, AnimationResourceData* data);
		//static TextureResourceData* ReadCachedTexture(std::filesystem::path& keyPath);
		static void SerializeResourceCache();
		static bool DeserializeResourceCache();

		static std::unordered_map<UUID, TextureResourceData*> TextureResourceDataCache;
		static std::unordered_map<UUID, AnimationResourceData*> AnimationResourceDataCache;
		static std::unordered_map<UUID, ResourceType> ResourceTypeInfo;
	};
}