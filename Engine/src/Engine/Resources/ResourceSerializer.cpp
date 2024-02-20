#include "egpch.h"
#include "Engine/Core/Core.h"
#include "ResourceSerializer.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <optional>
#include <iostream>

namespace eg
{
	void ResourceSerializer::CacheTexture(TextureResourceData& data)
	{
		ResourceCache* cached = new ResourceCache();

		cached->Type = ResourceType::Image;
		cached->data = &data;

		std::filesystem::path keyPath = data.ResourcePath / std::filesystem::path(data.ImageName);

		if (ResourceDataCache.find(keyPath) == ResourceDataCache.end())
		{
			delete ResourceDataCache[keyPath]->data;
			delete ResourceDataCache[keyPath];
			ResourceDataCache.erase(keyPath);
		}

		ResourceDataCache[keyPath] = cached;
	}

	bool ResourceSerializer::ReadCachedTexture(std::filesystem::path& keyPath)
	{
		TextureResourceData* data = new TextureResourceData();

		if (ResourceDataCache.find(keyPath) == ResourceDataCache.end())
			return false;

		ResourceCache* cached = ResourceDataCache[keyPath];
	}
}