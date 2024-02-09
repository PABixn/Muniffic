#include "egpch.h"
#include "ResourceDatabase.h"

namespace eg
{
	std::vector<std::filesystem::path> ResourceDatabase::GetResourcesInDirectory(const std::filesystem::path& directory)
	{
		for (auto& directoryEntry : std::filesystem::directory_iterator(directory))
		{
			const std::filesystem::path path = directoryEntry.path();

			if (path.extension().string() == ".mnmeta")
			{
				
			}
		}
	}
	std::vector<std::filesystem::path> ResourceDatabase::FindResourcesByName(const std::string& name)
	{
		return std::vector<std::filesystem::path>();
	}
}