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

	bool ResourceSerializer::DeserializeResourceCache()
	{
		std::filesystem::path textureMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Image);

		YAML::Node textureNode;

		if (!std::filesystem::exists(textureMetadataPath))
		{
			return false;
		}

		try
		{
			textureNode = YAML::LoadFile(textureMetadataPath.string());
		}
		catch(YAML::ParserException e)
		{
			EG_CORE_ERROR("Failed to load .mnmeta file '{0}'\n     {1}", textureMetadataPath, e.what());
			return false;
		}

		if (!textureNode["Resources"])
			return false;

		auto textureResources = textureNode["Resources"];

		if (textureResources)
		{
			for (auto resource : textureResources)
			{
				std::filesystem::path resourcePath = resource["ResourcePath"].as<std::string>();
				std::string imageName = resource["ImageName"].as<std::string>();
				std::string extension = resource["Extension"].as<std::string>();
				int width = resource["Width"].as<int>();
				int height = resource["Height"].as<int>();
				int channels = resource["Channels"].as<int>();

				TextureResourceData* data = new TextureResourceData();
				data->ResourcePath = resourcePath;
				data->ImageName = imageName;
				data->Extension = extension;
				data->Width = width;
				data->Height = height;
				data->Channels = channels;

				CacheTexture(data);
			}
		}
	}

	void ResourceSerializer::SerializeResourceCache()
	{
		std::filesystem::path textureMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Image);
		YAML::Emitter textureOut;

		textureOut << YAML::BeginMap;
		textureOut << YAML::Key << "Resources" << YAML::Value << YAML::BeginSeq;

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

		YAML::EndSeq;
		YAML::EndMap;

		std::ofstream file(textureMetadataPath, std::ios::trunc);
		file << textureOut.c_str();
		file.close();
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