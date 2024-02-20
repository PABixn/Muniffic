#include "egpch.h"
#include "ResourceDatabase.h"
#include "ResourceSerializer.h"
#include "Engine/Project/Project.h"	
#include <yaml-cpp/yaml.h>
#include "ResourceUtils.h"

namespace eg
{
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
		std::filesystem::path metadata = ResourceUtils::GetMetadataPath(ResourceUtils::GetResourceType(keyPath));

		YAML::Node data;

		try
		{
			data = YAML::LoadFile(metadata.string());
		}
		catch (YAML::ParserException e)
		{
			EG_CORE_ERROR("Failed to load .mnmeta file '{0}'\n     {1}", metadata, e.what());
		}

		return std::filesystem::path();
	}

	void AddTextureResource(const std::filesystem::path& originalResourcePath, TextureResourceData* data)
	{
		ResourceSerializer serializer;
		serializer.CacheTexture(*((TextureResourceData*)data));
		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / ((TextureResourceData*)data)->ResourcePath / std::string(((TextureResourceData*)data)->ImageName + ((TextureResourceData*)data)->Extension);

		if (finalPath != originalResourcePath)
			std::filesystem::copy(originalResourcePath, finalPath, std::filesystem::copy_options::overwrite_existing);
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