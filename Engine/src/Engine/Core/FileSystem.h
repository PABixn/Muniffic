#pragma once

#include <filesystem>
#include <fstream>
namespace eg {
	// TODO: platforms
	class Buffer;
	class FileSystem
	{
	public:
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};
}