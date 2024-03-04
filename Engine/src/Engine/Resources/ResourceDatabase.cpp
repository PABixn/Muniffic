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

	void AddTextureResource(const std::filesystem::path& originalResourcePath, TextureResourceData* data)
	{
		ResourceSerializer::CacheTexture(UUID(), data);
		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / data->ResourcePath / std::string(data->ImageName + data->Extension);

		if (finalPath != originalResourcePath)
			std::filesystem::copy(originalResourcePath, finalPath, std::filesystem::copy_options::overwrite_existing);
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