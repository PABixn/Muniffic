#pragma once

#include "Engine/Resources/resourceTypes.h"

#include <cstdint>
#include <filesystem>


namespace eg
{

	struct ResourceSystemState;
	struct ResourceSystemConfig
	{
		uint32_t MaxLoaderCount;
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

	bool resourceSystemInit(uint64_t* memory_requirement, ResourceSystemState* state, ResourceSystemConfig config);
	void resourceSystemShutdown(void* state);

	bool resourceSystemRegisterLoader(void* state, ResourceLoader* loader);

	bool resourceSystemLoad(std::string name, ResourceType resourceType, Resource* outResource);
	bool resourceSystemLoadCustom(std::string name, std::string customType, Resource* outResource);

	void resourceSystemUnload(Resource* resource);

	const std::filesystem::path resourceSystemGetResourceDirectory();
}