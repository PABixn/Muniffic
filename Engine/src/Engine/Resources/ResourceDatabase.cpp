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
		else if(type == ResourceType::SubTexture)
		{
			for (auto& [uuid, data] : ResourceSerializer::SubTextureResourceDataCache)
			{
				if (data->ResourcePath / std::filesystem::path(data->SubTextureName + data->Extension) == keyPath)
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
		else if (type == ResourceType::SpriteAtlas)
		{
			for (auto& [uuid, data] : ResourceSerializer::SpriteAtlasResourceDataCache)
			{
				if (data->ResourcePath / std::filesystem::path(data->AtlasName + data->Extension) == keyPath)
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
		else if(type == ResourceType::SubTexture)
			return ResourceSerializer::SubTextureResourceDataCache[uuid]->SubTextureName;
		else if (type == ResourceType::Animation)
			return ResourceSerializer::AnimationResourceDataCache[uuid]->AnimationName;
		else if(type == ResourceType::SpriteAtlas)
			return ResourceSerializer::SpriteAtlasResourceDataCache[uuid]->AtlasName;
		else
		{
			EG_CORE_ERROR("Resource type not supported");
			return std::string();
		}
	}

	std::string ResourceDatabase::GetResourceTypeExtension(ResourceType type)
	{
		switch (type)
		{
			case ResourceType::Image:
				return ".png";
			case ResourceType::SubTexture:
				return ".png";
			case ResourceType::Animation:
				return ".anim";
			case ResourceType::SpriteAtlas:
				return ".atlas";
			case ResourceType::Shader:
				return ".shader";
			case ResourceType::Font:
				return ".font";
			case ResourceType::NativeScript:
				return ".cpp";
			case ResourceType::Script:
				return ".cs";
			case ResourceType::Custom:
				return ".custom";
			default:
				return ".unknown";
		}
	}

	std::filesystem::path ResourceDatabase::GetFullPath(UUID uuid)
	{
		ResourceType type = ResourceSerializer::ResourceTypeInfo.at(uuid);

		if (type == ResourceType::Image)
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / ResourceSerializer::TextureResourceDataCache[uuid]->ResourcePath / std::string(ResourceSerializer::TextureResourceDataCache[uuid]->ImageName + ResourceSerializer::TextureResourceDataCache[uuid]->Extension);
		else if (type == ResourceType::SubTexture)
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / ResourceSerializer::SubTextureResourceDataCache[uuid]->ResourcePath / std::string(ResourceSerializer::SubTextureResourceDataCache[uuid]->SubTextureName + ResourceSerializer::SubTextureResourceDataCache[uuid]->Extension);
		else if (type == ResourceType::Animation)
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / ResourceSerializer::AnimationResourceDataCache[uuid]->ResourcePath / std::string(ResourceSerializer::AnimationResourceDataCache[uuid]->AnimationName + ResourceSerializer::AnimationResourceDataCache[uuid]->Extension);
		else if (type == ResourceType::SpriteAtlas)
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / ResourceSerializer::SpriteAtlasResourceDataCache[uuid]->ResourcePath / std::string(ResourceSerializer::SpriteAtlasResourceDataCache[uuid]->AtlasName + ResourceSerializer::SpriteAtlasResourceDataCache[uuid]->Extension);
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
		else if(type == ResourceType::SubTexture)
			return ResourceSerializer::SubTextureResourceDataCache[uuid]->ResourcePath;
		else if (type == ResourceType::Animation)
			return ResourceSerializer::AnimationResourceDataCache[uuid]->ResourcePath;
		else if (type == ResourceType::SpriteAtlas)
			return ResourceSerializer::SpriteAtlasResourceDataCache[uuid]->ResourcePath;
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
		else if (type == ResourceType::SubTexture)
		{
			for (auto& [uuid, data] : ResourceSerializer::SubTextureResourceDataCache)
			{
				if (data->ResourcePath == ResourceUtils::GetResourcePath(fullKeyPath) && data->SubTextureName + data->Extension == fullKeyPath.filename().string())
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
		else if (type == ResourceType::SpriteAtlas)
		{
			for (auto& [uuid, data] : ResourceSerializer::SpriteAtlasResourceDataCache)
			{
				if (data->ResourcePath == ResourceUtils::GetResourcePath(fullKeyPath) && data->AtlasName + data->Extension == fullKeyPath.filename().string())
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
		else if(type == ResourceType::SubTexture)
		{
			for (auto& [uuid, data] : ResourceSerializer::SubTextureResourceDataCache)
			{
				if (data->ResourcePath == ResourceUtils::GetResourcePath(path) && data->SubTextureName + data->Extension == path.filename().string())
					return uuid;
			}
		}
		else if (type == ResourceType::Animation)
		{
			for (auto& [uuid, data] : ResourceSerializer::AnimationResourceDataCache)
			{
				if (data->ResourcePath == ResourceUtils::GetResourcePath(path) && data->AnimationName + data->Extension == path.filename().string())
					return uuid;
			}
		}
		else if (type == ResourceType::SpriteAtlas)
		{
			for (auto& [uuid, data] : ResourceSerializer::SpriteAtlasResourceDataCache)
			{
				if (data->ResourcePath == ResourceUtils::GetResourcePath(path) && data->AtlasName + data->Extension == path.filename().string())
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
		else if(type == ResourceType::SubTexture)
		{
			for (auto& [uuid, data] : ResourceSerializer::SubTextureResourceDataCache)
			{
				if (data->ResourcePath == resourcePath)
					resourcesToDelete.push_back(uuid);
			}

			for (auto& uuid : resourcesToDelete)
			{
				ResourceSerializer::SubTextureResourceDataCache.erase(uuid);
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
		else if (type == ResourceType::SpriteAtlas)
		{
			for (auto& [uuid, data] : ResourceSerializer::SpriteAtlasResourceDataCache)
			{
				if (data->ResourcePath == resourcePath)
					resourcesToDelete.push_back(uuid);
			}

			for (auto& uuid : resourcesToDelete)
			{
				ResourceSerializer::SpriteAtlasResourceDataCache.erase(uuid);
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
		else if(resourceType == ResourceType::SubTexture)
		{
			if (ResourceSerializer::SubTextureResourceDataCache.find(uuid) != ResourceSerializer::SubTextureResourceDataCache.end())
			{
				SubTextureResourceData* data = ResourceSerializer::SubTextureResourceDataCache[uuid];
				ResourceSerializer::SubTextureResourceDataCache.erase(uuid);
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
		else if (resourceType == ResourceType::SpriteAtlas)
		{
			if (ResourceSerializer::SpriteAtlasResourceDataCache.find(uuid) != ResourceSerializer::SpriteAtlasResourceDataCache.end())
			{
				ResourceSerializer::SpriteAtlasResourceDataCache.erase(uuid);
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
		else if(type == ResourceType::SubTexture)
		{
			ResourceSerializer::SubTextureResourceDataCache.at(uuid)->SubTextureName = name;
			newPath = newPath.string() + ResourceSerializer::SubTextureResourceDataCache.at(uuid)->Extension;
		}
		else if (type == ResourceType::Animation)
		{
			ResourceSerializer::AnimationResourceDataCache.at(uuid)->AnimationName = name;
			newPath = newPath.string() + ResourceSerializer::AnimationResourceDataCache.at(uuid)->Extension;
		}
		else if (type == ResourceType::SpriteAtlas)
		{
			ResourceSerializer::SpriteAtlasResourceDataCache.at(uuid)->AtlasName = name;
			newPath = newPath.string() + ResourceSerializer::SpriteAtlasResourceDataCache.at(uuid)->Extension;
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
		else if (type == ResourceType::SubTexture)
		{
			std::filesystem::path newResourcePath = ResourceUtils::GetResourcePath(newPath);

			for (auto& [uuid, data] : ResourceSerializer::SubTextureResourceDataCache)
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
		else if (type == ResourceType::SpriteAtlas)
		{
			std::filesystem::path newResourcePath = ResourceUtils::GetResourcePath(newPath);

			for (auto& [uuid, data] : ResourceSerializer::SpriteAtlasResourceDataCache)
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
		TextureResourceData* resourceData = new TextureResourceData();
		memcpy(resourceData, data, sizeof(TextureResourceData));
		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / resourceData->ResourcePath / std::string(resourceData->ImageName + resourceData->Extension);

		if (finalPath != originalResourcePath)
		{
			if (!std::filesystem::exists(finalPath.parent_path()))
			{
				std::filesystem::create_directories(finalPath.parent_path());
			}
			std::filesystem::copy(originalResourcePath, finalPath, std::filesystem::copy_options::overwrite_existing);
		}

		ResourceSerializer::CacheTexture(uuid, resourceData);
	}

	void AddSubTextureResource(UUID uuid, const std::filesystem::path& originalResourcePath, SubTextureResourceData* data)
	{
		SubTextureResourceData* resourceData = new SubTextureResourceData();
		memcpy(resourceData, data, sizeof(SubTextureResourceData));
		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / resourceData->ResourcePath / std::string(resourceData->SubTextureName + "subtexture");

		if (finalPath != originalResourcePath)
		{
			if (!std::filesystem::exists(finalPath.parent_path()))
			{
				std::filesystem::create_directories(finalPath.parent_path());
			}
			std::filesystem::copy(originalResourcePath, finalPath, std::filesystem::copy_options::overwrite_existing);
		}

		ResourceSerializer::CacheSubTexture(uuid, resourceData);
	}

	void AddAnimationResource(UUID uuid, const std::filesystem::path& originalResourcePath, AnimationResourceData* data)
	{
		AnimationResourceData* resourceData = new AnimationResourceData();
		memcpy(resourceData, data, sizeof(AnimationResourceData));
		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / resourceData->ResourcePath / std::string(resourceData->AnimationName + resourceData->Extension);

		ResourceSerializer::CacheAnimation(uuid, resourceData);
	}

	void AddSpriteAtlasResource(UUID uuid, const std::filesystem::path& originalResourcePath, SpriteAtlasResourceData* data)
	{
		SpriteAtlasResourceData* resourceData = new SpriteAtlasResourceData();
		memcpy(resourceData, data, sizeof(SpriteAtlasResourceData));
		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / resourceData->ResourcePath / std::string(resourceData->AtlasName + resourceData->Extension);

		if (finalPath != originalResourcePath)
		{
			if (!std::filesystem::exists(finalPath.parent_path()))
			{
				std::filesystem::create_directories(finalPath.parent_path());
			}
			std::filesystem::copy(originalResourcePath, finalPath, std::filesystem::copy_options::overwrite_existing);
		}

		ResourceSerializer::CacheSpriteAtlas(uuid, resourceData);
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
				else if (type == ResourceType::Animation)
					ResourceSerializer::AnimationResourceDataCache[uuid]->ResourcePath = ResourceUtils::GetResourcePath(path);
				else if (type == ResourceType::SpriteAtlas)
					ResourceSerializer::SpriteAtlasResourceDataCache[uuid]->ResourcePath = ResourceUtils::GetResourcePath(path);
				else
					EG_CORE_ERROR("Resource type not supported for moving");
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
		case ResourceType::SubTexture:
			AddSubTextureResource(uuid, originalResourcePath, (SubTextureResourceData*)data);
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
