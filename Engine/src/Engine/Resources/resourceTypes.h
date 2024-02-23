#pragma once
#include <cstdint>
#include <string>
#include <filesystem>
#include "Engine/Project/Project.h"

namespace eg {
	enum class ResourceType
	{
		Animation,
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
		void* Data;
	};

	struct ImageResourceData
	{
		uint8_t channelCount;
		uint32_t width;
		uint32_t height;
		unsigned char* pixels;
	};

	struct TextureResourceData
	{
		int Width = 0, Height = 0, OriginalHeight = 0, OriginalWidth = 0;
		int Top = 0, Bottom = 0, Left = 0, Right = 0;
		int Channels = 0;
		std::filesystem::path ResourcePath = "";
		std::string ImageName = "";
		std::string Extension = "";

		std::filesystem::path GetRelativePath()
		{
			return ResourcePath / std::filesystem::path(ImageName + Extension);
		}

		std::filesystem::path GetAbsolutePath()
		{
			return Project::GetProjectDirectory() / Project::GetAssetDirectory() / GetRelativePath();
		}
	};
}

