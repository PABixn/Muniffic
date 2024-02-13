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
		out << YAML::Key << "Path" << YAML::Value << data.ResourcePath.string();
		out << YAML::Key << "Name" << YAML::Value << data.ImageName;
		out << YAML::Key << "Width" << YAML::Value << data.Width;
		out << YAML::Key << "Height" << YAML::Value << data.Height;
		out << YAML::Key << "OriginalWidth" << YAML::Value << data.OriginalWidth;
		out << YAML::Key << "OriginalHeight" << YAML::Value << data.OriginalHeight;
		out << YAML::Key << "Top" << YAML::Value << data.Top;
		out << YAML::Key << "Bottom" << YAML::Value << data.Bottom;
		out << YAML::Key << "Left" << YAML::Value << data.Left;
		out << YAML::Key << "Right" << YAML::Value << data.Right;
		out << YAML::Key << "Channels" << YAML::Value << data.Channels;
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
		Resourcedata->ResourcePath = textureResource["Path"].as<std::string>();
		Resourcedata->ImageName = textureResource["Name"].as<std::string>();
		Resourcedata->Width = textureResource["Width"].as<int>();
		Resourcedata->Height = textureResource["Height"].as<int>();
		Resourcedata->OriginalWidth = textureResource["OriginalWidth"].as<int>();
		Resourcedata->OriginalHeight = textureResource["OriginalHeight"].as<int>();
		Resourcedata->Top = textureResource["Top"].as<int>();
		Resourcedata->Bottom = textureResource["Bottom"].as<int>();
		Resourcedata->Left = textureResource["Left"].as<int>();
		Resourcedata->Right = textureResource["Right"].as<int>();
		Resourcedata->Channels = textureResource["Channels"].as<int>();
		return true;
	}
}