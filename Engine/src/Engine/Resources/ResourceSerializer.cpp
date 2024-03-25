#include "egpch.h"
#include "Engine/Core/Core.h"
#include "ResourceSerializer.h"
#include "Engine/Utils/YAMLConversion.h"
#include "ResourceUtils.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <optional>
#include <iostream>


namespace eg
{
	std::unordered_map<UUID, TextureResourceData*> ResourceSerializer::TextureResourceDataCache;
	std::unordered_map<UUID, AnimationResourceData*> ResourceSerializer::AnimationResourceDataCache;
	std::unordered_map<UUID, SpriteAtlasResourceData*> ResourceSerializer::SpriteAtlasResourceDataCache;
	std::unordered_map<UUID, SubTextureResourceData*> ResourceSerializer::SubTextureResourceDataCache;
	std::unordered_map<UUID, FontResourceData*> ResourceSerializer::FontResourceDataCache;
	std::unordered_map<UUID, ResourceType> ResourceSerializer::ResourceTypeInfo;

	bool ResourceSerializer::DeserializeResourceCache()
	{
		std::filesystem::path textureMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Image);
		std::filesystem::path animationMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Animation);
		std::filesystem::path spriteAtlasMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::SpriteAtlas);
		std::filesystem::path subTextureMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::SubTexture);
		std::filesystem::path fontMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Font);

		YAML::Node textureNode, animationNode, spriteAtlasNode, subTextureNode, fontNode;

		if (!std::filesystem::exists(textureMetadataPath))
			return false;

		if (!std::filesystem::exists(animationMetadataPath))
			return false;

		if (!std::filesystem::exists(spriteAtlasMetadataPath))
			return false;

		if (!std::filesystem::exists(subTextureMetadataPath))
			return false;

		if (!std::filesystem::exists(fontMetadataPath))
			return false;

		try
		{
			textureNode = YAML::LoadFile(textureMetadataPath.string());
		}
		catch (YAML::ParserException e)
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

		try
		{
			subTextureNode = YAML::LoadFile(subTextureMetadataPath.string());
		}
		catch (YAML::ParserException e)
		{
			EG_CORE_ERROR("Failed to load .mnmeta file '{0}'\n     {1}", subTextureMetadataPath, e.what());
			return false;
		}

		try
		{
			fontNode = YAML::LoadFile(fontMetadataPath.string());
		}
		catch (YAML::ParserException e)
		{
			EG_CORE_ERROR("Failed to load .mnmeta file '{0}'\n     {1}", fontMetadataPath, e.what());
			return false;
		}

		auto textureResources = textureNode["Resources"];
		auto animationResources = animationNode["Resources"];
		auto spriteAtlasResources = spriteAtlasNode["Resources"];
		auto subtextureResource = subTextureNode["Resources"];
		auto fontResources = fontNode["Resources"];

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

				CacheTexture(uuid, data);
			}
		}

		if (subtextureResource)
		{
			for (auto resource : subtextureResource)
			{
				UUID uuid = resource["UUID"].as<uint64_t>();

				SubTextureResourceData* data = new SubTextureResourceData();
				data->ResourcePath = resource["ResourcePath"].as<std::string>();
				data->SubTextureName = resource["SubTextureName"].as<std::string>();
				data->Extension = resource["Extension"].as<std::string>();
				data->Texture = resource["Texture"].as<uint64_t>();

				auto texCoords = resource["TexCoords"];
				int i = 0;
				for (auto texCoord : texCoords)
				{
					data->TexCoords[i] = texCoord.as<glm::vec2>();
					i++;
				}

				CacheSubTexture(uuid, data);
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
				data->FrameRate = resource["FrameRate"].as<float>();
				data->FrameCount = resource["FrameCount"].as<int>();
				data->Loop = resource["Loop"].as<bool>();
				data->AnimationName = resource["Name"].as<std::string>();

				auto frames = resource["Frames"];
				for (auto frame : frames)
				{
					data->Frames.push_back(frame.as<uint64_t>());
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

			if (fontResources)
			{
				for (auto resource : fontResources)
				{
					UUID uuid = resource["UUID"].as<uint64_t>();

					FontResourceData* data = new FontResourceData();
					data->ResourcePath = resource["ResourcePath"].as<std::string>();
					data->FontName = resource["FontName"].as<std::string>();
					data->Extension = resource["Extension"].as<std::string>();

					CacheFont(uuid, data);
				}
			}

			if (ResourceDatabase::GetFontResourceDataCache().size() > 0)
				Font::SetDefaultFont(ResourceDatabase::GetFontResourceDataCache().begin()->first);
			else
				Font::LoadFont("assets/fonts/opensans/OpenSans-Regular.ttf");
		}
	}

	void ResourceSerializer::SerializeResourceCache()
	{
		std::filesystem::path textureMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Image);
		std::filesystem::path animationMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Animation);
		std::filesystem::path spriteAtlasMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::SpriteAtlas);
		std::filesystem::path subTextureMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::SubTexture);
		std::filesystem::path fontMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Font);


		YAML::Emitter textureOut, animationOut, spriteAtlasOut, subtextureOut, fontOut;

		if(!std::filesystem::exists(textureMetadataPath.parent_path()))
			std::filesystem::create_directories(textureMetadataPath.parent_path());

		if (!std::filesystem::exists(animationMetadataPath.parent_path()))
			std::filesystem::create_directories(animationMetadataPath.parent_path());

		if (!std::filesystem::exists(spriteAtlasMetadataPath.parent_path()))
			std::filesystem::create_directories(spriteAtlasMetadataPath.parent_path());

		if (!std::filesystem::exists(subTextureMetadataPath.parent_path()))
			std::filesystem::create_directories(subTextureMetadataPath.parent_path());

		if (!std::filesystem::exists(fontMetadataPath.parent_path()))
			std::filesystem::create_directories(fontMetadataPath.parent_path());

		textureOut << YAML::BeginMap;
		textureOut << YAML::Key << "Resources" << YAML::Value << YAML::BeginSeq;

		for (auto& [key, value] : TextureResourceDataCache)
		{
			textureOut << YAML::BeginMap;
			textureOut << YAML::Key << "UUID" << YAML::Value << key;
			textureOut << YAML::Key << "ResourcePath" << YAML::Value << value->ResourcePath.string();
			textureOut << YAML::Key << "ImageName" << YAML::Value << value->ImageName;
			textureOut << YAML::Key << "Extension" << YAML::Value << value->Extension;
			textureOut << YAML::Key << "Width" << YAML::Value << value->Width;
			textureOut << YAML::Key << "Height" << YAML::Value << value->Height;
			textureOut << YAML::Key << "Channels" << YAML::Value << value->Channels;

			textureOut << YAML::EndMap;
		}

		textureOut << YAML::EndSeq;
		textureOut << YAML::EndMap;

		subtextureOut << YAML::BeginMap;
		subtextureOut << YAML::Key << "Resources" << YAML::Value << YAML::BeginSeq;

		for (auto& [key, value] : SubTextureResourceDataCache)
		{
			subtextureOut << YAML::BeginMap;
			subtextureOut << YAML::Key << "UUID" << YAML::Value << key;
			subtextureOut << YAML::Key << "ResourcePath" << YAML::Value << value->ResourcePath.string();
			subtextureOut << YAML::Key << "SubTextureName" << YAML::Value << value->SubTextureName;
			subtextureOut << YAML::Key << "Extension" << YAML::Value << value->Extension;
			subtextureOut << YAML::Key << "Texture" << YAML::Value << value->Texture;
			subtextureOut << YAML::Key << "TexCoords" << YAML::Value << YAML::BeginSeq;
			for (int i = 0; i < 4; i++)
			{
				subtextureOut << YAML::BeginMap;
				subtextureOut << value->TexCoords[i];
				subtextureOut << YAML::EndMap;
			}
			subtextureOut << YAML::EndSeq;
			subtextureOut << YAML::EndMap;
		}

		subtextureOut << YAML::EndSeq;
		subtextureOut << YAML::EndMap;

		animationOut << YAML::BeginMap;
		animationOut << YAML::Key << "Resources" << YAML::Value << YAML::BeginSeq;

		for (auto& [key, value] : AnimationResourceDataCache)
		{
			animationOut << YAML::BeginMap;
			animationOut << YAML::Key << "UUID" << YAML::Value << key;
			animationOut << YAML::Key << "ResourcePath" << YAML::Value << value->ResourcePath.string();
			animationOut << YAML::Key << "AnimationName" << YAML::Value << value->AnimationName;
			animationOut << YAML::Key << "Extension" << YAML::Value << value->Extension;
			animationOut << YAML::Key << "FrameRate" << YAML::Value << value->FrameRate;
			animationOut << YAML::Key << "FrameCount" << YAML::Value << value->FrameCount;
			animationOut << YAML::Key << "Loop" << YAML::Value << value->Loop;
			animationOut << YAML::Key << "Name" << YAML::Value << value->AnimationName;
			animationOut << YAML::Key << "Frames" << YAML::Value << YAML::BeginSeq;
			for (auto& frame : value->Frames)
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

		fontOut << YAML::BeginMap;
		fontOut << YAML::Key << "Resources" << YAML::Value << YAML::BeginSeq;

		for (auto& [key, value] : FontResourceDataCache)
		{
			fontOut << YAML::BeginMap;
			fontOut << YAML::Key << "UUID" << YAML::Value << key;
			fontOut << YAML::Key << "ResourcePath" << YAML::Value << value->ResourcePath.string();
			fontOut << YAML::Key << "FontName" << YAML::Value << value->FontName;
			fontOut << YAML::Key << "Extension" << YAML::Value << value->Extension;
			fontOut << YAML::EndMap;
		}

		fontOut << YAML::EndSeq;
		fontOut << YAML::EndMap;

		std::ofstream textureFile(textureMetadataPath, std::ios::trunc);
		std::ofstream animationFile(animationMetadataPath, std::ios::trunc);
		std::ofstream spriteAtlasFile(spriteAtlasMetadataPath, std::ios::trunc);
		std::ofstream subTextureFile(subTextureMetadataPath, std::ios::trunc);
		std::ofstream fontFile(fontMetadataPath, std::ios::trunc);

		textureFile << textureOut.c_str();
		animationFile << animationOut.c_str();
		spriteAtlasFile << spriteAtlasOut.c_str();
		subTextureFile << subtextureOut.c_str();
		fontFile << fontOut.c_str();

		textureFile.close();
		animationFile.close();
		spriteAtlasFile.close();
		subTextureFile.close();
		fontFile.close();
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

	void ResourceSerializer::CacheSubTexture(UUID uuid, SubTextureResourceData* data)
	{
		if (SubTextureResourceDataCache.find(uuid) != SubTextureResourceDataCache.end())
		{
			if (SubTextureResourceDataCache[uuid] != nullptr)
				delete SubTextureResourceDataCache[uuid];
			SubTextureResourceDataCache.erase(uuid);
		}

		SubTextureResourceDataCache[uuid] = data;
		ResourceTypeInfo[uuid] = ResourceType::SubTexture;
	}

	void ResourceSerializer::CacheFont(UUID uuid, FontResourceData* data)
	{
		std::filesystem::path finalKeyPath = data->ResourcePath / std::string(data->FontName + data->Extension);
		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / finalKeyPath;

		if (!std::filesystem::exists(finalPath))
		{
			EG_CORE_ERROR("File '{0}' has not been found on disk.", finalPath.string());
			return;
		}

		if (FontResourceDataCache.find(uuid) != FontResourceDataCache.end())
		{
			if (FontResourceDataCache.at(uuid) != nullptr)
				delete FontResourceDataCache.at(uuid);
			FontResourceDataCache.erase(uuid);
		}

		UUID foundUUID = ResourceDatabase::FindResourceByKeyPath(finalKeyPath, ResourceType::Font);

		if (foundUUID != 0)
		{
			if (FontResourceDataCache.at(foundUUID) != nullptr)
				delete FontResourceDataCache.at(foundUUID);
			FontResourceDataCache.erase(foundUUID);
		}

		FontResourceDataCache[uuid] = data;
		ResourceTypeInfo[uuid] = ResourceType::Font;
	}

	void ResourceSerializer::CacheAnimation(UUID uuid, AnimationResourceData* data)
	{
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