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
	std::unordered_map<UUID, TextureResourceData*> ResourceSerializer::TextureResourceDataCache;
	std::unordered_map<UUID, ResourceType> ResourceSerializer::ResourceTypeInfo;

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
				UUID uuid = resource["UUID"].as<uint64_t>();

				TextureResourceData* data = new TextureResourceData();
				data->ResourcePath = resource["ResourcePath"].as<std::string>();
				data->ImageName = resource["ImageName"].as<std::string>();
				data->Extension = resource["Extension"].as<std::string>();
				data->Width = resource["Width"].as<int>();
				data->Height = resource["Height"].as<int>();
				data->Channels = resource["Channels"].as<int>();
				data->IsSubTexture = resource["IsSubTexture"].as<bool>();

				if (data->IsSubTexture)
				{
					auto texCoords = resource["TexCoords"];
					for (int i = 0; i < 4; i++)
					{
						auto coord = texCoords[i];
						data->m_TexCoords[i] = { coord["x"].as<float>(), coord["y"].as<float>() };
					}
				}

				CacheTexture(uuid, data);
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
			textureOut << YAML::Key << "UUID" << YAML::Value << key;
			textureOut << YAML::Key << "ResourcePath" << YAML::Value << value->ResourcePath.string();
			textureOut << YAML::Key << "ImageName" << YAML::Value << value->ImageName;
			textureOut << YAML::Key << "Extension" << YAML::Value << value->Extension;
			textureOut << YAML::Key << "Width" << YAML::Value << value->Width;
			textureOut << YAML::Key << "Height" << YAML::Value << value->Height;
			textureOut << YAML::Key << "Channels" << YAML::Value << value->Channels;
			textureOut << YAML::Key << "IsSubTexture" << YAML::Value << value->IsSubTexture;
			//textureOut << YAML::Key << "ResourceType" << YAML::Value << "Image";

			if (value->IsSubTexture)
			{
				textureOut << YAML::Key << "TexCoords" << YAML::Value << YAML::BeginSeq;
				for (int i = 0; i < 4; i++)
				{
					textureOut << YAML::BeginMap;
					textureOut << YAML::Key << "x" << YAML::Value << value->m_TexCoords[i].x;
					textureOut << YAML::Key << "y" << YAML::Value << value->m_TexCoords[i].y;
					textureOut << YAML::EndMap;
				}
				textureOut << YAML::EndSeq;
			}
			textureOut << YAML::EndMap;
		}

		YAML::EndSeq;
		YAML::EndMap;

		std::ofstream file(textureMetadataPath, std::ios::trunc);
		file << textureOut.c_str();
		file.close();
	}

	void ResourceSerializer::CacheTexture(UUID uuid, TextureResourceData* data)
	{
		if (TextureResourceDataCache.find(uuid) != TextureResourceDataCache.end())
		{
			if (TextureResourceDataCache[uuid] != nullptr)
				delete TextureResourceDataCache[uuid];
			TextureResourceDataCache.erase(uuid);
		}

		TextureResourceDataCache[uuid] = data;
		ResourceTypeInfo[uuid] = ResourceType::Image;
	}

	/*TextureResourceData* ResourceSerializer::ReadCachedTexture(std::filesystem::path& keyPath)
	{
		if (TextureResourceDataCache.find(keyPath) == TextureResourceDataCache.end())
			return nullptr;

		return TextureResourceDataCache[keyPath];
	}*/

	void ResourceSerializer::DeleteCachedResource(UUID uuid, ResourceType resourceType, bool deleteFile)
	{
		if (resourceType == ResourceType::Image)
		{
			if (TextureResourceDataCache.find(uuid) != TextureResourceDataCache.end())
			{
				TextureResourceData* data = TextureResourceDataCache[uuid];
				TextureResourceDataCache.erase(uuid);
				ResourceTypeInfo.erase(uuid);
				if (deleteFile)
				{
					std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / ((TextureResourceData*)data)->ResourcePath / std::string(((TextureResourceData*)data)->ImageName + ((TextureResourceData*)data)->Extension);
					std::remove(finalPath.string().c_str());
				}
			}
		}
		else
		{
			EG_CORE_ERROR("Resource type not supported for deletion");
		}
	}
}