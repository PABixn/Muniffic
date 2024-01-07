#include "egpch.h"
#include <filesystem>
#include "BinaryLoader.h"
#include "Engine/Resources/resourceTypes.h"

namespace eg
{
	bool binaryLoaderLoad(ResourceLoader* loader, std::string name, Resource* resource)
	{
		if (loader == nullptr || resource == nullptr)
		{
			EG_CORE_ERROR("Invalid arguments passed to binaryLoaderLoad");
			return false;
		}

		std::filesystem::path path = resourceSystemGetResourceDirectory() / name;
		std::ifstream file(path, std::ios::binary | std::ios::ate);

		if (!file.is_open())
		{
			EG_CORE_ERROR("Failed to open file: {0}", path.string());
			return false;
		}

		size_t size = file.tellg();
		file.seekg(0, std::ios::beg);

		resource->Data = malloc(size);
		file.read((char*)resource->Data, size);
		resource->Size = size;

		file.close();

		return true;
	}

	ResourceLoader binaryResourceLoaderCreate()
	{
		return ResourceLoader();
	}
}
