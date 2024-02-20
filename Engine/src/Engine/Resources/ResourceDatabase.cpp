#include "egpch.h"
#include "ResourceDatabase.h"
#include "ResourceSerializer.h"
#include "Engine/Project/Project.h"	
#include <yaml-cpp/yaml.h>

namespace eg
{
	std::filesystem::path GetMetadataPath(ResourceType type)
	{
		switch (type)
		{
		case ResourceType::Image:
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / "metadata" / "Textures.mnmeta";
		case ResourceType::Shader:
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / "metadata" / "Shaders.mnmeta";
		case ResourceType::Font:
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / "metadata" / "Fonts.mnmeta";
		case ResourceType::Text:
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / "metadata" / "Texts.mnmeta";
		case ResourceType::Animation:
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / "metadata" / "Animations.mnmeta";
		case ResourceType::Script:
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / "metadata" / "Scripts.mnmeta";
		case ResourceType::NativeScript:
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / "metadata" / "NativeScripts.mnmeta";
		default:
			return std::filesystem::path();
		}
	}

	ResourceType GetResourceType(const std::filesystem::path& keyPath)
	{
		std::string type = keyPath.string().substr(keyPath.string().find('/'));

		if(type == "Textures")
			return ResourceType::Image;
		else if (type == "Shaders")
			return ResourceType::Shader;
		else if (type == "Fonts")
			return ResourceType::Font;
		else if (type == "Texts")
			return ResourceType::Text;
		else if (type == "Animations")
			return ResourceType::Animation;
		else if (type == "Scripts")
			return ResourceType::Script;
		else if (type == "NativeScripts")
			return ResourceType::NativeScript;
		else
			return ResourceType::None;
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
		std::filesystem::path metadata = GetMetadataPath(GetResourceType(keyPath));

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
		std::filesystem::path metaDataDestination = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "metadata" / "Textures.mnmeta";
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