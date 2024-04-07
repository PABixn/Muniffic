#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include "Engine/Core/UUID.h"
#include <unordered_map>
#include "AssetDirectory.h"
#include <Engine/Core/Log.h>
#include "ResourceDatabase.h"

namespace eg
{
	class AssetDirectoryManager
	{
	public:
		static std::filesystem::path getDirectoryPath(UUID uuid);
		static std::filesystem::path getDirectoryPath(UUID uuid, std::filesystem::path& path);
		static AssetDirectory* getAssetDirectory(UUID uuid);
		static bool validateAssetDirectory(UUID uuid);
		static void addAssetDirectory(UUID uuid, AssetDirectory* assetDirectory);
		static void removeAssetDirectory(UUID uuid);
		static bool findAssetDirectory(UUID uuid);
		static bool addSubDirectory(UUID parentUUID, UUID subdirectoryUUID);
		static bool removeSubDirectory(UUID parentUUID, UUID subdirectoryUUID);
		static bool addAsset(UUID directoryUUID, UUID assetUUID);
		static bool removeAsset(UUID directoryUUID, UUID assetUUID);
		static bool changeAssetDirectoryName(UUID uuid, const std::string& newName);
		static bool moveAsset(UUID assetUUID, UUID oldAssetDirectoryUUID, UUID newAssetDirectoryUUID);
		static bool moveAsset(UUID assetUUID, UUID newAssetDirectoryUUID);
		static bool moveAssetDirectory(UUID assetDirectoryUUID, UUID newParentUUID);
		static UUID GetRootAssetTypeDirectory(ResourceType type);
		static void initDefault(UUID rootUUID);

		static std::string getDirectoryName(UUID uuid);
		static UUID getParentDirectoryUUID(UUID uuid);
		static std::vector<UUID>& getSubdirectories(UUID uuid);
		static std::vector<UUID>& getAssets(UUID uuid);

		static std::unordered_map<UUID, AssetDirectory*>& getAssetDirectories() { return assetDirectories; }

		static UUID getRootDirectoryUUID() { return rootDirectoryUUID; }
		static void setRootDirectoryUUID(UUID uuid) { rootDirectoryUUID = uuid; }

	private:
		static std::unordered_map<UUID, AssetDirectory*> assetDirectories;
		static UUID rootDirectoryUUID;
	};
}
