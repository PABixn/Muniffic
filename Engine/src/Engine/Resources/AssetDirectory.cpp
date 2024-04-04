#include "egpch.h"
#include "AssetDirectory.h"
#include "AssetDirectoryManager.h"

namespace eg
{
	AssetDirectory::AssetDirectory(const std::string& name, const UUID& parentDirectory)
		: m_Name(name), m_ParentDirectory(parentDirectory)
	{

	}
}