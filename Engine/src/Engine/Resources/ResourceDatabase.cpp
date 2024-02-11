#include "egpch.h"
#include "ResourceDatabase.h"
#include "ResourceSerializer.h"
#include "Engine/Project/Project.h"	

namespace eg
{
	std::vector<std::filesystem::path> ResourceDatabase::GetResourcesInDirectory(const std::filesystem::path& directory)
	{
		for (auto& directoryEntry : std::filesystem::directory_iterator(directory))
		{
			const std::filesystem::path path = directoryEntry.path();

			if (path.extension().string() == ".mnmeta")
			{
				
			}
		}
	}

	std::vector<std::filesystem::path> ResourceDatabase::FindResourcesByName(const std::string& name)
	{
		return std::vector<std::filesystem::path>();
	}

	void AddTextureResource(const std::filesystem::path& filePath, const std::filesystem::path& originalResourcePath, TextureResourceData* data)
	{
		ResourceSerializer serializer;
		std::filesystem::path metaDataDestination = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "metadata" / "Textures.mnmeta";
		serializer.SerializeTextureResource(metaDataDestination, *((TextureResourceData*)data));
		std::filesystem::path finalPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Textures" / (((TextureResourceData*)data)->ImageName + "." + ((TextureResourceData*)data)->Extension);

		if (finalPath != ((TextureResourceData*)data)->ResourcePath)
			std::filesystem::copy(originalResourcePath, finalPath, std::filesystem::copy_options::overwrite_existing);
	}

	void ResourceDatabase::AddResource(const std::filesystem::path& filePath, const std::filesystem::path& originalResourcePath, void* data, ResourceType resourceType)
	{
		switch (resourceType)
		{
		case::eg::ResourceType::Image:
			AddTextureResource(filePath, originalResourcePath, (TextureResourceData*)data);
			break;
		}
	}
}