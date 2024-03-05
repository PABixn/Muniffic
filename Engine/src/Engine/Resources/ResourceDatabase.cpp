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

	std::filesystem::path ResourceDatabase::FindResourceByKeyPath(const std::filesystem::path& keyPath)
	{
		return std::filesystem::path();
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
		else
		{
			EG_CORE_ERROR("Resource type not supported for deletion");
		}

		std::filesystem::remove_all(directory);
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
		else
		{
			EG_CORE_ERROR("Resource type not supported for deletion");
		}
	}

	void ResourceDatabase::RenameResource(const std::filesystem::path& oldPath, const std::filesystem::path& newPath)
	{
		std::filesystem::rename(oldPath, newPath);
		ResourceType type;

		if(std::filesystem::is_directory(newPath))
			type = ResourceUtils::GetCurrentResourceDirectoryType(oldPath);
		else
			type = ResourceUtils::GetResourceTypeByExtension(oldPath.extension().string());

		bool isDirectory = std::filesystem::is_directory(newPath);

		if (type == ResourceType::Image)
		{
			std::filesystem::path newResourcePath = ResourceUtils::GetResourcePath(isDirectory ? newPath : newPath.parent_path());

			for (auto& [uuid, data] : ResourceSerializer::TextureResourceDataCache)
			{
				if (data->ResourcePath == ResourceUtils::GetResourcePath(oldPath))
					data->ResourcePath = newResourcePath;
			}
		}
	}

	void AddTextureResource(const std::filesystem::path& originalResourcePath, TextureResourceData* data)
	{
		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / data->ResourcePath / std::string(data->ImageName + data->Extension);

		if (finalPath != originalResourcePath)
			std::filesystem::copy(originalResourcePath, finalPath, std::filesystem::copy_options::overwrite_existing);

		ResourceSerializer::CacheTexture(UUID(), data);
	}

	void ResourceDatabase::LoadResource(const std::filesystem::path& filePath)
	{
		ResourceType type = ResourceUtils::GetResourceTypeByExtension(filePath.extension().string());

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
			AddTextureResource(filePath, data);
		}
		else
		{
			EG_CORE_ERROR("Resource type not supported");
		}
	}

	void ResourceDatabase::AddResource(const std::filesystem::path& originalResourcePath, void* data, ResourceType resourceType)
	{
		switch (resourceType)
		{
		case::eg::ResourceType::Image:
			AddTextureResource(originalResourcePath, (TextureResourceData*)data);
			break;
		}
	}
}