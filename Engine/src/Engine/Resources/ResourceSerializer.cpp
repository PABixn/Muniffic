#include "egpch.h"
#include "Engine/Core/Core.h"
#include "ResourceSerializer.h"
#include "Engine/Utils/YAMLConversion.h"
#include "AssetDirectorySerializer.h"
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
	std::unordered_map<UUID, ScriptResourceData*> ResourceSerializer::ScriptResourceDataCache;
	std::unordered_map<UUID, AudioResourceData*> ResourceSerializer::AudioResourceDataCache;
	std::unordered_map<UUID, ResourceType> ResourceSerializer::ResourceTypeInfo;

	bool ResourceSerializer::DeserializeResourceCache()
	{
		AssetDirectorySerializer::DeserializeAssetDirectoryCache();

		std::filesystem::path textureMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Image);
		std::filesystem::path animationMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Animation);
		std::filesystem::path spriteAtlasMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::SpriteAtlas);
		std::filesystem::path subTextureMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::SubTexture);
		std::filesystem::path fontMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Font);
		std::filesystem::path scriptMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Script);
		std::filesystem::path audioMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Audio);

		YAML::Node textureNode, animationNode, spriteAtlasNode, subTextureNode, fontNode, scriptNode, audioNode;

		if (!std::filesystem::exists(textureMetadataPath))
		{
			std::filesystem::create_directories(textureMetadataPath.parent_path());
			std::ofstream file(textureMetadataPath, std::ios::trunc);
			file.close();
		}

		if (!std::filesystem::exists(animationMetadataPath))
		{
			std::filesystem::create_directories(animationMetadataPath.parent_path());
			std::ofstream file(animationMetadataPath, std::ios::trunc);
			file.close();
		}

		if (!std::filesystem::exists(spriteAtlasMetadataPath))
		{
			std::filesystem::create_directories(spriteAtlasMetadataPath.parent_path());
			std::ofstream file(spriteAtlasMetadataPath, std::ios::trunc);
			file.close();
		}

		if (!std::filesystem::exists(subTextureMetadataPath))
		{
			std::filesystem::create_directories(subTextureMetadataPath.parent_path());
			std::ofstream file(subTextureMetadataPath, std::ios::trunc);
			file.close();
		}

		if (!std::filesystem::exists(fontMetadataPath))
		{
			std::filesystem::create_directories(fontMetadataPath.parent_path());
			std::ofstream file(fontMetadataPath, std::ios::trunc);
			file.close();
		}

		if (!std::filesystem::exists(scriptMetadataPath))
		{
			std::filesystem::create_directories(scriptMetadataPath.parent_path());
			std::ofstream file(scriptMetadataPath, std::ios::trunc);
			file.close();
		}

		if (!std::filesystem::exists(audioMetadataPath))
		{
			std::filesystem::create_directories(audioMetadataPath.parent_path());
			std::ofstream file(audioMetadataPath, std::ios::trunc);
			file.close();
		}

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

		try
		{
			scriptNode = YAML::LoadFile(scriptMetadataPath.string());
		}
		catch (YAML::ParserException e)
		{
			EG_CORE_ERROR("Failed to load .mnmeta file '{0}'\n     {1}", scriptMetadataPath, e.what());
			audioNode = YAML::LoadFile(audioMetadataPath.string());
		}

		try
		{
			audioNode = YAML::LoadFile(audioMetadataPath.string());
		}
		catch (YAML::ParserException e)
		{
			EG_CORE_ERROR("Failed to load .mnmeta file '{0}'\n     {1}", audioMetadataPath, e.what());
			return false;
		}

		auto textureResources = textureNode["Resources"];
		auto animationResources = animationNode["Resources"];
		auto spriteAtlasResources = spriteAtlasNode["Resources"];
		auto subtextureResource = subTextureNode["Resources"];
		auto fontResources = fontNode["Resources"];
		auto scriptResources = scriptNode["Resources"];
		auto audioResources = audioNode["Resources"];

		if (fontResources)
		{
			for (auto resource : fontResources)
			{
				UUID uuid = resource["UUID"].as<uint64_t>();

				FontResourceData* data = new FontResourceData();
				if (resource["ParentDirectory"])
					data->ParentDirectory = resource["ParentDirectory"].as<UUID>();
				else
					data->ParentDirectory = 0;
				if (resource["ResourceName"])
					data->ResourceName = resource["ResourceName"].as<std::string>();
				else
					data->ResourceName = resource["FontName"].as<std::string>();
				data->Extension = resource["Extension"].as<std::string>();

				CacheFont(uuid, data);
			}
		}

		if (ResourceDatabase::GetFontResourceDataCache().size() > 0)
			Font::SetDefaultFont(ResourceDatabase::GetFontResourceDataCache().begin()->first);
		else
			Font::LoadFont("assets/fonts/opensans/OpenSans-Regular.ttf");

		if (textureResources)
		{
			for (auto resource : textureResources)
			{
				UUID uuid = resource["UUID"].as<uint64_t>();

				TextureResourceData* data = new TextureResourceData();
				if(resource["ParentDirectory"])
					data->ParentDirectory = resource["ParentDirectory"].as<UUID>();
				else 
					data->ParentDirectory = 0;
				if(resource["ResourceName"])
					data->ResourceName = resource["ResourceName"].as<std::string>();
				else
					data->ResourceName = resource["ImageName"].as<std::string>();
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
				if (resource["ParentDirectory"])
					data->ParentDirectory = resource["ParentDirectory"].as<UUID>();
				else
					data->ParentDirectory = 0;
				if (resource["ResourceName"])
					data->ResourceName = resource["ResourceName"].as<std::string>();
				else
					data->ResourceName = resource["SubtextureName"].as<std::string>();
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
				if (resource["ParentDirectory"])
					data->ParentDirectory = resource["ParentDirectory"].as<UUID>();
				else
					data->ParentDirectory = 0;
				if (resource["ResourceName"])
					data->ResourceName = resource["ResourceName"].as<std::string>();
				else
					data->ResourceName = resource["AnimationName"].as<std::string>();

				data->Extension = resource["Extension"].as<std::string>();
				data->FrameRate = resource["FrameRate"].as<float>();
				data->FrameCount = resource["FrameCount"].as<int>();
				data->Loop = resource["Loop"].as<bool>();

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
					if (resource["ParentDirectory"])
						data->ParentDirectory = resource["ParentDirectory"].as<UUID>();
					else
						data->ParentDirectory = 0;
					if (resource["ResourceName"])
						data->ResourceName = resource["ResourceName"].as<std::string>();
					else
						data->ResourceName = resource["SpriteAtlas"].as<std::string>();

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

			if (scriptResources)
			{
				for (auto resource : scriptResources)
				{
					UUID uuid = resource["UUID"].as<uint64_t>();

					ScriptResourceData* data = new ScriptResourceData();

					if(resource["ParentDirectory"])
						data->ParentDirectory = resource["ParentDirectory"].as<UUID>();
					else
						data->ParentDirectory = 0;
					data->ResourceName = resource["ResourceName"].as<std::string>();
					if(resource["IsEnabled"])
						data->IsEnabled = resource["IsEnabled"].as<bool>();
					else
						data->IsEnabled = true;
					data->Extension = resource["Extension"].as<std::string>();

					CacheScript(uuid, data);
				}
			}
			if (audioResources)
			{
				for (auto resource : audioResources)
				{
					UUID uuid = resource["UUID"].as<uint64_t>();

					AudioResourceData* data = new AudioResourceData();
					if (resource["ParentDirectory"])
						data->ParentDirectory = resource["ParentDirectory"].as<UUID>();
					else
						data->ParentDirectory = 0;
					if (resource["ResourceName"])
						data->ResourceName = resource["ResourceName"].as<std::string>();
					else
						data->ResourceName = resource["AudioName"].as<std::string>();

					data->Extension = resource["Extension"].as<std::string>();

					CacheAudio(uuid, data);
				}
			}

		}
	}

	void ResourceSerializer::SerializeResourceCache()
	{
		AssetDirectorySerializer::SerializeAssetDirectoryCache();

		std::filesystem::path textureMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Image);
		std::filesystem::path animationMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Animation);
		std::filesystem::path spriteAtlasMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::SpriteAtlas);
		std::filesystem::path subTextureMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::SubTexture);
		std::filesystem::path fontMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Font);
		std::filesystem::path scriptMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Script);
		std::filesystem::path audioMetadataPath = ResourceUtils::GetMetadataPath(ResourceType::Audio);


		YAML::Emitter textureOut, animationOut, spriteAtlasOut, subtextureOut, fontOut, scriptOut, audioOut;

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

		if (!std::filesystem::exists(scriptMetadataPath.parent_path()))
			std::filesystem::create_directories(scriptMetadataPath.parent_path());
		if (!std::filesystem::exists(audioMetadataPath.parent_path()))
			std::filesystem::create_directories(audioMetadataPath.parent_path());

		textureOut << YAML::BeginMap;
		textureOut << YAML::Key << "Resources" << YAML::Value << YAML::BeginSeq;

		for (auto& [key, value] : TextureResourceDataCache)
		{
			textureOut << YAML::BeginMap;
			textureOut << YAML::Key << "UUID" << YAML::Value << key;
			textureOut << YAML::Key << "ParentDirectory" << YAML::Value << value->ParentDirectory;
			textureOut << YAML::Key << "ResourceName" << YAML::Value << value->ResourceName;
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
			subtextureOut << YAML::Key << "ParentDirectory" << YAML::Value << value->ParentDirectory;
			subtextureOut << YAML::Key << "ResourceName" << YAML::Value << value->ResourceName;
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
			animationOut << YAML::Key << "ParentDirectory" << YAML::Value << value->ParentDirectory;
			animationOut << YAML::Key << "ResourceName" << YAML::Value << value->ResourceName;
			animationOut << YAML::Key << "Extension" << YAML::Value << value->Extension;
			animationOut << YAML::Key << "FrameRate" << YAML::Value << value->FrameRate;
			animationOut << YAML::Key << "FrameCount" << YAML::Value << value->FrameCount;
			animationOut << YAML::Key << "Loop" << YAML::Value << value->Loop;
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
			spriteAtlasOut << YAML::Key << "ParentDirectory" << YAML::Value << value->ParentDirectory;
			spriteAtlasOut << YAML::Key << "ResourceName" << YAML::Value << value->ResourceName;
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
			fontOut << YAML::Key << "ParentDirectory" << YAML::Value << value->ParentDirectory;
			fontOut << YAML::Key << "ResourceName" << YAML::Value << value->ResourceName;
			fontOut << YAML::Key << "Extension" << YAML::Value << value->Extension;
			fontOut << YAML::EndMap;
		}

		fontOut << YAML::EndSeq;
		fontOut << YAML::EndMap;

		scriptOut << YAML::BeginMap;
		scriptOut << YAML::Key << "Resources" << YAML::Value << YAML::BeginSeq;

		for (auto& [key, value] : ScriptResourceDataCache)
		{
			scriptOut << YAML::BeginMap;
			scriptOut << YAML::Key << "UUID" << YAML::Value << key;
			scriptOut << YAML::Key << "ParentDirectory" << YAML::Value << value->ParentDirectory;
			scriptOut << YAML::Key << "ResourceName" << YAML::Value << value->ResourceName;
			scriptOut << YAML::Key << "Extension" << YAML::Value << value->Extension;
			scriptOut << YAML::Key << "IsEnabled" << YAML::Value << value->IsEnabled;
			scriptOut << YAML::EndMap;
		}

		scriptOut << YAML::EndSeq;
		scriptOut << YAML::EndMap;
		
		audioOut << YAML::BeginMap;
		audioOut << YAML::Key << "Resources" << YAML::Value << YAML::BeginSeq;

		for (auto& [key, value] : AudioResourceDataCache)
		{
			audioOut << YAML::BeginMap;
			audioOut << YAML::Key << "UUID" << YAML::Value << key;
			audioOut << YAML::Key << "ParentDirectory" << YAML::Value << value->ParentDirectory;
			audioOut << YAML::Key << "ResourceName" << YAML::Value << value->ResourceName;
			audioOut << YAML::Key << "Extension" << YAML::Value << value->Extension;
			audioOut << YAML::EndMap;
		}

		audioOut << YAML::EndSeq;
		audioOut << YAML::EndMap;

		std::ofstream textureFile(textureMetadataPath, std::ios::trunc);
		std::ofstream animationFile(animationMetadataPath, std::ios::trunc);
		std::ofstream spriteAtlasFile(spriteAtlasMetadataPath, std::ios::trunc);
		std::ofstream subTextureFile(subTextureMetadataPath, std::ios::trunc);
		std::ofstream fontFile(fontMetadataPath, std::ios::trunc);
		std::ofstream scriptFile(scriptMetadataPath, std::ios::trunc);
		std::ofstream audioFile(audioMetadataPath, std::ios::trunc);

		textureFile << textureOut.c_str();
		animationFile << animationOut.c_str();
		spriteAtlasFile << spriteAtlasOut.c_str();
		subTextureFile << subtextureOut.c_str();
		fontFile << fontOut.c_str();
		scriptFile << scriptOut.c_str();
		audioFile << audioOut.c_str();

		textureFile.close();
		animationFile.close();
		spriteAtlasFile.close();
		subTextureFile.close();
		fontFile.close();
		scriptFile.close();
		audioFile.close();
	}

	void ResourceSerializer::CacheTexture(UUID uuid, TextureResourceData* data)
	{
		//std::filesystem::path finalPath = ResourceDatabase::GetResourcePath(uuid);

		//if (!std::filesystem::exists(finalPath))
		//{
		//	EG_CORE_ERROR("File '{0}' has not been found on disk.", finalPath.string());
		//	return;
		//}

		if (TextureResourceDataCache.find(uuid) != TextureResourceDataCache.end())
		{
			if (TextureResourceDataCache[uuid] != nullptr)
				delete TextureResourceDataCache[uuid];
			TextureResourceDataCache.erase(uuid);
		}

		/*UUID foundUUID = ResourceDatabase::FindResourceByKeyPath(finalKeyPath, ResourceType::Image);

		if (foundUUID != 0)
		{
			if (TextureResourceDataCache.at(foundUUID) != nullptr)
				delete TextureResourceDataCache.at(foundUUID);
			TextureResourceDataCache.erase(foundUUID);
		}*/

		data->Type = ResourceType::Image;
		TextureResourceDataCache[uuid] = data;
		ResourceTypeInfo[uuid] = ResourceType::Image;
		AssetDirectoryManager::addAsset(data->ParentDirectory, uuid);
	}

	void ResourceSerializer::CacheSubTexture(UUID uuid, SubTextureResourceData* data)
	{
		if (SubTextureResourceDataCache.find(uuid) != SubTextureResourceDataCache.end())
		{
			if (SubTextureResourceDataCache[uuid] != nullptr)
				delete SubTextureResourceDataCache[uuid];
			SubTextureResourceDataCache.erase(uuid);
		}

		data->Type = ResourceType::SubTexture;
		SubTextureResourceDataCache[uuid] = data;
		ResourceTypeInfo[uuid] = ResourceType::SubTexture;
		AssetDirectoryManager::addAsset(data->ParentDirectory, uuid);
	}

	void ResourceSerializer::CacheFont(UUID uuid, FontResourceData* data)
	{
		/*std::filesystem::path finalPath = ResourceDatabase::GetResourcePath(uuid);

		if (!std::filesystem::exists(finalPath))
		{
			EG_CORE_ERROR("File '{0}' has not been found on disk.", finalPath.string());
			return;
		}*/

		if (FontResourceDataCache.find(uuid) != FontResourceDataCache.end())
		{
			if (FontResourceDataCache.at(uuid) != nullptr)
				delete FontResourceDataCache.at(uuid);
			FontResourceDataCache.erase(uuid);
		}

		/*UUID foundUUID = ResourceDatabase::FindResourceByKeyPath(finalKeyPath, ResourceType::Font);

		if (foundUUID != 0)
		{
			if (FontResourceDataCache.at(foundUUID) != nullptr)
				delete FontResourceDataCache.at(foundUUID);
			FontResourceDataCache.erase(foundUUID);
		}*/

		data->Type = ResourceType::Font;
		FontResourceDataCache[uuid] = data;
		ResourceTypeInfo[uuid] = ResourceType::Font;
		AssetDirectoryManager::addAsset(data->ParentDirectory, uuid);
	}

	void ResourceSerializer::CacheScript(UUID uuid, ScriptResourceData* data)
	{
		if (ScriptResourceDataCache.find(uuid) != ScriptResourceDataCache.end())
		{
			if (ScriptResourceDataCache[uuid] != nullptr)
				delete ScriptResourceDataCache[uuid];
			ScriptResourceDataCache.erase(uuid);
		}

		data->Type = ResourceType::Script;
		ScriptResourceDataCache[uuid] = data;
		ResourceTypeInfo[uuid] = ResourceType::Script;
		AssetDirectoryManager::addAsset(data->ParentDirectory, uuid);
	}

	void ResourceSerializer::CacheAudio(UUID uuid, AudioResourceData* data)
	{
		if (AudioResourceDataCache.find(uuid) != AudioResourceDataCache.end())
		{
			if (AudioResourceDataCache[uuid] != nullptr)
				delete AudioResourceDataCache[uuid];
			AudioResourceDataCache.erase(uuid);
		}

		data->Type = ResourceType::Audio;
		AudioResourceDataCache[uuid] = data;
		ResourceTypeInfo[uuid] = ResourceType::Audio;
		AssetDirectoryManager::addAsset(data->ParentDirectory, uuid);
	}

	void ResourceSerializer::CacheAnimation(UUID uuid, AnimationResourceData* data)
	{
		if (AnimationResourceDataCache.find(uuid) != AnimationResourceDataCache.end())
		{
			if (AnimationResourceDataCache[uuid] != nullptr)
				delete AnimationResourceDataCache[uuid];
			AnimationResourceDataCache.erase(uuid);
		}

		data->Type = ResourceType::Animation;
		AnimationResourceDataCache[uuid] = data;
		ResourceTypeInfo[uuid] = ResourceType::Animation;
		AssetDirectoryManager::addAsset(data->ParentDirectory, uuid);
	}

	void ResourceSerializer::CacheSpriteAtlas(UUID uuid, SpriteAtlasResourceData* data)
	{
		if (SpriteAtlasResourceDataCache.find(uuid) != SpriteAtlasResourceDataCache.end())
		{
			if (SpriteAtlasResourceDataCache[uuid] != nullptr)
				delete SpriteAtlasResourceDataCache[uuid];
			SpriteAtlasResourceDataCache.erase(uuid);
		}

		data->Type = ResourceType::SpriteAtlas;
		SpriteAtlasResourceDataCache[uuid] = data;
		ResourceTypeInfo[uuid] = ResourceType::SpriteAtlas;
		AssetDirectoryManager::addAsset(data->ParentDirectory, uuid);
	}
}