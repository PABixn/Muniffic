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
		std::filesystem::path FindResourceByKeyPath(const std::filesystem::path& keyPath);

		static UUID GetResourceByKeyPath(const std::filesystem::path& keyPath, ResourceType resourceType);
		static UUID GetResourceByKeyPath(const std::filesystem::path& keyPath);

		static void RemoveResource(UUID uuid, ResourceType resourceType, bool deleteFile);
		static void RenameResource(const std::filesystem::path& oldPath, const std::filesystem::path& newPath);
		static void LoadResource(const std::filesystem::path& filePath);
		static void AddResource(const std::filesystem::path& originalResourcePath, void* data, ResourceType resourceType);
		static void DeleteDirectory(const std::filesystem::path& directory);

		static void SetCurrentPath(std::filesystem::path* directory);
		static std::filesystem::path* GetCurrentPath();
	private:
		static std::filesystem::path* m_CurrentDirectory;
	};
}