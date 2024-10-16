#include "egpch.h"
#include "ResourceSystem.h"
#include "Engine/Resources/loaders/BinaryLoader.h"
#include "Engine/Resources/loaders/ImageLoader.h"
#include "Engine/Resources/loaders/TextLoader.h"
#include "Engine/Resources/loaders/FontLoader.h"

//Todo: switch to Ref instead of raw pointers
namespace eg {



	static Ref<ResourceSystemState> StatePtr = nullptr;

	bool load(std::string name, ResourceLoader* loader, Resource* outResource)
	{
        EG_PROFILE_FUNCTION();
		if (name.empty() || loader == nullptr || outResource == nullptr)
		{
			EG_CORE_ERROR("Invalid arguments passed to resourceSystemLoad");
			outResource = nullptr;
			return false;
		}

		outResource->LoaderId = loader->Id;
		return loader->load(loader, name, outResource);
	}

	bool resourceSystemInit(ResourceSystemConfig config)
	{
        EG_PROFILE_FUNCTION();
		if (config.MaxLoaderCount == 0)
		{
			EG_CORE_ERROR("Invalid MaxLoaderCount passed to resourceSystemInit");
			return false;
		}

		StatePtr = CreateRef<ResourceSystemState>();
		StatePtr->Config = config;

		StatePtr->RegisteredLoaders.resize(StatePtr->Config.MaxLoaderCount);

		uint32_t count = config.MaxLoaderCount;
		for (uint32_t i = 0; i < count; i++)
		{
			StatePtr->RegisteredLoaders[i].Id = -1;
		}

		resourceSystemRegisterLoader(StatePtr, textResourceLoaderCreate());
		resourceSystemRegisterLoader(StatePtr, binaryResourceLoaderCreate());
		resourceSystemRegisterLoader(StatePtr, imageResourceLoaderCreate());
		resourceSystemRegisterLoader(StatePtr, fontResourceLoaderCreate());
	}

	void resourceSystemShutdown()
	{
        EG_PROFILE_FUNCTION();
		if (StatePtr)
		{
			StatePtr = nullptr;
		}
	}

	bool resourceSystemRegisterLoader(Ref<ResourceSystemState> state, const ResourceLoader loader)
	{
        EG_PROFILE_FUNCTION();
		if (StatePtr) {
			uint32_t count = StatePtr->Config.MaxLoaderCount;
			//Ensure no loader with the given type is already registered
			for (uint32_t i = 0; i < count; i++)
			{
				ResourceLoader* currentLoader = &StatePtr->RegisteredLoaders[i];
				if (currentLoader->Id != -1)
				{
					if (currentLoader->Type == loader.Type)
					{
						EG_ERROR("A loader with the given type is already registered");
						return false;
					}
					else if (!loader.CustomType.empty() && loader.CustomType == currentLoader->CustomType)
					{
							EG_ERROR("A loader with the given custom type is already registered");
							return false;
					}
				}
			}
			for (uint32_t i = 0; i < count; i++)
			{
				if (StatePtr->RegisteredLoaders[i].Id == -1)
				{
					StatePtr->RegisteredLoaders[i] = loader;
					StatePtr->RegisteredLoaders[i].Id = i;
					EG_TRACE("Registered loader with id: {0}", i);
					return true;
				}
			}
		}

		return false;
	}

	bool resourceSystemLoad(std::string name, ResourceType resourceType, Resource* outResource)
	{
        EG_PROFILE_FUNCTION();
		if (StatePtr && resourceType != ResourceType::Custom)
		{
			uint32_t count = StatePtr->Config.MaxLoaderCount;
			for (uint32_t i = 0; i < count; i++)
			{
				ResourceLoader* currentLoader = &StatePtr->RegisteredLoaders[i];
				if (currentLoader->Id != -1 && currentLoader->Type == resourceType)
				{
					return load(name, currentLoader, outResource);
				}
			}
		}
	}

	bool resourceSystemLoadCustom(std::string name, std::string customType, Resource* outResource)
	{
        EG_PROFILE_FUNCTION();
		if (StatePtr && !customType.empty())
		{
			uint32_t count = StatePtr->Config.MaxLoaderCount;
			for (uint32_t i = 0; i < count; i++)
			{
				ResourceLoader* currentLoader = &StatePtr->RegisteredLoaders[i];
				if (currentLoader->Id != -1 && currentLoader->CustomType == customType && currentLoader->Type == ResourceType::Custom)
				{
					return load(name, currentLoader, outResource);
				}
			}
		}
	}

	void resourceSystemUnload(Resource* resource)
	{
        EG_PROFILE_FUNCTION();
		if (StatePtr && resource)
		{
			if (resource->LoaderId != -1)
			{
				ResourceLoader* loader = &StatePtr->RegisteredLoaders[resource->LoaderId];
				if (loader->Id != -1  && loader->unload)
				{
					loader->unload(loader, resource);
				}
			}
		}
	}

	const std::filesystem::path resourceSystemGetResourceDirectory()
	{
        EG_PROFILE_FUNCTION();
		if (StatePtr)
			return StatePtr->Config.ResourceDirectory;

		EG_ERROR("Resource system not initialized");
		return std::filesystem::path();
	}



}
