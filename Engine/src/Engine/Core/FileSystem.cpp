#include "egpch.h"
#include "FileSystem.h"
#include "Engine/Core/Buffer.h"

namespace eg {

	Buffer FileSystem::ReadFileBinary(const std::filesystem::path& filepath)
	{
		EG_PROFILE_FUNCTION();
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		if (!stream)
		{
			// Failed to open the file
			return {};
		}

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();

		if (size == 0)
		{
			// File is empty
			return {};
		}

		Buffer buffer(size);
		stream.read(buffer.As<char>(), buffer.Size);
		stream.close();

		return buffer;
	}
}
