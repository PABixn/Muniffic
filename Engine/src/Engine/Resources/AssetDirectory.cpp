#include "egpch.h"
#include "AssetDirectory.h"
#include "AssetDirectoryManager.h"

namespace eg
{
	AssetDirectory::AssetDirectory(UUID uuid, const std::string& name, const UUID& parentDirectory)
		: m_Name(name), m_ParentDirectory(0)
	{
		m_ParentDirectory = parentDirectory;

		AssetDirectoryManager::addAssetDirectory(uuid, this);

		if(parentDirectory != 0)
		{
			AssetDirectoryManager::addSubDirectory(parentDirectory, uuid);
		}
	}
}