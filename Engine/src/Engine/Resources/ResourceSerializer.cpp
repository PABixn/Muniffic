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
	std::unordered_map<UUID, AnimationResourceData*> ResourceSerializer::AnimationResourceDataCache;
	std::unordered_map<UUID, SpriteAtlasResourceData*> ResourceSerializer::SpriteAtlasResourceDataCache;
	std::unordered_map<UUID, ResourceType> ResourceSerializer::ResourceTypeInfo;

	bool ResourceSerializer::DeserializeResourceCache()
	{
		std::filesystem::path textureMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Image);
		std::filesystem::path animationMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Animation);
		std::filesystem::path spriteAtlasMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::SpriteAtlas);

		YAML::Node textureNode, animationNode, spriteAtlasNode;

		if (!std::filesystem::exists(textureMetadataPath))
			return false;

		if (!std::filesystem::exists(animationMetadataPath))
			return false;

		if (!std::filesystem::exists(spriteAtlasMetadataPath))
			return false;

		try
		{
			textureNode = YAML::LoadFile(textureMetadataPath.string());
		}
		catch(YAML::ParserException e)
		{
			EG_CORE_ERROR("Failed to load .mnmeta file '{0}'\n     {1}", textureMetadataPath, e.what());
			return false;
		}

		try
		{
			animationNode = YAML::LoadFile(animationMetadataPath.string());
		}
		catch (YAML::ParserException e)
		{
			EG_CORE_ERROR("Failed to load .mnmeta file '{0}'\n     {1}", animationMetadataPath, e.what());
			return false;
		}

		try
		{
			spriteAtlasNode = YAML::LoadFile(spriteAtlasMetadataPath.string());
		}
		catch (YAML::ParserException e)
		{
			EG_CORE_ERROR("Failed to load .mnmeta file '{0}'\n     {1}", spriteAtlasMetadataPath, e.what());
			return false;
		}

		auto textureResources = textureNode["Resources"];
		auto animationResources = animationNode["Resources"];
		auto spriteAtlasResources = spriteAtlasNode["Resources"];

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

		if (animationResources)
		{
			for (auto resource : animationResources)
			{
				UUID uuid = resource["UUID"].as<uint64_t>();

				AnimationResourceData* data = new AnimationResourceData();
				data->ResourcePath = resource["ResourcePath"].as<std::string>();
				data->AnimationName = resource["AnimationName"].as<std::string>();
				data->Extension = resource["Extension"].as<std::string>();
				data->m_frameRate = resource["FrameRate"].as<float>();
				data->m_frameCount = resource["FrameCount"].as<int>();
				data->m_loop = resource["Loop"].as<bool>();
				data->name = resource["Name"].as<std::string>();

				auto frames = resource["Frames"];
				for (auto frame : frames)
				{
					data->m_frames.push_back(frame.as<uint64_t>());
				}

				CacheAnimation(uuid, data);
			}

			if (spriteAtlasResources)
			{
				for (auto resource : spriteAtlasResources)
				{
					UUID uuid = resource["UUID"].as<uint64_t>();

					SpriteAtlasResourceData* data = new SpriteAtlasResourceData();
					data->ResourcePath = resource["ResourcePath"].as<std::string>();
					data->AtlasName = resource["AtlasName"].as<std::string>();
					data->Extension = resource["Extension"].as<std::string>();
					data->Width = resource["Width"].as<int>();
					data->Height = resource["Height"].as<int>();
					data->Channels = resource["Channels"].as<int>();

					auto sprites = resource["Sprites"];
					for (auto sprite : sprites)
					{
						data->Sprites.push_back(sprite.as<uint64_t>());
					}

					CacheSpriteAtlas(uuid, data);
				}
			}
		}
	}

	void ResourceSerializer::SerializeResourceCache()
	{
		std::filesystem::path textureMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Image);
		std::filesystem::path animationMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Animation);
		std::filesystem::path spriteAtlasMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::SpriteAtlas);
		YAML::Emitter textureOut, animationOut, spriteAtlasOut;

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

		textureOut << YAML::EndSeq;
		textureOut << YAML::EndMap;

		animationOut << YAML::BeginMap;
		animationOut << YAML::Key << "Resources" << YAML::Value << YAML::BeginSeq;

		for (auto& [key, value] : AnimationResourceDataCache)
		{
			animationOut << YAML::BeginMap;
			animationOut << YAML::Key << "UUID" << YAML::Value << key;
			animationOut << YAML::Key << "ResourcePath" << YAML::Value << value->ResourcePath.string();
			animationOut << YAML::Key << "AnimationName" << YAML::Value << value->AnimationName;
			animationOut << YAML::Key << "Extension" << YAML::Value << value->Extension;
			animationOut << YAML::Key << "FrameRate" << YAML::Value << value->m_frameRate;
			animationOut << YAML::Key << "FrameCount" << YAML::Value << value->m_frameCount;
			animationOut << YAML::Key << "Loop" << YAML::Value << value->m_loop;
			animationOut << YAML::Key << "Name" << YAML::Value << value->name;
			animationOut << YAML::Key << "Frames" << YAML::Value << YAML::BeginSeq;
			for (auto& frame : value->m_frames)
			{
				animationOut << frame;
			}
			animationOut << YAML::EndSeq;
			animationOut << YAML::EndMap;
		}

		animationOut << YAML::EndSeq;
		animationOut << YAML::EndMap;

		spriteAtlasOut << YAML::BeginMap;
		spriteAtlasOut << YAML::Key << "Resources" << YAML::Value << YAML::BeginSeq;

		for (auto& [key, value] : SpriteAtlasResourceDataCache)
		{
			spriteAtlasOut << YAML::BeginMap;
			spriteAtlasOut << YAML::Key << "UUID" << YAML::Value << key;
			spriteAtlasOut << YAML::Key << "ResourcePath" << YAML::Value << value->ResourcePath.string();
			spriteAtlasOut << YAML::Key << "AtlasName" << YAML::Value << value->AtlasName;
			spriteAtlasOut << YAML::Key << "Extension" << YAML::Value << value->Extension;
			spriteAtlasOut << YAML::Key << "Width" << YAML::Value << value->Width;
			spriteAtlasOut << YAML::Key << "Height" << YAML::Value << value->Height;
			spriteAtlasOut << YAML::Key << "Channels" << YAML::Value << value->Channels;
			spriteAtlasOut << YAML::Key << "Sprites" << YAML::Value << YAML::BeginSeq;
			for (auto& sprite : value->Sprites)
			{
				spriteAtlasOut << sprite;
			}
			spriteAtlasOut << YAML::EndSeq;
			spriteAtlasOut << YAML::EndMap;
		}

		spriteAtlasOut << YAML::EndSeq;
		spriteAtlasOut << YAML::EndMap;

		std::ofstream textureFile(textureMetadataPath, std::ios::trunc);
		std::ofstream animationFile(animationMetadataPath, std::ios::trunc);
		std::ofstream spriteAtlasFile(spriteAtlasMetadataPath, std::ios::trunc);

		textureFile << textureOut.c_str();
		animationFile << animationOut.c_str();
		spriteAtlasFile << spriteAtlasOut.c_str();

		textureFile.close();
		animationFile.close();
		spriteAtlasFile.close();
	}

	void ResourceSerializer::CacheTexture(UUID uuid, TextureResourceData* data)
	{
		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / data->ResourcePath / std::string(data->ImageName + data->Extension);
		if (!std::filesystem::exists(finalPath))
		{
			EG_CORE_ERROR("File '{0}' has not been found on disk.", finalPath.string());
			return;
		}

		if (TextureResourceDataCache.find(uuid) != TextureResourceDataCache.end())
		{
			if (TextureResourceDataCache[uuid] != nullptr)
				delete TextureResourceDataCache[uuid];
			TextureResourceDataCache.erase(uuid);
		}

		TextureResourceDataCache[uuid] = data;
		ResourceTypeInfo[uuid] = ResourceType::Image;
	}

	void ResourceSerializer::CacheAnimation(UUID uuid, AnimationResourceData* data)
	{
		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / data->ResourcePath / std::string(data->AnimationName + data->Extension);

		if (!std::filesystem::exists(finalPath))
		{
			EG_CORE_ERROR("File '{0}' has not been found on disk.", finalPath.string());
			return;
		}

		if (AnimationResourceDataCache.find(uuid) != AnimationResourceDataCache.end())
		{
			if (AnimationResourceDataCache[uuid] != nullptr)
				delete AnimationResourceDataCache[uuid];
			AnimationResourceDataCache.erase(uuid);
		}

		AnimationResourceDataCache[uuid] = data;
		ResourceTypeInfo[uuid] = ResourceType::Animation;
	}

	void ResourceSerializer::CacheSpriteAtlas(UUID uuid, SpriteAtlasResourceData* data)
	{
		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / data->ResourcePath / std::string(data->AtlasName + data->Extension);

		if (!std::filesystem::exists(finalPath))
		{
			EG_CORE_ERROR("File '{0}' has not been found on disk.", finalPath.string());
			return;
		}

		if (SpriteAtlasResourceDataCache.find(uuid) != SpriteAtlasResourceDataCache.end())
		{
			if (SpriteAtlasResourceDataCache[uuid] != nullptr)
				delete SpriteAtlasResourceDataCache[uuid];
			SpriteAtlasResourceDataCache.erase(uuid);
		}

		SpriteAtlasResourceDataCache[uuid] = data;
		ResourceTypeInfo[uuid] = ResourceType::SpriteAtlas;
	}
}