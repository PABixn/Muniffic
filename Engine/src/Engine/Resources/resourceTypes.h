#pragma once
#include <cstdint>
#include <string>
#include <filesystem>

namespace eg {
	enum class ResourceType
	{
		Texture,
		Shader,
		Font,
		Text,
		Image,
		Script,
		NativeScript,
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
}

