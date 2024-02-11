#pragma once
#include "resourceTypes.h"
#include <filesystem>
#include <string>
#include <vector>

namespace eg {
	class ResourceDatabase
	{
	public:
		std::vector<std::filesystem::path> GetResourcesInDirectory(const std::filesystem::path& directory);
		std::vector<std::filesystem::path> FindResourcesByName(const std::string& name);
		bool DeleteResource(const std::filesystem::path& filePath);
		static void AddResource(const std::filesystem::path& filePath, const std::filesystem::path& originalResourcePath, void* data, ResourceType resourceType);
	};
}