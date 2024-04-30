#include "ResourceDatabase.h"
#include <filesystem>
#include <string>
#include <vector>
#include "egpch.h"
#include "Engine/Project/Project.h"	
#include <yaml-cpp/yaml.h>
#include "ResourceUtils.h"
#include "Systems/ResourceSystem.h"
#include "AssetDirectoryManager.h"

namespace eg
{
	UUID ResourceDatabase::m_CurrentDirectory;

	std::unordered_map<UUID, Ref<Font>> ResourceDatabase::RuntimeFontResourceCache;
	std::unordered_map<UUID, Ref<Texture2D>> ResourceDatabase::RuntimeTextureResourceCache;

	void ResourceDatabase::EnableScript(UUID uuid, bool enable)
	{
		if (ResourceSerializer::ScriptResourceDataCache.find(uuid) != ResourceSerializer::ScriptResourceDataCache.end())
			((ScriptResourceData*)ResourceSerializer::ScriptResourceDataCache.at(uuid))->IsEnabled = enable;
		else
			EG_CORE_ERROR("Script resource not found");
	}

	bool ResourceDatabase::IsScriptEnabled(UUID uuid)
	{
		if (ResourceSerializer::ScriptResourceDataCache.find(uuid) != ResourceSerializer::ScriptResourceDataCache.end())
			return ((ScriptResourceData*)ResourceSerializer::ScriptResourceDataCache.at(uuid))->IsEnabled;
		else
		{
			EG_CORE_ERROR("Script resource not found");
			return false;
		}
	}

	bool ResourceDatabase::FindRuntimeResource(UUID uuid, ResourceType type)
	{
		if (type == ResourceType::Font)
			return RuntimeFontResourceCache.find(uuid) != RuntimeFontResourceCache.end();
		else if(type == ResourceType::Image)
			return RuntimeTextureResourceCache.find(uuid) != RuntimeTextureResourceCache.end();
		else
		{
			EG_CORE_ERROR("Resource type not supported");
			return false;
		}
	}

	void* ResourceDatabase::AddRuntimeResource(UUID uuid, void* data, ResourceType type)
	{
		if (data == nullptr)
			return nullptr;

		if (type == ResourceType::Font)
		{
			Ref<Font> font = CreateRef<Font>((Font*)data);
			RuntimeFontResourceCache[uuid] = font;
			return font.get();
		}
		else if (type == ResourceType::Image)
		{
			Ref<Texture2D> texture = Texture2D::Create((const char*)data);
			RuntimeTextureResourceCache[uuid] = texture;
			return texture.get();
		}
		else
		{
			EG_CORE_WARN("Resource type not suitable for adding to runtime");
			return nullptr;
		}
	}

	Ref<Font> ResourceDatabase::GetFontRuntimeResource(UUID uuid)
	{
		if(RuntimeFontResourceCache.find(uuid) != RuntimeFontResourceCache.end())
			return RuntimeFontResourceCache.at(uuid);
		else
		{
 			void* ptr = LoadRuntimeResource(uuid, ResourceType::Font);
			if(ptr != nullptr)
				return RuntimeFontResourceCache.at(uuid);
			else
			{
				EG_CORE_ERROR("Failed to load font resource");
				return nullptr;
			}
		}
	}

	Ref<Texture2D> ResourceDatabase::GetTextureRuntimeResource(UUID uuid)
	{
		if (RuntimeTextureResourceCache.find(uuid) != RuntimeTextureResourceCache.end())
			return RuntimeTextureResourceCache.at(uuid);
		else
		{
			void* ptr = LoadRuntimeResource(uuid, ResourceType::Image);
			if (ptr != nullptr)
				return RuntimeTextureResourceCache.at(uuid);
			else
			{
				EG_CORE_ERROR("Failed to load texture resource");
				return nullptr;
			}
		}
	}

	void* ResourceDatabase::GetRuntimeResource(UUID uuid, ResourceType type)
	{
		if (type == ResourceType::Font)
		{
			if (FindRuntimeResource(uuid, type))
				return RuntimeFontResourceCache.at(uuid).get();
			else
				return LoadRuntimeResource(uuid, type);
		}
		else if (type == ResourceType::Image)
		{
			if (FindRuntimeResource(uuid, type))
				return RuntimeTextureResourceCache.at(uuid).get();
			else
				return LoadRuntimeResource(uuid, type);
		}
		else
		{
			EG_CORE_ERROR("Resource type not supported");
			return nullptr;
		}
	}

	void ResourceDatabase::SetResourceData(UUID uuid, ResourceType resourceType, void* data)
	{
		if (resourceType == ResourceType::Image)
			ResourceSerializer::TextureResourceDataCache[uuid] = (TextureResourceData*)data;
		else if(resourceType == ResourceType::SubTexture)
			ResourceSerializer::SubTextureResourceDataCache[uuid] = (SubTextureResourceData*)data;
		else if (resourceType == ResourceType::Animation)
			ResourceSerializer::AnimationResourceDataCache[uuid] = (AnimationResourceData*)data;
		else if(resourceType == ResourceType::SpriteAtlas)
			ResourceSerializer::SpriteAtlasResourceDataCache[uuid] = (SpriteAtlasResourceData*)data;
		else if(resourceType == ResourceType::Font)
			ResourceSerializer::FontResourceDataCache[uuid] = (FontResourceData*)data;
		else if(resourceType == ResourceType::Script)
			ResourceSerializer::ScriptResourceDataCache[uuid] = (ScriptResourceData*)data;
		else
			EG_CORE_ERROR("Resource type not supported");
	}

	ResourceType ResourceDatabase::GetResourceType(UUID uuid)
	{
		return ResourceSerializer::ResourceTypeInfo.at(uuid);
	}

	bool ResourceDatabase::FindResourceData(UUID uuid, ResourceType resourceType)
	{
		if (resourceType == ResourceType::Image)
			return ResourceSerializer::TextureResourceDataCache.find(uuid) != ResourceSerializer::TextureResourceDataCache.end();
		else if (resourceType == ResourceType::SubTexture)
			return ResourceSerializer::SubTextureResourceDataCache.find(uuid) != ResourceSerializer::SubTextureResourceDataCache.end();
		else if (resourceType == ResourceType::Animation)
			return ResourceSerializer::AnimationResourceDataCache.find(uuid) != ResourceSerializer::AnimationResourceDataCache.end();
		else if (resourceType == ResourceType::SpriteAtlas)
			return ResourceSerializer::SpriteAtlasResourceDataCache.find(uuid) != ResourceSerializer::SpriteAtlasResourceDataCache.end();
		else if (resourceType == ResourceType::Font)
			return ResourceSerializer::FontResourceDataCache.find(uuid) != ResourceSerializer::FontResourceDataCache.end();
		else if (resourceType == ResourceType::Script)
			return ResourceSerializer::ScriptResourceDataCache.find(uuid) != ResourceSerializer::ScriptResourceDataCache.end();
		else
		{
			EG_CORE_ERROR("Resource type not supported");
			return false;
		}
	}

	UUID ResourceDatabase::GetResourceByName(const std::string& name, ResourceType type)
	{
		if (type == ResourceType::Image)
		{
			for (auto& [uuid, data] : ResourceSerializer::TextureResourceDataCache)
			{
				if (data->ResourceName == name)
					return uuid;
			}
		}
		else if(type == ResourceType::SubTexture)
		{
			for (auto& [uuid, data] : ResourceSerializer::SubTextureResourceDataCache)
			{
				if (data->ResourceName == name)
					return uuid;
			}
		}
		else if (type == ResourceType::Animation)
		{
			for (auto& [uuid, data] : ResourceSerializer::AnimationResourceDataCache)
			{
				if (data->ResourceName == name)
					return uuid;
			}
		}
		else if (type == ResourceType::SpriteAtlas)
		{
			for (auto& [uuid, data] : ResourceSerializer::SpriteAtlasResourceDataCache)
			{
				if (data->ResourceName == name)
					return uuid;
			}
		}
		else if (type == ResourceType::Font)
		{
			for (auto& [uuid, data] : ResourceSerializer::FontResourceDataCache)
			{
				if (data->ResourceName == name)
					return uuid;
			}
		}
		else if (type == ResourceType::Script)
		{
			for (auto& [uuid, data] : ResourceSerializer::ScriptResourceDataCache)
			{
				if (data->ResourceName == name)
					return uuid;
			}
		}
		else
		{
			EG_CORE_ERROR("Resource type not supported");
		}

		return 0;
	}

	void ResourceDatabase::RemoveResource(UUID uuid, bool deleteFile)
	{
		ResourceDatabase::RemoveResource(uuid, ResourceSerializer::ResourceTypeInfo.at(uuid), deleteFile);
	}

	void ResourceDatabase::RemoveResource(UUID uuid, ResourceType resourceType, bool deleteFile)
	{
		if (resourceType == ResourceType::Image)
		{
			if (ResourceSerializer::TextureResourceDataCache.find(uuid) != ResourceSerializer::TextureResourceDataCache.end())
			{
				TextureResourceData* data = ResourceSerializer::TextureResourceDataCache[uuid];

				if (deleteFile)
				{
					std::filesystem::path finalPath = GetResourcePath(uuid);
					std::remove(finalPath.string().c_str());
				}

				ResourceSerializer::TextureResourceDataCache.erase(uuid);
				ResourceSerializer::ResourceTypeInfo.erase(uuid);

				delete data;
			}
		}
		else if(resourceType == ResourceType::SubTexture)
		{
			if (ResourceSerializer::SubTextureResourceDataCache.find(uuid) != ResourceSerializer::SubTextureResourceDataCache.end())
			{
				delete ResourceSerializer::SubTextureResourceDataCache.at(uuid);
				ResourceSerializer::SubTextureResourceDataCache.erase(uuid);
				ResourceSerializer::ResourceTypeInfo.erase(uuid);
			}
		}
		else if(resourceType == ResourceType::Animation)
		{
			if (ResourceSerializer::AnimationResourceDataCache.find(uuid) != ResourceSerializer::AnimationResourceDataCache.end())
			{
				delete ResourceSerializer::AnimationResourceDataCache.at(uuid);
				ResourceSerializer::AnimationResourceDataCache.erase(uuid);
				ResourceSerializer::ResourceTypeInfo.erase(uuid);
			}
		}
		else if (resourceType == ResourceType::SpriteAtlas)
		{
			if (ResourceSerializer::SpriteAtlasResourceDataCache.find(uuid) != ResourceSerializer::SpriteAtlasResourceDataCache.end())
			{
				delete ResourceSerializer::SpriteAtlasResourceDataCache.at(uuid);
				ResourceSerializer::SpriteAtlasResourceDataCache.erase(uuid);
				ResourceSerializer::ResourceTypeInfo.erase(uuid);
			}
		}
		else if (resourceType == ResourceType::Script)
		{
			if (ResourceSerializer::SpriteAtlasResourceDataCache.find(uuid) != ResourceSerializer::SpriteAtlasResourceDataCache.end())
			{
				delete ResourceSerializer::SpriteAtlasResourceDataCache.at(uuid);
				ResourceSerializer::SpriteAtlasResourceDataCache.erase(uuid);
				ResourceSerializer::ResourceTypeInfo.erase(uuid);
			}
		}
		else if (resourceType == ResourceType::Font)
		{
			if (uuid == Font::GetDefaultFontUUID())
			{
				EG_CORE_ERROR("Cannot delete default font");
				return;
			}

			FontResourceData* data = ResourceSerializer::FontResourceDataCache[uuid];

			if (deleteFile)
			{
				std::filesystem::path finalPath = GetResourcePath(uuid);
				std::remove(finalPath.string().c_str());
			}

			ResourceSerializer::FontResourceDataCache.erase(uuid);
			ResourceSerializer::ResourceTypeInfo.erase(uuid);

			delete data;
		}


		else
		{
			EG_CORE_ERROR("Resource type not supported for deletion");
		}
	}

	void* ResourceDatabase::GetResourceData(UUID uuid)
	{
		if (ResourceSerializer::ResourceTypeInfo.find(uuid) == ResourceSerializer::ResourceTypeInfo.end())
		{
			EG_CORE_ERROR("Resource not found in cache");
			return nullptr;
		}

		ResourceType type = ResourceSerializer::ResourceTypeInfo.at(uuid);

		if (type == ResourceType::Image)
			return ResourceSerializer::TextureResourceDataCache.at(uuid);
		else if (type == ResourceType::SubTexture)
			return ResourceSerializer::SubTextureResourceDataCache.at(uuid);
		else if (type == ResourceType::Animation)
			return ResourceSerializer::AnimationResourceDataCache.at(uuid);
		else if (type == ResourceType::SpriteAtlas)
			return ResourceSerializer::SpriteAtlasResourceDataCache.at(uuid);
		else if (type == ResourceType::Font)
			return ResourceSerializer::FontResourceDataCache.at(uuid);
		else if(type == ResourceType::Script)
			return ResourceSerializer::ScriptResourceDataCache.at(uuid);
		else
		{
			EG_CORE_ERROR("Resource type not supported");
			return nullptr;
		}
	}

	bool ResourceDatabase::FindResourceData(UUID uuid)
	{
		return ResourceSerializer::ResourceTypeInfo.find(uuid) != ResourceSerializer::ResourceTypeInfo.end();
	}

	std::filesystem::path ResourceDatabase::GetResourcePath(UUID uuid)
	{
		ResourceData* data = (ResourceData*)ResourceDatabase::GetResourceData(uuid);

		return AssetDirectoryManager::getDirectoryPath(data->ParentDirectory) / std::string(data->ResourceName + data->Extension);
	}

	std::filesystem::path ResourceDatabase::GetResourcePath(std::filesystem::path path, ResourceType type)
	{
		return AssetDirectoryManager::getDirectoryPath(AssetDirectoryManager::GetRootAssetTypeDirectory(type)) / path.filename();
	}

	std::filesystem::path ResourceDatabase::GetResourcePath(std::filesystem::path path)
	{
		return AssetDirectoryManager::getDirectoryPath(m_CurrentDirectory) / path.filename();
	}

	bool ResourceDatabase::MoveResource(UUID uuid, UUID parentDirectory)
	{
		if (!FindResourceData(uuid))
		{
			EG_CORE_ERROR("Resource not found in cache");
			return false;
		}

		ResourceData* data = (ResourceData*)ResourceDatabase::GetResourceData(uuid);

		std::filesystem::path path = GetResourcePath(uuid);

		AssetDirectoryManager::moveAsset(uuid, data->ParentDirectory, parentDirectory);
		data->ParentDirectory = parentDirectory;

		std::filesystem::path newPath = GetResourcePath(uuid);

		if (path != newPath)
		{
			std::filesystem::rename(path, newPath);
		}

		return true;
	}

	bool ResourceDatabase::RenameResource(UUID uuid, const std::string& name)
	{
		if(!FindResourceData(uuid))
		{
			EG_CORE_ERROR("Resource not found in cache");
			return false;
		}

		ResourceData* data = (ResourceData*)ResourceDatabase::GetResourceData(uuid);

		std::filesystem::path path = GetResourcePath(uuid);
		std::filesystem::path newPath = AssetDirectoryManager::getDirectoryPath(data->ParentDirectory) / std::string(name + data->Extension);

		data->ResourceName = name;
		std::filesystem::rename(path, newPath);

		return true;
	}

	void AddTextureResource(UUID uuid, const std::filesystem::path& originalResourcePath, TextureResourceData* data)
	{
		std::filesystem::path finalPath = ResourceDatabase::GetResourcePath(originalResourcePath, ResourceType::Image);

		if (finalPath != originalResourcePath)
		{
			if (!std::filesystem::exists(finalPath.parent_path()))
			{
				std::filesystem::create_directories(finalPath.parent_path());
			}

			std::filesystem::copy(originalResourcePath, finalPath, std::filesystem::copy_options::overwrite_existing);
		}

		ResourceSerializer::CacheTexture(uuid, data);
	}

	void AddSubTextureResource(UUID uuid, const std::filesystem::path& originalResourcePath, SubTextureResourceData* data)
	{
		std::filesystem::path finalPath = ResourceDatabase::GetResourcePath(originalResourcePath, ResourceType::SubTexture);

		if (finalPath != originalResourcePath)
		{
			if (!std::filesystem::exists(finalPath.parent_path()))
			{
				std::filesystem::create_directories(finalPath.parent_path());
			}
		}

		ResourceSerializer::CacheSubTexture(uuid, data);
	}

	void AddFontResourceData(UUID uuid, const std::filesystem::path& originalResourcePath, FontResourceData* data)
	{
		std::filesystem::path finalPath = ResourceDatabase::GetResourcePath(originalResourcePath, ResourceType::Font);

		if (finalPath != originalResourcePath)
		{
			if (!std::filesystem::exists(finalPath.parent_path()))
			{
				std::filesystem::create_directories(finalPath.parent_path());
			}

			std::filesystem::copy(originalResourcePath, finalPath, std::filesystem::copy_options::overwrite_existing);
		}

		ResourceSerializer::CacheFont(uuid, data);
	}

	void AddScriptResourceData(UUID uuid, const std::filesystem::path& originalResource, ScriptResourceData* data)
	{
		std::filesystem::path finalPath = ResourceDatabase::GetResourcePath(originalResource, ResourceType::Script);

		if (!std::filesystem::exists(finalPath.parent_path()))
		{
			std::filesystem::create_directories(finalPath.parent_path());
		}

		ResourceSerializer::CacheScript(uuid, data);
	}

	void AddAnimationResource(UUID uuid, const std::filesystem::path& originalResourcePath, AnimationResourceData* data)
	{
		std::filesystem::path finalPath = ResourceDatabase::GetResourcePath(originalResourcePath, ResourceType::Animation);

		if (!std::filesystem::exists(finalPath.parent_path()))
		{
			std::filesystem::create_directories(finalPath.parent_path());
		}

		ResourceSerializer::CacheAnimation(uuid, data);
	}

	void AddSpriteAtlasResource(UUID uuid, const std::filesystem::path& originalResourcePath, SpriteAtlasResourceData* data)
	{
		std::filesystem::path finalPath = ResourceDatabase::GetResourcePath(originalResourcePath, ResourceType::SpriteAtlas);

		if (finalPath != originalResourcePath)
		{
			if (!std::filesystem::exists(finalPath.parent_path()))
			{
				std::filesystem::create_directories(finalPath.parent_path());
			}
		}

		ResourceSerializer::CacheSpriteAtlas(uuid, data);
	}

	UUID ResourceDatabase::GetResourceParentDirectory(UUID uuid)
	{
		ResourceData* data = (ResourceData*)ResourceDatabase::GetResourceData(uuid);

		return data->ParentDirectory;
	}

	UUID ResourceDatabase::LoadResource(const std::filesystem::path& filePath)
	{
		ResourceType type = ResourceUtils::GetResourceTypeByExtension(filePath.extension().string());

		/*if (ResourceDatabase::FindResourceByKeyPath(ResourceUtils::GetKeyPath(filePath), type) != 0)
		{
			EG_CORE_ERROR("Resource already exists: {0}", filePath.string());
			return;
		}*/

		UUID uuid = UUID();

		if (type == ResourceType::Image)
		{
			Resource* loadedResource = new Resource();
			bool resourceLoad = resourceSystemLoad(filePath.string(), ResourceType::Image, loadedResource);

			if(!resourceLoad)
			{
				EG_CORE_ERROR("Failed to load resource: {0}", filePath.string());
				return 0;
			}

			TextureResourceData* data = new TextureResourceData();
			data->ParentDirectory = AssetDirectoryManager::GetRootAssetTypeDirectory(type);
			data->ResourceName = filePath.stem().string();
			data->Extension = filePath.extension().string();
			data->Height = ((ImageResourceData*)loadedResource->Data)->height;
			data->Width = ((ImageResourceData*)loadedResource->Data)->width;
			data->Channels = ((ImageResourceData*)loadedResource->Data)->channelCount;
			AddTextureResource(uuid, filePath, data);

			return uuid;
		}
		else if (type == ResourceType::Font)
		{
			FontResourceData* data = new FontResourceData();
			data->ParentDirectory = AssetDirectoryManager::GetRootAssetTypeDirectory(type);
			data->ResourceName = filePath.stem().string();
			data->Extension = filePath.extension().string();
			AddFontResourceData(uuid, filePath, data);

			return uuid;
		}
		else
		{
			EG_CORE_ERROR("Resource type not supported");
			return 0;
		}
	}

	void* ResourceDatabase::LoadRuntimeResource(UUID uuid, ResourceType type)
	{
		if (!FindResourceData(uuid, type))
		{
			EG_CORE_ERROR("Resource not found in cache");
			return nullptr;
		}

		std::string fullPath = GetResourcePath(uuid).string();

		if(FindRuntimeResource(uuid, type))
			return GetRuntimeResource(uuid, type);

		if (type == ResourceType::Image)
			return AddRuntimeResource(uuid, (void*)fullPath.c_str(), type);

		Resource* loadedResource = new Resource();
		bool resourceLoad = resourceSystemLoad(fullPath, ResourceType::Font, loadedResource);

		if (!resourceLoad)
		{
			EG_CORE_ERROR("Failed to load resource: {0}", fullPath);
			return nullptr;
		}

		if (type == ResourceType::Font)
		{
			Font* font = (Font*)loadedResource->Data;
			return AddRuntimeResource(uuid, font, type);
		}
		else
		{
			EG_CORE_ERROR("Resource type not supported for runtime loading");
			return nullptr;
		}
	}

	UUID ResourceDatabase::GetResourceByData(void* data)
	{
		ResourceData* resourceData = (ResourceData*)data;

		UUID parentDirectory = resourceData->ParentDirectory;

		std::vector<UUID>& assets = AssetDirectoryManager::getAssetDirectory(parentDirectory)->getAssets();

		for (UUID asset : assets)
		{
			ResourceData* data = (ResourceData*)GetResourceData(asset);

			if (data->ResourceName == resourceData->ResourceName && data->Extension == resourceData->Extension)
				return asset;
		}

		return 0;
	}

	std::string ResourceDatabase::GetResourceName(UUID uuid)
	{
		if (!FindResourceData(uuid))
		{
			EG_CORE_ERROR("Resource not found in cache");
			return "";
		}

		ResourceData* data = (ResourceData*)GetResourceData(uuid);

		return data->ResourceName;
	}

	UUID ResourceDatabase::AddResource(const std::filesystem::path& originalResourcePath, void* data, ResourceType resourceType)
	{
		UUID uuid = GetResourceByData(data);

		if (uuid != 0)
			return uuid;

		uuid = UUID();

		switch (resourceType)
		{
		case ResourceType::Image:
			AddTextureResource(uuid, originalResourcePath, (TextureResourceData*)data);
			break;
		case ResourceType::SubTexture:
			AddSubTextureResource(uuid, originalResourcePath, (SubTextureResourceData*)data);
			break;
		case ResourceType::Animation:
			AddAnimationResource(uuid, originalResourcePath, (AnimationResourceData*)data);
			break;
		case ResourceType::SpriteAtlas:
			AddSpriteAtlasResource(uuid, originalResourcePath, (SpriteAtlasResourceData*)data);
			break;
		case ResourceType::Font:
			AddFontResourceData(uuid, originalResourcePath, (FontResourceData*)data);
			break;
		case ResourceType::Script:
			AddScriptResourceData(uuid, originalResourcePath, (ScriptResourceData*)data);
			break;
		}

		return uuid;
	}
}
