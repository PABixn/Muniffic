#pragma once
#include <filesystem>
#include "resourceTypes.h"
#include <string>

namespace eg {

	class ResourceSerializer
	{
	public:
		ResourceSerializer() = default;
		~ResourceSerializer() = default;

		static void CacheTexture(TextureResourceData* data);
		static TextureResourceData* ReadCachedTexture(std::filesystem::path& keyPath);
		static void SerializeResourceCache();
		static bool DeserializeResourceCache();
		static void DeleteCachedResource(const std::filesystem::path& keyPath, ResourceType resourceType, bool deleteFile);

		static std::unordered_map<std::filesystem::path, TextureResourceData*> TextureResourceDataCache;
	};
}