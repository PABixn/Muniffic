#pragma once

#include "resourceTypes.h"
#include "ResourceSerializer.h"
#include "Engine/Renderer/Font.h"

namespace eg
{
	class ResourceDatabase
	{
	public:
		static void* GetRuntimeResource(UUID uuid, ResourceType type);
		static void* AddRuntimeResource(UUID uuid, void* data, ResourceType type);
		static bool FindRuntimeResource(UUID uuid, ResourceType type);
		static void* LoadRuntimeResource(UUID uuid, ResourceType type);

		static void* GetResourceData(UUID uuid, ResourceType resourceType);
		static bool FindResourceData(UUID uuid, ResourceType resourceType);
		static UUID GetResourceByName(const std::string& name, ResourceType type);
		static void SetResourceData(UUID uuid, ResourceType resourceType, void* data);
		static ResourceType GetResourceType(UUID uuid);
		static UUID FindResourceByKeyPath(const std::filesystem::path& keyPath, ResourceType type);
		static std::filesystem::path GetResourcePath(UUID uuid);
		static std::filesystem::path GetFullPath(UUID uuid);
		static std::string GetResourceName(UUID uuid);
		static std::string GetResourceTypeExtension(ResourceType type);
		static UUID GetResourceByKeyPathWithoutCategory(const std::filesystem::path& keyPath, ResourceType resourceType);
		static UUID GetResourceByPath(const std::filesystem::path& keyPath);
		static UUID GetResourceByData(void* data, ResourceType resourceType);

		static void RemoveResource(std::filesystem::path path, bool deleteFile = false);
		static void RemoveResource(UUID uuid, bool deleteFile = false);
		static void RemoveResource(UUID uuid, ResourceType resourceType, bool deleteFile = false);
		static void RenameDirectory(const std::filesystem::path& oldPath, const std::string& name);
		static void RenameResource(UUID uuid, const std::string& name);
		static void LoadResource(const std::filesystem::path& filePath);
		static UUID AddResource(const std::filesystem::path& originalResourcePath, void* data, ResourceType resourceType);
		static void DeleteDirectory(const std::filesystem::path& directory);
		static void MoveResource(UUID uuid, const std::filesystem::path& path);

		static void SetCurrentPath(std::filesystem::path* directory) { m_CurrentDirectory = directory; }
		static std::filesystem::path* GetCurrentPath() { return m_CurrentDirectory; }

		static std::unordered_map<UUID, TextureResourceData*>& GetTextureResourceDataCache() { return ResourceSerializer::TextureResourceDataCache; }
		static std::unordered_map<UUID, AnimationResourceData*>& GetAnimationResourceDataCache() { return ResourceSerializer::AnimationResourceDataCache; }
		static std::unordered_map<UUID, SubTextureResourceData*>& GetSubTextureResourceDataCache() { return ResourceSerializer::SubTextureResourceDataCache; }
		static std::unordered_map < UUID, SpriteAtlasResourceData*>& GetSpriteAtlasResourceDataCache() { return ResourceSerializer::SpriteAtlasResourceDataCache; }
		static std::unordered_map<UUID, FontResourceData*>& GetFontResourceDataCache() { return ResourceSerializer::FontResourceDataCache; }

	private:
		static std::filesystem::path* m_CurrentDirectory;

		static std::unordered_map<UUID, Ref<Font>> RuntimeFontResourceCache;
	};
}