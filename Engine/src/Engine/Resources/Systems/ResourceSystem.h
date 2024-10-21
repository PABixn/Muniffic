#pragma once

#include "Engine/Resources/resourceTypes.h"

#include <cstdint>
#include <filesystem>


namespace eg
{

	

	struct ResourceSystemConfig
	{
		uint32_t MaxLoaderCount = 0;
		std::filesystem::path ResourceDirectory;
	};

	struct ResourceLoader {
		uint32_t Id;
		ResourceType Type;
		std::string CustomType;
		std::filesystem::path TypePath;
		bool (*load)(ResourceLoader* loader, std::string name, Resource* resource);
		void (*unload)(ResourceLoader* loader, Resource* resource);
	};

	struct ResourceSystemState
	{
		ResourceSystemConfig Config;
		std::vector<ResourceLoader> RegisteredLoaders;
	};

	

	bool resourceSystemInit(ResourceSystemConfig config);
	void resourceSystemShutdown();

	bool resourceSystemRegisterLoader(Ref<ResourceSystemState> state, const ResourceLoader loader);

	bool resourceSystemLoad(std::string name, ResourceType resourceType, Resource* outResource);
	bool resourceSystemLoadCustom(std::string name, std::string customType, Resource* outResource);

	void resourceSystemUnload(Resource* resource);

	const std::filesystem::path resourceSystemGetResourceDirectory();
}