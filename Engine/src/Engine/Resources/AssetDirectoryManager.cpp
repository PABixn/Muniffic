#include "egpch.h"
#include "AssetDirectoryManager.h"

namespace eg
{
	std::unordered_map<UUID, AssetDirectory*> AssetDirectoryManager::assetDirectories;
	UUID AssetDirectoryManager::rootDirectoryUUID = 0;

	std::string AssetDirectoryManager::getDirectoryName(UUID uuid)
	{
		if (assetDirectories.find(uuid) == assetDirectories.end())
		{
			EG_CORE_WARN("AssetDirectory with UUID {0} not found", uuid);
			return "";
		}

		AssetDirectory* assetDirectory = assetDirectories.at(uuid);
		return assetDirectory->getName();
	}

	std::vector<UUID>& AssetDirectoryManager::getAssets(UUID uuid)
	{
		if (assetDirectories.find(uuid) == assetDirectories.end())
		{
			EG_CORE_WARN("AssetDirectory with UUID {0} not found", uuid);
			std::vector<UUID> empty;
			return empty;
		}

		AssetDirectory* assetDirectory = assetDirectories.at(uuid);
		return assetDirectory->getAssets();
	}

	std::vector<UUID>& AssetDirectoryManager::getSubdirectories(UUID uuid)
	{
		if (assetDirectories.find(uuid) == assetDirectories.end())
		{
			EG_CORE_WARN("AssetDirectory with UUID {0} not found", uuid);
			std::vector<UUID> empty;
			return empty;
		}

		AssetDirectory* assetDirectory = assetDirectories.at(uuid);
		return assetDirectory->getSubdirectories();
	}

	UUID AssetDirectoryManager::getParentDirectoryUUID(UUID uuid)
	{
		if (assetDirectories.find(uuid) == assetDirectories.end())
		{
			EG_CORE_WARN("AssetDirectory with UUID {0} not found", uuid);
			return 0;
		}

		AssetDirectory* assetDirectory = assetDirectories.at(uuid);
		return assetDirectory->getParentDirectory();
	}

	void AssetDirectoryManager::initDefault()
	{
		UUID rootUUID = UUID();
		AssetDirectory* root = new AssetDirectory("Assets");
		AssetDirectoryManager::addAssetDirectory(rootUUID, root);
		rootDirectoryUUID = rootUUID;

		AssetDirectory* textures = new AssetDirectory("Textures", rootUUID);
		AssetDirectoryManager::addAssetDirectory(UUID(), textures);

		AssetDirectory* subtextures = new AssetDirectory("SubTextures", rootUUID);
		AssetDirectoryManager::addAssetDirectory(UUID(), subtextures);

		AssetDirectory* animations = new AssetDirectory("Animations", rootUUID);
		AssetDirectoryManager::addAssetDirectory(UUID(), animations);

		AssetDirectory* fonts = new AssetDirectory("Fonts", rootUUID);
		AssetDirectoryManager::addAssetDirectory(UUID(), fonts);

		AssetDirectory* spriteAtlas = new AssetDirectory("SpriteAtlas", rootUUID);
		AssetDirectoryManager::addAssetDirectory(UUID(), spriteAtlas);

		AssetDirectory* shaders = new AssetDirectory("Shaders", rootUUID);
		AssetDirectoryManager::addAssetDirectory(UUID(), shaders);
	}

	bool AssetDirectoryManager::moveAssetDirectory(UUID assetDirectoryUUID, UUID newParentUUID)
	{
		if (assetDirectories.find(assetDirectoryUUID) == assetDirectories.end())
		{
			EG_CORE_WARN("AssetDirectory with UUID {0} not found", assetDirectoryUUID);
			return false;
		}

		if (assetDirectories.find(newParentUUID) == assetDirectories.end())
		{
			EG_CORE_WARN("Parent AssetDirectory with UUID {0} not found", newParentUUID);
			return false;
		}

		AssetDirectory* assetDirectory = assetDirectories.at(assetDirectoryUUID);
		AssetDirectory* newParent = assetDirectories.at(newParentUUID);

		if (assetDirectory->getParentDirectory() == newParentUUID)
		{
			EG_CORE_WARN("AssetDirectory with UUID {0} is already a child of AssetDirectory with UUID {1}", assetDirectoryUUID, newParentUUID);
			return false;
		}

		if (assetDirectory->getParentDirectory() != 0)
			AssetDirectoryManager::removeSubDirectory(assetDirectory->getParentDirectory(), assetDirectoryUUID);

		AssetDirectoryManager::addSubDirectory(newParentUUID, assetDirectoryUUID);
		assetDirectory->setParentDirectory(newParentUUID);

		return true;
	}

	bool AssetDirectoryManager::moveAsset(UUID assetUUID, UUID newAssetDirectoryUUID)
	{
		ResourceData* data = (ResourceData*)ResourceDatabase::GetResourceData(assetUUID);
		return moveAsset(assetUUID, data->ParentDirectory, newAssetDirectoryUUID);
	}

	bool AssetDirectoryManager::moveAsset(UUID assetUUID, UUID oldAssetDirectoryUUID, UUID newAssetDirectoryUUID)
	{
		if (assetDirectories.find(newAssetDirectoryUUID) == assetDirectories.end())
		{
			EG_CORE_WARN("AssetDirectory with UUID {0} not found", newAssetDirectoryUUID);
			return false;
		}

		AssetDirectory* newAssetDirectory = assetDirectories.at(newAssetDirectoryUUID);
		AssetDirectory* oldAssetDirectory = assetDirectories.at(oldAssetDirectoryUUID);

		std::vector<UUID>& assets = newAssetDirectory->getAssets();

		if (std::find(assets.begin(), assets.end(), assetUUID) != assets.end())
		{
			EG_CORE_WARN("Asset with UUID {0} already exists in AssetDirectory with UUID {1}", assetUUID, newAssetDirectoryUUID);
			return false;
		}

		AssetDirectoryManager::removeAsset(oldAssetDirectoryUUID, assetUUID);
		AssetDirectoryManager::addAsset(newAssetDirectoryUUID, assetUUID);

		return true;
	}

	bool AssetDirectoryManager::changeAssetDirectoryName(UUID uuid, const std::string& newName)
	{
		if (assetDirectories.find(uuid) == assetDirectories.end())
		{
			EG_CORE_WARN("AssetDirectory with UUID {0} not found", uuid);
			return false;
		}

		AssetDirectory* assetDirectory = assetDirectories.at(uuid);
		assetDirectory->setName(newName);

		std::filesystem::path path = AssetDirectoryManager::getDirectoryPath(uuid);
		std::filesystem::path newPath = path.parent_path() / newName;
		std::filesystem::rename(path, newPath);

		return true;
	}

	bool AssetDirectoryManager::addAsset(UUID assetDirectoryUUID, UUID assetUUID)
	{
		if (assetDirectories.find(assetDirectoryUUID) == assetDirectories.end())
		{
			EG_CORE_WARN("AssetDirectory with UUID {0} not found", assetDirectoryUUID);
			return false;
		}

		AssetDirectory* assetDirectory = assetDirectories.at(assetDirectoryUUID);

		std::vector<UUID>& assets = assetDirectory->getAssets();

		if(std::find(assets.begin(), assets.end(), assetUUID) != assets.end())
		{
			EG_CORE_WARN("Asset with UUID {0} already exists in AssetDirectory with UUID {1}", assetUUID, assetDirectoryUUID);
			return false;
		}

		assetDirectory->addAsset(assetUUID);
	}

	bool AssetDirectoryManager::removeAsset(UUID assetDirectoryUUID, UUID assetUUID)
	{
		if (assetDirectories.find(assetDirectoryUUID) == assetDirectories.end())
		{
			EG_CORE_WARN("AssetDirectory with UUID {0} not found", assetDirectoryUUID);
			return false;
		}

		AssetDirectory* assetDirectory = assetDirectories.at(assetDirectoryUUID);

		std::vector<UUID>& assets = assetDirectory->getAssets();

		auto it = std::find(assets.begin(), assets.end(), assetUUID);

		if (it == assets.end())
		{
			EG_CORE_WARN("Asset with UUID {0} not found in AssetDirectory with UUID {1}", assetUUID, assetDirectoryUUID);
			return false;
		}

		assets.erase(it);
		ResourceDatabase::RemoveResource(assetUUID);

		return true;
	}

	void AssetDirectoryManager::removeAssetDirectory(UUID uuid)
	{
		if (assetDirectories.find(uuid) == assetDirectories.end())
		{
			EG_CORE_WARN("AssetDirectory with UUID {0} not found", uuid);
			return;
		}

		AssetDirectory* assetDirectory = assetDirectories.at(uuid);

		if (assetDirectory->getParentDirectory() != 0)
			AssetDirectoryManager::removeSubDirectory(assetDirectory->getParentDirectory(), uuid);

		std::vector<UUID>& subDirectories = assetDirectory->getSubdirectories();

		for (UUID subDirectoryUUID : subDirectories)
		{
			delete assetDirectories.at(subDirectoryUUID);
			assetDirectories.erase(subDirectoryUUID);
		}

		std::vector<UUID>& assets = assetDirectory->getAssets();

		for(UUID subDirectoryUUID : assets)
			ResourceDatabase::RemoveResource(subDirectoryUUID);

		std::filesystem::path path = AssetDirectoryManager::getDirectoryPath(uuid);
		std::filesystem::remove_all(path);

		assetDirectories.erase(uuid);
		delete assetDirectory;
	}

	void AssetDirectoryManager::addAssetDirectory(UUID uuid, AssetDirectory* assetDirectory)
	{
		if (assetDirectories.find(uuid) == assetDirectories.end())
			assetDirectories[uuid] = assetDirectory;
		else
			EG_CORE_WARN("AssetDirectory with UUID {0} already exists", uuid);

		if(assetDirectory->getParentDirectory() != 0)
			AssetDirectoryManager::addSubDirectory(assetDirectory->getParentDirectory(), uuid);

		if(rootDirectoryUUID == 0)
			rootDirectoryUUID = uuid;

		validateAssetDirectory(uuid);
	}

	bool AssetDirectoryManager::removeSubDirectory(UUID parentUUID, UUID subdirectoryUUID)
	{
		if (assetDirectories.find(parentUUID) == assetDirectories.end())
		{
			EG_CORE_WARN("Parent AssetDirectory with UUID {0} not found", parentUUID);
			return false;
		}

		if (assetDirectories.find(subdirectoryUUID) == assetDirectories.end())
		{
			EG_CORE_WARN("Subdirectory AssetDirectory with UUID {0} not found", subdirectoryUUID);
			return false;
		}

		AssetDirectory* parentDirectory = assetDirectories.at(parentUUID);
		AssetDirectory* subDirectory = assetDirectories.at(subdirectoryUUID);

		std::vector<UUID>& subDirectories = parentDirectory->getSubdirectories();

		auto it = std::find(subDirectories.begin(), subDirectories.end(), subdirectoryUUID);

		if (it == subDirectories.end())
		{
			EG_CORE_WARN("Parent AssetDirectory with UUID {0} does not have subdirectory with UUID {1} as a subdirectory", parentUUID, subdirectoryUUID);
			return false;
		}

		subDirectories.erase(it);
		subDirectory->setParentDirectory(0);

		return true;
	}

	bool AssetDirectoryManager::addSubDirectory(UUID parentUUID, UUID subdirectoryUUID)
	{
		if (assetDirectories.find(parentUUID) == assetDirectories.end())
		{
			EG_CORE_WARN("Parent AssetDirectory with UUID {0} not found", parentUUID);
			return false;
		}

		if (assetDirectories.find(subdirectoryUUID) == assetDirectories.end())
		{
			EG_CORE_WARN("Subdirectory AssetDirectory with UUID {0} not found", subdirectoryUUID);
			return false;
		}

		AssetDirectory* parentDirectory = assetDirectories.at(parentUUID);
		AssetDirectory* subDirectory = assetDirectories.at(subdirectoryUUID);

		if (parentDirectory->getParentDirectory() == subdirectoryUUID)
		{
			EG_CORE_WARN("Parent AssetDirectory with UUID {0} is the same as subdirectory with UUID {1}", parentUUID, subdirectoryUUID);
			return false;
		}

		if (subDirectory->getParentDirectory() != 0)
		{
			EG_CORE_WARN("Subdirectory AssetDirectory with UUID {0} already has a parent", subdirectoryUUID);
			return false;
		}

		std::vector<UUID>& subDirectories = parentDirectory->getSubdirectories();

		if(parentDirectory->getParentDirectory() == parentUUID)
		{
			EG_CORE_WARN("Parent AssetDirectory with UUID {0} already has subdirectory with UUID {1} as a subdirectory", parentUUID, subdirectoryUUID);
			return false;
		}

		subDirectories.push_back(subdirectoryUUID);
		subDirectory->setParentDirectory(parentUUID);

		return true;
	}

	bool AssetDirectoryManager::findAssetDirectory(UUID uuid)
	{
		return assetDirectories.find(uuid) != assetDirectories.end();
	}

	bool AssetDirectoryManager::validateAssetDirectory(UUID uuid)
	{
		if (assetDirectories.find(uuid) == assetDirectories.end())
			return false;
		
		std::filesystem::path path = AssetDirectoryManager::getDirectoryPath(uuid);

		if (!std::filesystem::exists(path))
			std::filesystem::create_directories(path);

		return true;
	}

	AssetDirectory* AssetDirectoryManager::getAssetDirectory(UUID uuid)
	{
		if(assetDirectories.find(uuid) != assetDirectories.end())
			return assetDirectories.at(uuid);
		else
		{
			EG_CORE_WARN("AssetDirectory with UUID {0} not found", uuid);
			return nullptr;
		}
	}

	std::filesystem::path AssetDirectoryManager::getDirectoryPath(UUID uuid)
	{
		if (assetDirectories.find(uuid) == assetDirectories.end())
			return std::filesystem::path();

		AssetDirectory* assetDirectory = assetDirectories.at(uuid);

		if(assetDirectory->getParentDirectory() != 0)
			return assetDirectory->getName() / getDirectoryPath(assetDirectory->getParentDirectory());
		else
			return assetDirectory->getName();
	}
}