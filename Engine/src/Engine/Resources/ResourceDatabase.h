#pragma once
#include <filesystem>
#include <string>
#include <vector>
namespace eg {
	class ResourceDatabase
	{
	public:
		std::vector<std::filesystem::path> GetResourcesInDirectory(const std::filesystem::path& directory);
		std::vector<std::filesystem::path> FindResourcesByName(const std::string& name);

	};
}