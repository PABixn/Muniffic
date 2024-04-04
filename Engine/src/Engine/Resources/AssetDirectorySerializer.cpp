#include "egpch.h"
#include "AssetDirectorySerializer.h"
#include "ResourceUtils.h"
#include "Engine/Utils/YAMLConversion.h"
#include "AssetDirectoryManager.h"

namespace eg
{
	bool AssetDirectorySerializer::DeserializeAssetDirectoryCache()
	{
		std::filesystem::path metadata = ResourceUtils::GetAssetDirectoryMetadataPath();

		YAML::Node node;

		if (!std::filesystem::exists(metadata))
			return false;

		try
		{
			node = YAML::LoadFile(metadata.string());
		}
		catch (YAML::ParserException e)
		{
			EG_CORE_ERROR("Failed to parse Asset Directory Cache: {0}", e.what());
			return false;
		}

		auto directories = node["Directories"];

		if (directories)
		{
			for (auto directory : directories)
			{
				AssetDirectory* data = new AssetDirectory();
				data->setName(directory["Name"].as<std::string>());
				data->setParentDirectory(directory["Parent"].as<UUID>());
				auto subdirectories = directory["Subdirectories"];
				if (subdirectories)
				{
					for (auto subdirectory : subdirectories)
					{
						data->addSubdirectory(subdirectory.as<UUID>());
					}
				}
				auto assets = directory["Assets"];
				if (assets)
				{
					for (auto asset : assets)
					{
						data->addAsset(asset.as<UUID>());
					}
				}

				AssetDirectoryManager::addAssetDirectory(directory["UUID"].as<UUID>(), data);
			}
		}
		else
			AssetDirectoryManager::initDefault();
	}

	void AssetDirectorySerializer::SerializeAssetDirectoryCache()
	{
		EG_CORE_TRACE("Serializing Asset Directory Cache");

		std::filesystem::path metadataPath = ResourceUtils::GetAssetDirectoryMetadataPath();

		YAML::Emitter out;

		if(!std::filesystem::exists(metadataPath.parent_path()))
			std::filesystem::create_directories(metadataPath.parent_path());

		out << YAML::BeginMap;
		out << YAML::Key << "Directories" << YAML::Value << YAML::BeginSeq;

		for (auto& [key, value] : AssetDirectoryManager::getAssetDirectories())
		{
			out << YAML::BeginMap;
			out << YAML::Key << "UUID" << YAML::Value << key;
			out << YAML::Key << "Name" << YAML::Value << value->getName();
			out << YAML::Key << "Parent" << YAML::Value << value->getParentDirectory();
			out << YAML::Key << "Subdirectories" << YAML::Value << YAML::BeginSeq;
			for (auto& subdirectory : value->getSubdirectories())
			{
				out << subdirectory;
			}
			out << YAML::EndSeq;
			out << YAML::Key << "Assets" << YAML::Value << YAML::BeginSeq;
			for (auto& asset : value->getAssets())
			{
				out << asset;
			}
			out << YAML::EndSeq;
			out << YAML::EndMap;
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(metadataPath);
		
		fout << out.c_str();

		fout.close();
	}
}