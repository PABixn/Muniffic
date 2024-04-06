#pragma once
#include <cstdint>
#include <string>
#include <filesystem>
#include "Engine/Project/Project.h"
#include "Engine/Core/UUID.h"

namespace eg
{
	enum class ResourceType
	{
		Animation,
		SpriteAtlas,
		SubTexture,
		Shader,
		Font,
		Text,
		Image,
		Script,
		NativeScript,
		None,
		Custom
	};

	struct Resource
	{
		uint32_t LoaderId;
		ResourceType Type;
		std::string Name;
		std::filesystem::path Path;
		uint64_t DataSize;
		void *Data;
	};

	struct ImageResourceData
	{
		uint8_t channelCount;
		uint32_t width;
		uint32_t height;
		unsigned char *pixels;
	};

	struct SpriteAtlasResourceData
	{
		int Width = 0, Height = 0;
		int Channels = 0;
		std::filesystem::path ResourcePath = "";
		std::string AtlasName = "";
		std::string Extension = "";
		std::vector<UUID> Sprites;
	};

	struct AnimationResourceData
	{
		float FrameRate;
		int FrameCount;
		bool Loop;
		std::vector<UUID> Frames;
		std::filesystem::path ResourcePath = "";
		std::string AnimationName = "";
		std::string Extension = "";
	};

	struct SubTextureResourceData
	{
		glm::vec2 TexCoords[4];
		std::filesystem::path ResourcePath = "";
		std::string SubTextureName = "";
		std::string Extension = "";
		UUID Texture;
	};

	struct TextureResourceData
	{
		int Width = 0, Height = 0;
		int Channels = 0;
		std::filesystem::path ResourcePath = "";
		std::string ImageName = "";
		std::string Extension = "";

		std::filesystem::path GetKeyPath()
		{
			return ResourcePath / std::filesystem::path(ImageName + Extension);
		}

		std::filesystem::path GetRelativePath()
		{
			return std::filesystem::path(Project::GetProjectName()) / Project::GetAssetDirectory() / ResourcePath / std::filesystem::path(ImageName + Extension);
		}

		std::filesystem::path GetAbsolutePath()
		{
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / GetRelativePath();
		}
	};

}
