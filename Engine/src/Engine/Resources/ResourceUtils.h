#pragma once

#include "egpch.h"
#include "resourceTypes.h"
#include "Engine/Project/Project.h"	
#include "ResourceDatabase.h"
#include "AssetDirectoryManager.h"

namespace eg
{
	class ResourceUtils
	{
	public:
		static std::filesystem::path GetMetadataPath(ResourceType type)
		{
			std::filesystem::path baseDirectory = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "metadata";

			switch (type)
			{
			case ResourceType::Image:
				return baseDirectory / "Textures.mnmeta";
			case ResourceType::SubTexture:
				return baseDirectory / "SubTextures.mnmeta";
			case ResourceType::SpriteAtlas:
				return baseDirectory / "SpriteAtlases.mnmeta";
			case ResourceType::Shader:
				return baseDirectory / "Shaders.mnmeta";
			case ResourceType::Font:
				return baseDirectory / "Fonts.mnmeta";
			case ResourceType::Text:
				return baseDirectory / "Texts.mnmeta";
			case ResourceType::Animation:
				return baseDirectory / "Animations.mnmeta";
			case ResourceType::Script:
				return baseDirectory / "Scripts.mnmeta";
			case ResourceType::NativeScript:
				return baseDirectory / "NativeScripts.mnmeta";
			default:
				return std::filesystem::path();
			}
		}

		static std::filesystem::path GetAssetDirectoryMetadataPath()
		{
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / "metadata" / "AssetDirectory.mnmeta";
		}

		static bool CanDrop(UUID assetDirectory)
		{
			if(!AssetDirectoryManager::findAssetDirectory(assetDirectory))
				return false;
		}
	};
}