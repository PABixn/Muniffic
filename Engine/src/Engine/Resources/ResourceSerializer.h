#pragma once
#include <filesystem>
#include "resourceTypes.h"
#include <string>

namespace eg {

	class ResourceSerializer {
	public:
		ResourceSerializer() = default;
		~ResourceSerializer() = default;

		void SerializeTextureResource(const std::filesystem::path& filepath, const TextureResourceData& data);
		bool DeserializeTextureResource(const std::string& path, TextureResourceData* data);
	};
}