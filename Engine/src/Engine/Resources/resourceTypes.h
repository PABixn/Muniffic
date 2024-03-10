#pragma once
#include <cstdint>
#include <string>
#include <filesystem>
#include "Engine/Project/Project.h"
#include "Engine/Core/UUID.h"

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

	struct AnimationResourceData
	{
		float m_frameRate;
		int m_frameCount;
		bool m_loop;
		std::string name;
		std::vector<UUID> m_frames;
		std::filesystem::path ResourcePath = "";
		std::string AnimationName = "";
		std::string Extension = "";
	};

	struct TextureResourceData
	{
		int Width = 0, Height = 0;
		glm::vec2 m_TexCoords[4];
		bool IsSubTexture = false;
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
			return Project::GetProjectDirectory().parent_path() / GetRelativePath();
		}
	};
}

