#pragma once

namespace eg
{
	class AssetDirectorySerializer
	{
	public:
		static void SerializeAssetDirectoryCache();
		static bool DeserializeAssetDirectoryCache();
	};
}
