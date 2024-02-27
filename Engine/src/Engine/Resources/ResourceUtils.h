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
		static void* GetResourcePointer(ResourceType type, std::filesystem::path& key)
		{
			switch (type)
			{
			case ResourceType::Image:
				return ResourceSerializer::TextureResourceDataCache[key];
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

		static ResourceType GetResourceTypeFromKeyPath(const std::filesystem::path& keyPath)
		{
			std::string type = keyPath.string().substr(keyPath.string().find('/'));

			if (type == "Textures")
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
	};
}