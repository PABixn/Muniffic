#include "egpch.h"
#include "ImageLoader.h"
#include <filesystem>
#include <stb_image.h>
#include <cstdio>

namespace eg
{
	bool imageLoaderLoad(ResourceLoader *loader, std::string name, Resource *outResource)
	{
        EG_PROFILE_FUNCTION();
		if (!loader || name.empty() || !outResource)
		{
			return false;
		}

		std::filesystem::path fullPath = name;
		/*
		EG_CORE_TRACE("Root directory: {}", fullPath.root_directory().string().c_str());
		EG_CORE_TRACE("Parent path: {}", fullPath.parent_path().string().c_str());
		*/
		int32_t width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;

		data = stbi_load(fullPath.string().c_str(), &width, &height, &channels, 0);

		if (!data)
		{
			const char *failReason = stbi_failure_reason();
			if (failReason)
			{
				EG_ERROR("Image resource loader failed to load file {}, {}", fullPath.string().c_str(), failReason);

				return false;
			}
			EG_ERROR("Image resource loader failed to load file {}", fullPath);
			return false;
		}

		ImageResourceData* imageResourceData = new ImageResourceData;

		imageResourceData->pixels = data;
		imageResourceData->width = width;
		imageResourceData->height = height;
		imageResourceData->channelCount = channels;

		outResource->Data = imageResourceData;
		outResource->DataSize = sizeof(imageResourceData);
		outResource->Name = fullPath.filename().string();
		outResource->Path = fullPath.parent_path().string();

		return true;
	}

	void imageLoaderUnload(ResourceLoader *loader, Resource *resource)
	{
        EG_PROFILE_FUNCTION();
		if (!loader || !resource)
		{
			EG_ERROR("imageLoaderUnload called with nullptr");
			return;
		}

		resource->Path.clear();

		if (resource->Data)
		{
			delete resource->Data;
			resource->Data = 0;
			resource->DataSize = 0;
			resource->LoaderId = 0;
		}
	}

	ResourceLoader imageResourceLoaderCreate()
	{
        EG_PROFILE_FUNCTION();
		ResourceLoader loader;
		loader.Type = ResourceType::Image;
		loader.load = imageLoaderLoad;
		loader.unload = imageLoaderUnload;
		loader.TypePath = "Textures";

		return loader;
	}
}
