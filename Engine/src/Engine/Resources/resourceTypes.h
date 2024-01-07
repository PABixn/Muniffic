#pragma once
#include <cstdint>
#include <string>
#include <filesystem>

enum class ResourceType
{
	Texture,
	Shader,
	Font,
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