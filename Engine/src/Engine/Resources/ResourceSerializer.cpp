#include "egpch.h"
#include "Engine/Core/Core.h"
#include "ResourceSerializer.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <optional>
#include <iostream>
#include "ResourceUtils.h"

namespace eg
{
	std::unordered_map<std::filesystem::path, TextureResourceData*> ResourceSerializer::TextureResourceDataCache;

	void ResourceSerializer::SerializeResourceCache()
	{
		std::filesystem::path textureMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Image);
		YAML::Emitter textureOut;

		for(auto& [key, value] : TextureResourceDataCache)
		{
			textureOut << YAML::BeginMap;
			textureOut << YAML::Key << "ResourcePath" << YAML::Value << value->ResourcePath.string();
			textureOut << YAML::Key << "ImageName" << YAML::Value << value->ImageName;
			textureOut << YAML::Key << "Extension" << YAML::Value << value->Extension;
			textureOut << YAML::Key << "Width" << YAML::Value << value->Width;
			textureOut << YAML::Key << "Height" << YAML::Value << value->Height;
			textureOut << YAML::Key << "Channels" << YAML::Value << value->Channels;
			textureOut << YAML::EndMap;
		}

		std::ofstream file(textureMetadataPath);
		file << textureOut.c_str();
	}

	void ResourceSerializer::CacheTexture(TextureResourceData* data)
	{
		std::filesystem::path keyPath = data->ResourcePath / std::filesystem::path(data->ImageName);

		if (TextureResourceDataCache.find(keyPath) != TextureResourceDataCache.end())
		{
			delete TextureResourceDataCache[keyPath];
			TextureResourceDataCache.erase(keyPath);
		}

		TextureResourceDataCache[keyPath] = data;
	}

	TextureResourceData* ResourceSerializer::ReadCachedTexture(std::filesystem::path& keyPath)
	{
		if (TextureResourceDataCache.find(keyPath) == TextureResourceDataCache.end())
			return nullptr;

		return TextureResourceDataCache[keyPath];
	}
}