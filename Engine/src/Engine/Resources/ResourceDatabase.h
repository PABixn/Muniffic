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
		static UUID FindResourceByKeyPath(const std::filesystem::path& keyPath, ResourceType type);
		static std::filesystem::path GetResourcePath(UUID uuid);
		static std::filesystem::path GetFullPath(UUID uuid);
		static std::string GetResourceName(UUID uuid);

		static UUID GetResourceByKeyPath(const std::filesystem::path& keyPath, ResourceType resourceType);
		static UUID GetResourceByPath(const std::filesystem::path& keyPath);

		static void RemoveResource(std::filesystem::path path, bool deleteFile = false);
		static void RemoveResource(UUID uuid, bool deleteFile = false);
		static void RemoveResource(UUID uuid, ResourceType resourceType, bool deleteFile = false);
		static void RenameDirectory(const std::filesystem::path& oldPath, const std::string& name);
		static void RenameResource(UUID uuid, const std::string& name);
		static void LoadResource(const std::filesystem::path& filePath);
		static UUID AddResource(const std::filesystem::path& originalResourcePath, void* data, ResourceType resourceType);
		static void DeleteDirectory(const std::filesystem::path& directory);
		static void MoveResource(UUID uuid, const std::filesystem::path& path);

		static void SetCurrentPath(std::filesystem::path* directory);
		static std::filesystem::path* GetCurrentPath();
	private:
		static std::filesystem::path* m_CurrentDirectory;
	};
}