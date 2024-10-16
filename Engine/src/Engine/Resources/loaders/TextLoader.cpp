#include "egpch.h"
#include "TextLoader.h"
#include "Engine/Resources/resourceTypes.h"
#include <filesystem>


namespace eg
{

	bool textLoaderLoad(ResourceLoader* loader, std::string name, Resource* outResource)
	{
        EG_PROFILE_FUNCTION();
		if (!loader || name.empty() || !outResource)
		{
			EG_CORE_ERROR("textLoaderLoad called with nullptr");
			return false;
		}

		std::filesystem::path path = loader->TypePath / name;
		std::ifstream file(path, std::ios::binary | std::ios::ate);

		if (!file.is_open())
		{
			EG_CORE_ERROR("Failed to open file: {0}", path.string());
			return false;
		}

		size_t size = file.tellg();
		std::ifstream t(path);
		std::stringstream buffer;
		buffer << t.rdbuf();

		std::string* resourceData = new std::string(buffer.str());

		outResource->Data = resourceData;
		outResource->DataSize = size;
		outResource->Name = name;

		file.close();

		return true;
	}

	void textLoaderUnload(ResourceLoader* loader, Resource* resource)
	{
        EG_PROFILE_FUNCTION();
		if (!loader || !resource)
		{
			EG_CORE_WARN("textLoaderUnload called with nullptr");
			return;
		}

		if (resource->Data)
		{
			resource->Path.clear();
			resource->Data = nullptr;
			resource->DataSize = 0;
			resource->LoaderId = -1;
		}
	}

	ResourceLoader textResourceLoaderCreate()
	{
        EG_PROFILE_FUNCTION();
		ResourceLoader loader;
		loader.Type = ResourceType::Text;
		loader.load = textLoaderLoad;
		loader.unload = textLoaderUnload;
		loader.TypePath = "";

		return loader;
	}

}
