#include "egpch.h"
#include "Engine/Core/Core.h"
#include "ResourceSerializer.h"
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <optional>
#include <iostream>

namespace eg
{
	void ResourceSerializer::SerializeTextureResource(const std::filesystem::path& filepath, const TextureResourceData& data)
	{
		YAML::Emitter out;
		out << YAML::Key << "TextureResource";
		out << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "Path" << YAML::Value << data.imagePath.string();
		out << YAML::Key << "Name" << YAML::Value << data.imageName;
		out << YAML::Key << "Width" << YAML::Value << data.width;
		out << YAML::Key << "Height" << YAML::Value << data.height;
		out << YAML::Key << "OriginalWidth" << YAML::Value << data.originalWidth;
		out << YAML::Key << "OriginalHeight" << YAML::Value << data.originalHeight;
		out << YAML::Key << "Top" << YAML::Value << data.top;
		out << YAML::Key << "Bottom" << YAML::Value << data.bottom;
		out << YAML::Key << "Left" << YAML::Value << data.left;
		out << YAML::Key << "Right" << YAML::Value << data.right;
		out << YAML::Key << "Channels" << YAML::Value << data.channels;
		out << YAML::EndMap;
		
		std::ofstream fout(filepath);
		fout << out.c_str();
		fout.close();
	}

	bool ResourceSerializer::DeserializeTextureResource(const std::string& path, TextureResourceData* Resourcedata)
	{
		if(!Resourcedata)
			return false;

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path);
		}
		catch (YAML::ParserException e)
		{
			EG_CORE_ERROR("Failed to load .hazel file '{0}'\n     {1}", path, e.what());
			return false;
		}
		if (!data["TextureResource"])
			return false;

		auto textureResource = data["TextureResource"];
		Resourcedata->imagePath = textureResource["Path"].as<std::string>();
		Resourcedata->imageName = textureResource["Name"].as<std::string>();
		Resourcedata->width = textureResource["Width"].as<int>();
		Resourcedata->height = textureResource["Height"].as<int>();
		Resourcedata->originalWidth = textureResource["OriginalWidth"].as<int>();
		Resourcedata->originalHeight = textureResource["OriginalHeight"].as<int>();
		Resourcedata->top = textureResource["Top"].as<int>();
		Resourcedata->bottom = textureResource["Bottom"].as<int>();
		Resourcedata->left = textureResource["Left"].as<int>();
		Resourcedata->right = textureResource["Right"].as<int>();
		Resourcedata->channels = textureResource["Channels"].as<int>();
		return true;
	}
}