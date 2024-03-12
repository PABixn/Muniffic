#pragma once

#include "egpch.h"
#include "ResourceSerializer.h"
#include "resourceTypes.h"
#include "Engine/Project/Project.h"	

namespace eg
{
	class ResourceUtils
	{
	public:
		static ResourceType GetResourceTypeByExtension(const std::string& extension)
		{
			if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || extension == ".bmp" || extension == ".tga" || extension == ".gif" || extension == ".psd" || extension == ".hdr" || extension == ".pic" || extension == ".pnm")
				return ResourceType::Image;
			else if (extension == ".shader")
				return ResourceType::Shader;
			else if (extension == ".ttf" || extension == ".otf")
				return ResourceType::Font;
			else if (extension == ".spratl")
				return ResourceType::SpriteAtlas;
			else if (extension == ".txt")
				return ResourceType::Text;
			else if (extension == ".anim")
				return ResourceType::Animation;
			else if (extension == ".mnscript")
				return ResourceType::Script;
			else if (extension == ".nativescript")
				return ResourceType::NativeScript;
			else
				return ResourceType::None;
		}

		static ResourceType GetCurrentResourceDirectoryType(std::filesystem::path path)
		{
			if (path.string().rfind(Project::GetAssetDirectory().string()) != std::string::npos)
			{
				size_t pos = path.string().rfind(Project::GetAssetDirectory().string());
				if (pos + Project::GetAssetDirectory().string().length() < path.string().length())
				{
					std::string resourcePath = path.string().substr(pos + Project::GetAssetDirectory().string().length() + 1);
					std::string type = resourcePath.substr(0, resourcePath.find('\\'));
					return GetResourceTypeFromText(type);
				}
			}
		}

		static std::filesystem::path GetResourcePath(std::filesystem::path path)
		{
			std::filesystem::path keyPath = GetKeyPath(path);

			if (std::filesystem::is_directory(path))
				return keyPath;

			if (keyPath.string().rfind('\\') != std::string::npos)
				return keyPath.string().substr(0, keyPath.string().rfind('\\'));
			else
				return path;
		}

		static std::filesystem::path GetKeyPath(std::filesystem::path path)
		{
			if(path.string().rfind(Project::GetAssetDirectory().string()) == std::string::npos)
				return std::filesystem::path();

			if (path.string().rfind(Project::GetAssetDirectory().string()) + Project::GetAssetDirectory().string().length() + 1 >= path.string().length())
				return std::filesystem::path();

			return path.string().substr(path.string().rfind(Project::GetAssetDirectory().string()) + Project::GetAssetDirectory().string().length() + 1);
		}

		static bool CanDrop(const std::filesystem::path& path)
		{
			if (!std::filesystem::is_directory(path))
				return false;

			if (path.string().rfind(Project::GetAssetDirectory().string()) == std::string::npos)
				return false;

			if (path.string().rfind(Project::GetAssetDirectory().string()) + Project::GetAssetDirectory().string().length() + 1 >= path.string().length())
				return false;

			return true;
		}

		static std::filesystem::path GetKeyPath(UUID uuid)
		{
			ResourceType& type = ResourceSerializer::ResourceTypeInfo[uuid];

			if (type == ResourceType::Image)
				return ResourceSerializer::TextureResourceDataCache[uuid]->ResourcePath / std::filesystem::path(ResourceSerializer::TextureResourceDataCache[uuid]->ImageName + ResourceSerializer::TextureResourceDataCache[uuid]->Extension);
			else if(type == ResourceType::Animation)
				return ResourceSerializer::AnimationResourceDataCache[uuid]->ResourcePath / std::filesystem::path(ResourceSerializer::AnimationResourceDataCache[uuid]->AnimationName + ResourceSerializer::AnimationResourceDataCache[uuid]->Extension);
			else if(type == ResourceType::SpriteAtlas)
				return ResourceSerializer::SpriteAtlasResourceDataCache[uuid]->ResourcePath / std::filesystem::path(ResourceSerializer::SpriteAtlasResourceDataCache[uuid]->AtlasName + ResourceSerializer::SpriteAtlasResourceDataCache[uuid]->Extension);
			else
				return std::filesystem::path();
		}

		static void* GetResourcePointer(UUID uuid, ResourceType type)
		{
			switch (type)
			{
			case ResourceType::Image:
				return ResourceSerializer::TextureResourceDataCache[uuid];
			case ResourceType::Animation:
				return ResourceSerializer::AnimationResourceDataCache[uuid];
			case ResourceType::SpriteAtlas:
				return ResourceSerializer::SpriteAtlasResourceDataCache[uuid];
			default:
				return nullptr;
			}
		}

		static std::filesystem::path GetMetadataPath(ResourceType type)
		{
			switch (type)
			{
			case ResourceType::Image:
				return Project::GetProjectDirectory() / Project::GetAssetDirectory() / "metadata" / "Textures.mnmeta";
			case ResourceType::SpriteAtlas:
				return Project::GetProjectDirectory() / Project::GetAssetDirectory() / "metadata" / "SpriteAtlases.mnmeta";
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

		static ResourceType GetResourceTypeFromText(const std::string type)
		{
			if (type == "Textures")
				return ResourceType::Image;
			else if(type == "SpriteAtlases")
				return ResourceType::SpriteAtlas;
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

		static std::string GetResourceTypeText(ResourceType type)
		{
			switch (type)
			{
			case ResourceType::Image:
				return "Textures";
			case ResourceType::SpriteAtlas:
				return "SpriteAtlases";
			case ResourceType::Shader:
				return "Shaders";
			case ResourceType::Font:
				return "Fonts";
			case ResourceType::Text:
				return "Texts";
			case ResourceType::Animation:
				return "Animations";
			case ResourceType::Script:
				return "Scripts";
			case ResourceType::NativeScript:
				return "NativeScripts";
			default:
				return "";
			}
		}

		static ResourceType GetResourceTypeByKeyPath(std::filesystem::path path)
		{
			std::string type = path.string().substr(0, path.string().find('\\'));
			return GetResourceTypeFromText(type);
		}
	};
}