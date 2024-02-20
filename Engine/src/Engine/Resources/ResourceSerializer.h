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

		void CacheTexture(TextureResourceData& data);
		bool ReadCachedTexture(std::filesystem::path& keyPath);

	private:
		static std::unordered_map<std::filesystem::path, ResourceCache*> ResourceDataCache;
	};
}