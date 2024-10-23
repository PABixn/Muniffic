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
		static std::string GetAssetDirectoryResourceTypeString(ResourceType type)
		{
			switch (type)
			{
			case ResourceType::Image:
				return "Textures";
			case ResourceType::SubTexture:
				return "SubTextures";
			case ResourceType::SpriteAtlas:
				return "SpriteAtlas";
			case ResourceType::Shader:
				return "Shaders";
			case ResourceType::Font:
				return "Fonts";
			case ResourceType::Animation:
				return "Animations";
			case ResourceType::Frame:
				return "Frames";
			case ResourceType::Script:
				return "Scripts";
			case ResourceType::Audio:
				return "Audio";
			default:
				return "None";
			}
		}

		static std::string GetResourceTypeExtension(ResourceType type)
		{
			switch (type)
			{
			case ResourceType::Image:
				return ".png";
			case ResourceType::SubTexture:
				return ".subtexture";
			case ResourceType::SpriteAtlas:
				return ".spriteatlas";
			case ResourceType::Shader:
				return ".shader";
			case ResourceType::Font:
				return ".ttf";
			case ResourceType::Text:
				return ".txt";
			case ResourceType::Animation:
				return ".anim";
			case ResourceType::Frame:
				return ".frame";
			case ResourceType::Script:
				return ".script";
			case ResourceType::NativeScript:
				return ".nativescript";
			case ResourceType::Audio:
				return ".wav";
			default:
				return "";
			}
		}

		static ResourceType GetResourceTypeByExtension(std::string extension)
		{

			if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
				return ResourceType::Image;
			else if (extension == ".subtexture")
				return ResourceType::SubTexture;
			else if (extension == ".spriteatlas")
				return ResourceType::SpriteAtlas;
			else if (extension == ".shader")
				return ResourceType::Shader;
			else if (extension == ".ttf")
				return ResourceType::Font;
			else if (extension == ".txt")
				return ResourceType::Text;
			else if (extension == ".anim")
				return ResourceType::Animation;
			else if (extension == ".frame")
				return ResourceType::Frame;
			else if (extension == ".cs")
				return ResourceType::Script;
			else if (extension == ".nativescript")
				return ResourceType::NativeScript;
			else if (extension == ".wav" || extension == ".mp3")
				return ResourceType::Audio;
			else
				return ResourceType::None;
		}

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
			case ResourceType::Frame:
				return baseDirectory / "Frames.mnmeta";
			case ResourceType::Script:
				return baseDirectory / "Scripts.mnmeta";
			case ResourceType::NativeScript:
				return baseDirectory / "NativeScripts.mnmeta";
			case ResourceType::Audio:
				return baseDirectory / "Audio.mnmeta";
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
			return true;
		}
	};
}