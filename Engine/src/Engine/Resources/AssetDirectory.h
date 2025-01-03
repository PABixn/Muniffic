#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include "Engine/Core/UUID.h"

namespace eg
{
	class AssetDirectory
	{
	public:
		AssetDirectory() = default;
		AssetDirectory(UUID uuid, const std::string& name, const UUID& parentDirectory = 0);

		std::string& getName() { return m_Name; }
		std::vector<UUID>& getSubdirectories() { return m_Subdirectories; }
		std::vector<UUID>& getAssets() { return m_Assets; }
		UUID& getParentDirectory() { return m_ParentDirectory; }

		void setName(const std::string& name) { m_Name = name; }
		void setParentDirectory(const UUID& parent) { m_ParentDirectory = parent; }

		void addAsset(const UUID& asset) { m_Assets.push_back(asset); }
		void addSubdirectory(const UUID& subdirectory) { m_Subdirectories.push_back(subdirectory); }

		void removeAsset(const UUID& asset) { m_Assets.erase(std::find(m_Assets.begin(), m_Assets.end(), asset)); }

		void removeSubdirectory(const UUID& subdirectory) { m_Subdirectories.erase(std::find(m_Subdirectories.begin(), m_Subdirectories.end(), subdirectory)); }

	private:
		std::string m_Name;
		std::vector<UUID> m_Subdirectories;
		std::vector<UUID> m_Assets;
		UUID m_ParentDirectory;
	};
}