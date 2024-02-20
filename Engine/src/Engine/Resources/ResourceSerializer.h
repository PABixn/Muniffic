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

		void CacheTexture(TextureResourceData* data);
		TextureResourceData* ReadCachedTexture(std::filesystem::path& keyPath);
		static void SerializeResourceCache();

	private:
		static std::unordered_map<std::filesystem::path, TextureResourceData*> TextureResourceDataCache;
	};
}