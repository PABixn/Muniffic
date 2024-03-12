#include "egpch.h"
#include "ResourceDatabase.h"
#include "ResourceSerializer.h"
#include "Engine/Project/Project.h"	
#include <yaml-cpp/yaml.h>
#include "ResourceUtils.h"
#include "Systems/ResourceSystem.h"

namespace eg
{
	std::filesystem::path* ResourceDatabase::m_CurrentDirectory;

	void ResourceDatabase::SetCurrentPath(std::filesystem::path* directory)
	{
		m_CurrentDirectory = directory;
	}

	std::filesystem::path* ResourceDatabase::GetCurrentPath()
	{
		return m_CurrentDirectory;
	}

	std::vector<std::filesystem::path> ResourceDatabase::GetResourcesInDirectory(const std::filesystem::path& directory)
	{
		return std::vector<std::filesystem::path>();
	}

	std::vector<std::filesystem::path> ResourceDatabase::FindResourcesByName(const std::string& name)
	{
		return std::vector<std::filesystem::path>();
	}

	UUID ResourceDatabase::FindResourceByKeyPath(const std::filesystem::path& keyPath, ResourceType type)
	{
		if (type == ResourceType::Image)
		{
			for (auto& [uuid, data] : ResourceSerializer::TextureResourceDataCache)
			{
				if (data->ResourcePath / std::filesystem::path(data->ImageName + data->Extension) == keyPath)
					return uuid;
			}
		}
		else if (type == ResourceType::Animation)
		{
			for (auto& [uuid, data] : ResourceSerializer::AnimationResourceDataCache)
			{
				if (data->ResourcePath / std::filesystem::path(data->AnimationName + data->Extension) == keyPath)
					return uuid;
			}
		}
		else
		{
			EG_CORE_ERROR("Resource type not supported");
		}

		return 0;
	}

	std::string ResourceDatabase::GetResourceName(UUID uuid)
	{
		ResourceType type = ResourceSerializer::ResourceTypeInfo.at(uuid);

		if (type == ResourceType::Image)
			return ResourceSerializer::TextureResourceDataCache[uuid]->ImageName;
		else if (type == ResourceType::Animation)
			return ResourceSerializer::AnimationResourceDataCache[uuid]->AnimationName;
		else
		{
			EG_CORE_ERROR("Resource type not supported");
			return std::string();
		}
	}

	std::filesystem::path ResourceDatabase::GetFullPath(UUID uuid)
	{
		ResourceType type = ResourceSerializer::ResourceTypeInfo.at(uuid);

		if (type == ResourceType::Image)
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / ResourceSerializer::TextureResourceDataCache[uuid]->ResourcePath / std::string(ResourceSerializer::TextureResourceDataCache[uuid]->ImageName + ResourceSerializer::TextureResourceDataCache[uuid]->Extension);
		else if (type == ResourceType::Animation)
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / ResourceSerializer::AnimationResourceDataCache[uuid]->ResourcePath / std::string(ResourceSerializer::AnimationResourceDataCache[uuid]->AnimationName + ResourceSerializer::AnimationResourceDataCache[uuid]->Extension);
		else
		{
			EG_CORE_ERROR("Resource type not supported");
			return std::filesystem::path();
		}
	
	}

	std::filesystem::path ResourceDatabase::GetResourcePath(UUID uuid)
	{
		ResourceType type = ResourceSerializer::ResourceTypeInfo.at(uuid);

		if (type == ResourceType::Image)
			return ResourceSerializer::TextureResourceDataCache[uuid]->ResourcePath;
		else if (type == ResourceType::Animation)
			return ResourceSerializer::AnimationResourceDataCache[uuid]->ResourcePath;
		else
		{
			EG_CORE_ERROR("Resource type not supported");
			return std::filesystem::path();
		}
	}

	UUID ResourceDatabase::GetResourceByKeyPath(const std::filesystem::path& keyPath, ResourceType type)
	{
		std::filesystem::path fullKeyPath = std::filesystem::path(ResourceUtils::GetResourceTypeText(type)) / keyPath;

		if (type == ResourceType::Image)
		{
			for (auto& [uuid, data] : ResourceSerializer::TextureResourceDataCache)
			{
				std::filesystem::path d = ResourceUtils::GetResourcePath(fullKeyPath);
				if (data->ResourcePath == ResourceUtils::GetResourcePath(fullKeyPath) && data->ImageName + data->Extension == fullKeyPath.filename().string())
					return uuid;
			}
		}
		else if (type == ResourceType::Animation)
		{
			for (auto& [uuid, data] : ResourceSerializer::AnimationResourceDataCache)
			{
				if (data->ResourcePath == ResourceUtils::GetResourcePath(fullKeyPath) && data->AnimationName + data->Extension == fullKeyPath.filename().string())
					return uuid;
			}
		}
		else
		{
			EG_CORE_ERROR("Resource type not supported");
			return 0;
		}
	}

	UUID ResourceDatabase::GetResourceByPath(const std::filesystem::path& path)
	{
		ResourceType type = ResourceUtils::GetCurrentResourceDirectoryType(path.parent_path());

		if (type == ResourceType::Image)
		{
			for (auto& [uuid, data] : ResourceSerializer::TextureResourceDataCache)
			{
				if (data->ResourcePath == ResourceUtils::GetResourcePath(path) && data->ImageName + data->Extension == path.filename().string())
					return uuid;
			}
		}
		else if(type == ResourceType::Animation)
		{
			for (auto& [uuid, data] : ResourceSerializer::AnimationResourceDataCache)
			{
				if (data->ResourcePath == ResourceUtils::GetResourcePath(path) && data->AnimationName + data->Extension == path.filename().string())
					return uuid;
			}
		}
		else
		{
			EG_CORE_ERROR("Resource type not supported");
		}
	}

	void ResourceDatabase::DeleteDirectory(const std::filesystem::path& directory)
	{
		ResourceType type = ResourceUtils::GetCurrentResourceDirectoryType(directory);
		std::filesystem::path resourcePath = ResourceUtils::GetResourcePath(directory);
		std::vector<UUID> resourcesToDelete;

		if (type == ResourceType::Image)
		{
			for (auto& [uuid, data] : ResourceSerializer::TextureResourceDataCache)
			{
				if (data->ResourcePath == resourcePath)
					resourcesToDelete.push_back(uuid);
			}

			for (auto& uuid : resourcesToDelete)
			{
				ResourceSerializer::TextureResourceDataCache.erase(uuid);
				ResourceSerializer::ResourceTypeInfo.erase(uuid);
			}
		}
		else if (type == ResourceType::Animation)
		{
			for (auto& [uuid, data] : ResourceSerializer::AnimationResourceDataCache)
			{
				if (data->ResourcePath == resourcePath)
					resourcesToDelete.push_back(uuid);
			}

			for (auto& uuid : resourcesToDelete)
			{
				ResourceSerializer::AnimationResourceDataCache.erase(uuid);
				ResourceSerializer::ResourceTypeInfo.erase(uuid);
			}
		}
		else
		{
			EG_CORE_ERROR("Resource type not supported for deletion");
		}

		std::filesystem::remove_all(directory);
	}

	void ResourceDatabase::RemoveResource(std::filesystem::path path, bool deleteFile)
	{
		UUID uuid = ResourceDatabase::GetResourceByKeyPath(path, ResourceUtils::GetCurrentResourceDirectoryType(path.parent_path()));
		ResourceType type = ResourceSerializer::ResourceTypeInfo.at(uuid);
		ResourceDatabase::RemoveResource(uuid, type, deleteFile);
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
				ResourceSerializer::TextureResourceDataCache.erase(uuid);
				ResourceSerializer::ResourceTypeInfo.erase(uuid);
				if (deleteFile)
				{
					std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / ((TextureResourceData*)data)->ResourcePath / std::string(((TextureResourceData*)data)->ImageName + ((TextureResourceData*)data)->Extension);
					std::remove(finalPath.string().c_str());
				}
			}
		}
		else if(resourceType == ResourceType::Animation)
		{
			if (ResourceSerializer::AnimationResourceDataCache.find(uuid) != ResourceSerializer::AnimationResourceDataCache.end())
			{
				ResourceSerializer::AnimationResourceDataCache.erase(uuid);
				ResourceSerializer::ResourceTypeInfo.erase(uuid);
			}
		}
		else
		{
			EG_CORE_ERROR("Resource type not supported for deletion");
		}
	}

	void ResourceDatabase::RenameResource(UUID uuid, const std::string& name)
	{
		ResourceType type = ResourceSerializer::ResourceTypeInfo.at(uuid);

		std::filesystem::path oldPath = GetFullPath(uuid);
		std::filesystem::path newPath = oldPath.parent_path() / name;

		if (type == ResourceType::Image)
		{
			ResourceSerializer::TextureResourceDataCache.at(uuid)->ImageName = name;
			newPath = newPath.string() + ResourceSerializer::TextureResourceDataCache.at(uuid)->Extension;
		}
		else if (type == ResourceType::Animation)
		{
			ResourceSerializer::AnimationResourceDataCache.at(uuid)->AnimationName = name;
			newPath = newPath.string() + ResourceSerializer::AnimationResourceDataCache.at(uuid)->Extension;
		}
		else
			EG_CORE_ERROR("Resource type not supported for renaming: {0}", oldPath.string());

		std::filesystem::rename(oldPath, newPath);
	}

	void ResourceDatabase::RenameDirectory(const std::filesystem::path& oldPath, const std::string& name)
	{
		ResourceType type = ResourceUtils::GetCurrentResourceDirectoryType(oldPath);

		const std::filesystem::path newPath = oldPath.parent_path() / name;

		std::filesystem::rename(oldPath, newPath);

		if (type == ResourceType::Image)
		{
			std::filesystem::path newResourcePath = ResourceUtils::GetResourcePath(newPath);

			for (auto& [uuid, data] : ResourceSerializer::TextureResourceDataCache)
			{
				if (data->ResourcePath == ResourceUtils::GetResourcePath(oldPath))
					data->ResourcePath = newResourcePath;
			}
		}
		else if (type == ResourceType::Animation)
		{
			std::filesystem::path newResourcePath = ResourceUtils::GetResourcePath(newPath);

			for (auto& [uuid, data] : ResourceSerializer::AnimationResourceDataCache)
			{
				if (data->ResourcePath == ResourceUtils::GetResourcePath(oldPath))
					data->ResourcePath = newResourcePath;
			}
		}
		else
		{
			EG_CORE_ERROR("Resource type not supported for renaming: {0}", oldPath.string());
		}
	}

	void AddTextureResource(UUID uuid, const std::filesystem::path& originalResourcePath, TextureResourceData* data)
	{
		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / data->ResourcePath / std::string(data->ImageName + data->Extension);

		if (finalPath != originalResourcePath)
			std::filesystem::copy(originalResourcePath, finalPath, std::filesystem::copy_options::overwrite_existing);

		ResourceSerializer::CacheTexture(uuid, data);
	}

	void AddAnimationResource(UUID uuid, const std::filesystem::path& originalResourcePath, AnimationResourceData* data)
	{
		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / data->ResourcePath / std::string(data->AnimationName + data->Extension);

		if (finalPath != originalResourcePath)
			std::filesystem::copy(originalResourcePath, finalPath, std::filesystem::copy_options::overwrite_existing);

		ResourceSerializer::CacheAnimation(uuid, data);
	}

	void AddSpriteAtlasResource(UUID uuid, const std::filesystem::path& originalResourcePath, SpriteAtlasResourceData* data)
	{
		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / data->ResourcePath / std::string(data->SpriteAtlasName + data->Extension);

		if (finalPath != originalResourcePath)
			std::filesystem::copy(originalResourcePath, finalPath, std::filesystem::copy_options::overwrite_existing);

		ResourceSerializer::CacheSpriteAtlas(uuid, data);
	}

	void ResourceDatabase::LoadResource(const std::filesystem::path& filePath)
	{
		ResourceType type = ResourceUtils::GetResourceTypeByExtension(filePath.extension().string());

		if (ResourceDatabase::FindResourceByKeyPath(ResourceUtils::GetKeyPath(filePath), type) != 0)
		{
			EG_CORE_ERROR("Resource already exists: {0}", filePath.string());
			return;
		}

		if (type == ResourceType::Image)
		{
			Resource* loadedResource = new Resource();
			bool resourceLoad = resourceSystemLoad(filePath.string(), ResourceType::Image, loadedResource);
			TextureResourceData* data = new TextureResourceData();
			data->ResourcePath = ResourceUtils::GetResourcePath(m_CurrentDirectory->string());
			data->ImageName = filePath.stem().string();
			data->Extension = filePath.extension().string();
			data->Height = ((ImageResourceData*)loadedResource->Data)->height;
			data->Width = ((ImageResourceData*)loadedResource->Data)->width;
			data->Channels = ((ImageResourceData*)loadedResource->Data)->channelCount;
			AddTextureResource(UUID(), filePath, data);
		}
		else
		{
			EG_CORE_ERROR("Resource type not supported");
		}
	}

	void ResourceDatabase::MoveResource(UUID uuid, const std::filesystem::path& path)
	{
		ResourceType type = ResourceSerializer::ResourceTypeInfo.at(uuid);
		std::filesystem::path keyPath = ResourceUtils::GetKeyPath(uuid);

		std::filesystem::path droppedPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / keyPath;

		if (std::filesystem::exists(droppedPath))
		{
			std::filesystem::path newPath = path / keyPath.filename();

			int result = std::rename(droppedPath.string().c_str(), newPath.string().c_str());

			if (result == 0)
			{
				if (type == ResourceType::Image)
					ResourceSerializer::TextureResourceDataCache[uuid]->ResourcePath = ResourceUtils::GetResourcePath(path);
			}
			else
			{
				EG_CORE_ERROR("Failed to move file {0}", droppedPath);
			}
		}
		else
		{
			EG_CORE_ERROR("File does not exist");
		}
	}

	UUID ResourceDatabase::AddResource(const std::filesystem::path& originalResourcePath, void* data, ResourceType resourceType)
	{
		UUID uuid = UUID();

		switch (resourceType)
		{
		case ResourceType::Image:
			AddTextureResource(uuid, originalResourcePath, (TextureResourceData*)data);
			break;
		case ResourceType::Animation:
			AddAnimationResource(uuid, originalResourcePath, (AnimationResourceData*)data);
			break;
		case ResourceType::SpriteAtlas:
			AddSpriteAtlasResource(uuid, originalResourcePath, (SpriteAtlasResourceData*)data);
			break;
		}

		return uuid;
	}
}