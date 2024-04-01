#pragma once

#include "egpch.h"
#include "resourceTypes.h"
#include "Engine/Project/Project.h"	
#include "ResourceDatabase.h"

namespace eg
{
	class ResourceUtils
	{
	public:
		static std::filesystem::path GetFullPath(UUID uuid)
		{
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / GetKeyPath(uuid);
		}

		static std::string GetResourceTypeExtension(ResourceType type)
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

		static std::string GetResourceName(UUID uuid)
		{
			ResourceType type = ResourceDatabase::GetResourceType(uuid);

			if (type == ResourceType::Image)
				return ((TextureResourceData*)ResourceDatabase::GetResourceData(uuid, type))->ImageName;
			else if (type == ResourceType::SubTexture)
				return ((SubTextureResourceData*)ResourceDatabase::GetResourceData(uuid, type))->SubTextureName;
			else if (type == ResourceType::Animation)
				return ((AnimationResourceData*)ResourceDatabase::GetResourceData(uuid, type))->AnimationName;
			else if (type == ResourceType::SpriteAtlas)
				return ((SpriteAtlasResourceData*)ResourceDatabase::GetResourceData(uuid, type))->AtlasName;
			else if (type == ResourceType::Font)
				return ((FontResourceData*)ResourceDatabase::GetResourceData(uuid, type))->FontName;
			else
			{
				EG_CORE_ERROR("Resource type not supported");
				return std::string();
			}
		}

		static ResourceType GetResourceTypeByExtension(const std::string& extension)
		{
			if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" || extension == ".bmp" || extension == ".tga" || extension == ".gif" || extension == ".psd" || extension == ".hdr" || extension == ".pic" || extension == ".pnm")
				return ResourceType::Image;
			else if (extension == ".subtex")
				return ResourceType::SubTexture;
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

		static std::filesystem::path GetResourcePath(UUID uuid)
		{
			ResourceType type = ResourceDatabase::GetResourceType(uuid);

			if (type == ResourceType::Image)
				return ((TextureResourceData*)ResourceDatabase::GetResourceData(uuid, type))->ResourcePath;
			else if (type == ResourceType::SubTexture)
				return ((SubTextureResourceData*)ResourceDatabase::GetResourceData(uuid, type))->ResourcePath;
			else if (type == ResourceType::Animation)
				return ((AnimationResourceData*)ResourceDatabase::GetResourceData(uuid, type))->ResourcePath;
			else if (type == ResourceType::SpriteAtlas)
				return ((SpriteAtlasResourceData*)ResourceDatabase::GetResourceData(uuid, type))->ResourcePath;
			else if (type == ResourceType::Font)
				return ((FontResourceData*)ResourceDatabase::GetResourceData(uuid, type))->ResourcePath;
			else
			{
				EG_CORE_ERROR("Resource type not supported");
				return std::filesystem::path();
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
			ResourceType type = ResourceDatabase::GetResourceType(uuid);

			if (type == ResourceType::Image)
			{
				TextureResourceData* data = (TextureResourceData*)ResourceDatabase::GetResourceData(uuid, ResourceType::Image);
				return data->ResourcePath / std::filesystem::path(data->ImageName + data->Extension);
			}
			else if (type == ResourceType::SubTexture)
			{
				SubTextureResourceData* data = (SubTextureResourceData*)ResourceDatabase::GetResourceData(uuid, ResourceType::SubTexture);
				return data->ResourcePath / std::filesystem::path(data->SubTextureName + data->Extension);
			}
			else if (type == ResourceType::Animation)
			{
				AnimationResourceData* data = (AnimationResourceData*)ResourceDatabase::GetResourceData(uuid, ResourceType::Animation);
				return data->ResourcePath / std::filesystem::path(data->AnimationName + data->Extension);
			}
			else if (type == ResourceType::SpriteAtlas)
			{
				SpriteAtlasResourceData* data = (SpriteAtlasResourceData*)ResourceDatabase::GetResourceData(uuid, ResourceType::SpriteAtlas);
				return data->ResourcePath / std::filesystem::path(data->AtlasName + data->Extension);
			}
			else if (type == ResourceType::Font)
			{
				FontResourceData* data = (FontResourceData*)ResourceDatabase::GetResourceData(uuid, ResourceType::Font);
				return data->ResourcePath / std::filesystem::path(data->FontName + data->Extension);
			}
			else
				return std::filesystem::path();
		}

		static void* GetResourcePointer(UUID uuid, ResourceType type)
		{
			switch (type)
			{
			case ResourceType::Image:
				return ResourceDatabase::GetResourceData(uuid, ResourceType::Image);
			case ResourceType::SubTexture:
				return ResourceDatabase::GetResourceData(uuid, ResourceType::SubTexture);
			case ResourceType::SpriteAtlas:
				return ResourceDatabase::GetResourceData(uuid, ResourceType::SpriteAtlas);
			case ResourceType::Animation:
				return ResourceDatabase::GetResourceData(uuid, ResourceType::Animation);
			case ResourceType::Font:
				return ResourceDatabase::GetResourceData(uuid, ResourceType::Font);
			default:
				return nullptr;
			}
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
			case ResourceType::Script:
				return baseDirectory / "Scripts.mnmeta";
			case ResourceType::NativeScript:
				return baseDirectory / "NativeScripts.mnmeta";
			default:
				return std::filesystem::path();
			}
		}

		static ResourceType GetResourceTypeFromText(const std::string type)
		{
			if (type == "Textures")
				return ResourceType::Image;
			else if (type == "SubTextures")
				return ResourceType::SubTexture;
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
			case ResourceType::SubTexture:
				return "SubTextures";
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