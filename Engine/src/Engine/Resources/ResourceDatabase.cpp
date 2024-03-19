#include "ResourceDatabase.h"
#include <filesystem>
#include <string>
#include <vector>
#include "egpch.h"
#include "Engine/Project/Project.h"	
#include <yaml-cpp/yaml.h>
#include "ResourceUtils.h"
#include "Systems/ResourceSystem.h"

namespace eg
{
	std::filesystem::path* ResourceDatabase::m_CurrentDirectory;

	std::unordered_map<UUID, Font*> ResourceDatabase::RuntimeFontResourceCache;

	void ResourceDatabase::AddRuntimeResource(UUID uuid, void* data, ResourceType type)
	{
		if (data == nullptr)
			return;

		if (type == ResourceType::Font)
			RuntimeFontResourceCache[uuid] = (Font*)data;
		else
		{
			EG_CORE_WARN("Resource type not suitable for adding to runtime");
			return;
		}
	}

	void* ResourceDatabase::GetRuntimeResource(UUID uuid, ResourceType type)
	{
		if (type == ResourceType::Font)
			return RuntimeFontResourceCache.at(uuid);
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
		else
		{
			EG_CORE_ERROR("Resource type not supported");
			return false;
		}
	}

	void* ResourceDatabase::GetResourceData(UUID uuid, ResourceType resourceType)
	{
		if (resourceType == ResourceType::Image)
			return ResourceSerializer::TextureResourceDataCache.at(uuid);
		else if(resourceType == ResourceType::SubTexture)
			return ResourceSerializer::SubTextureResourceDataCache.at(uuid);
		else if (resourceType == ResourceType::Animation)
			return ResourceSerializer::AnimationResourceDataCache.at(uuid);
		else if(resourceType == ResourceType::SpriteAtlas)
			return ResourceSerializer::SpriteAtlasResourceDataCache.at(uuid);
		else if(resourceType == ResourceType::Font)
			return ResourceSerializer::FontResourceDataCache.at(uuid);
		else
		{
			EG_CORE_ERROR("Resource type not supported");
			return nullptr;
		}
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
		else if (type == ResourceType::Font)
		{
			for (auto& [uuid, data] : ResourceSerializer::FontResourceDataCache)
			{
				if (data->ResourcePath / std::filesystem::path(data->FontName + data->Extension) == keyPath)
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
		else if(type == ResourceType::Font)
			return ResourceSerializer::FontResourceDataCache[uuid]->FontName;
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
				return ".ttf";
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
		else if(type == ResourceType::Font)
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / ResourceSerializer::FontResourceDataCache[uuid]->ResourcePath / std::string(ResourceSerializer::FontResourceDataCache[uuid]->FontName + ResourceSerializer::FontResourceDataCache[uuid]->Extension);
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
		else if(type == ResourceType::Font)
			return ResourceSerializer::FontResourceDataCache[uuid]->ResourcePath;
		else
		{
			EG_CORE_ERROR("Resource type not supported");
			return std::filesystem::path();
		}
	}

	UUID ResourceDatabase::GetResourceByKeyPathWithoutCategory(const std::filesystem::path& keyPath, ResourceType type)
	{
		std::filesystem::path fullKeyPath = std::filesystem::path(ResourceUtils::GetResourceTypeText(type)) / keyPath;

		return ResourceDatabase::FindResourceByKeyPath(fullKeyPath, type);
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
		else if (type == ResourceType::Font)
		{
			for (auto& [uuid, data] : ResourceSerializer::FontResourceDataCache)
			{
				if (data->ResourcePath == ResourceUtils::GetResourcePath(path) && data->FontName + data->Extension == path.filename().string())
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
		else if (type == ResourceType::Font)
		{
			for (auto& [uuid, data] : ResourceSerializer::FontResourceDataCache)
			{
				if (data->ResourcePath == resourcePath)
					resourcesToDelete.push_back(uuid);
			}

			for (auto& uuid : resourcesToDelete)
			{
				ResourceSerializer::FontResourceDataCache.erase(uuid);
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
		UUID uuid = ResourceDatabase::GetResourceByKeyPathWithoutCategory(path, ResourceUtils::GetCurrentResourceDirectoryType(path.parent_path()));
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
		else if (resourceType == ResourceType::Font)
		{
			if (ResourceSerializer::FontResourceDataCache.find(uuid) != ResourceSerializer::FontResourceDataCache.end())
			{
				ResourceSerializer::FontResourceDataCache.erase(uuid);
				ResourceSerializer::ResourceTypeInfo.erase(uuid);
			}

			if (deleteFile)
			{
				std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / ResourceSerializer::FontResourceDataCache[uuid]->ResourcePath / std::string(ResourceSerializer::FontResourceDataCache[uuid]->FontName + ResourceSerializer::FontResourceDataCache[uuid]->Extension);
				std::remove(finalPath.string().c_str());
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
		else if (type == ResourceType::Font)
		{
			ResourceSerializer::FontResourceDataCache.at(uuid)->FontName = name;
			newPath = newPath.string() + ResourceSerializer::FontResourceDataCache.at(uuid)->Extension;
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
		else if (type == ResourceType::Font)
		{
			std::filesystem::path newResourcePath = ResourceUtils::GetResourcePath(newPath);

			for (auto& [uuid, data] : ResourceSerializer::FontResourceDataCache)
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

		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / data->ResourcePath / std::string(data->SubTextureName + data->Extension);

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
		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / data->ResourcePath / std::string(data->FontName + data->Extension);

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

	void AddAnimationResource(UUID uuid, const std::filesystem::path& originalResourcePath, AnimationResourceData* data)
	{
		AnimationResourceData* resourceData = new AnimationResourceData();
		memcpy(resourceData, data, sizeof(AnimationResourceData));
		std::filesystem::path resourcePath = ((AnimationResourceData*)data)->ResourcePath.string().c_str();
		resourceData->ResourcePath = resourcePath;

		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / data->ResourcePath / std::string(data->AnimationName + data->Extension);

		if (!std::filesystem::exists(finalPath.parent_path()))
		{
			std::filesystem::create_directories(finalPath.parent_path());
		}

		ResourceSerializer::CacheAnimation(uuid, data);
	}

	void AddSpriteAtlasResource(UUID uuid, const std::filesystem::path& originalResourcePath, SpriteAtlasResourceData* data)
	{
		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / data->ResourcePath / std::string(data->AtlasName + data->Extension);

		if (finalPath != originalResourcePath)
		{
			if (!std::filesystem::exists(finalPath.parent_path()))
			{
				std::filesystem::create_directories(finalPath.parent_path());
			}
		}

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

			if(!resourceLoad)
			{
				EG_CORE_ERROR("Failed to load resource: {0}", filePath.string());
				return;
			}

			TextureResourceData* data = new TextureResourceData();
			data->ResourcePath = ResourceUtils::GetResourcePath(m_CurrentDirectory->string());
			data->ImageName = filePath.stem().string();
			data->Extension = filePath.extension().string();
			data->Height = ((ImageResourceData*)loadedResource->Data)->height;
			data->Width = ((ImageResourceData*)loadedResource->Data)->width;
			data->Channels = ((ImageResourceData*)loadedResource->Data)->channelCount;
			AddTextureResource(UUID(), filePath, data);
		}
		else if (type == ResourceType::Font)
		{
			FontResourceData* data = new FontResourceData();
			data->ResourcePath = ResourceUtils::GetResourcePath(m_CurrentDirectory->string());
			data->FontName = filePath.stem().string();
			data->Extension = filePath.extension().string();
			AddFontResourceData(UUID(), filePath, data);
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
				else if (type == ResourceType::SubTexture)
					ResourceSerializer::SubTextureResourceDataCache[uuid]->ResourcePath = ResourceUtils::GetResourcePath(path);
				else if (type == ResourceType::Font)
					ResourceSerializer::FontResourceDataCache[uuid]->ResourcePath = ResourceUtils::GetResourcePath(path);
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
		case ResourceType::Font:
			AddFontResourceData(uuid, originalResourcePath, (FontResourceData*)data);
			break;
		}

		return uuid;
	}
}
