#pragma once
#include <cstdint>
#include <string>
#include <filesystem>

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
		int width = 0, height = 0, originalHeight = 0, originalWidth = 0;
		int top = 0, bottom = 0, left = 0, right = 0;
		int channels = 0;
		std::filesystem::path imagePath = "";
		std::string imageName = "";
	};
}

