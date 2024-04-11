#pragma once

#include "resourceTypes.h"
#include "ResourceSerializer.h"
#include "Engine/Renderer/Font.h"

namespace eg
{
	class ResourceDatabase
	{
	public:
		static Ref<Font> GetFontRuntimeResource(UUID uuid);
		static Ref<Texture2D> GetTextureRuntimeResource(UUID uuid);

		static void* GetRuntimeResource(UUID uuid, ResourceType type);
		static void* AddRuntimeResource(UUID uuid, void* data, ResourceType type);
		static bool FindRuntimeResource(UUID uuid, ResourceType type);
		static void* LoadRuntimeResource(UUID uuid, ResourceType type);

		static std::filesystem::path GetResourcePath(UUID uuid);
		static std::filesystem::path GetResourcePath(std::filesystem::path path);
		static std::filesystem::path GetResourcePath(std::filesystem::path path, ResourceType type);
		static bool RenameResource(UUID uuid, const std::string& name);
		static bool MoveResource(UUID uuid, UUID parentDirectory);
		static bool FindResourceData(UUID uuid);
		static void* GetResourceData(UUID uuid);
		static UUID GetResourceByData(void* data);

		static bool FindResourceData(UUID uuid, ResourceType resourceType);
		static UUID GetResourceByName(const std::string& name, ResourceType type);
		static void SetResourceData(UUID uuid, ResourceType resourceType, void* data);
		static ResourceType GetResourceType(UUID uuid);
		static std::string GetResourceName(UUID uuid);
		static UUID GetResourceParentDirectory(UUID uuid);

		static void RemoveResource(UUID uuid, bool deleteFile = false);
		static void RemoveResource(UUID uuid, ResourceType resourceType, bool deleteFile = false);
		static UUID LoadResource(const std::filesystem::path& filePath);
		static UUID AddResource(const std::filesystem::path& originalResourcePath, void* data, ResourceType resourceType);

		static void SetCurrentDirectoryUUID(UUID directory) { m_CurrentDirectory = directory; }
		static UUID GetCurrentDirectoryUUID() { return m_CurrentDirectory; }

		static std::unordered_map<UUID, TextureResourceData*>& GetTextureResourceDataCache() { return ResourceSerializer::TextureResourceDataCache; }
		static std::unordered_map<UUID, AnimationResourceData*>& GetAnimationResourceDataCache() { return ResourceSerializer::AnimationResourceDataCache; }
		static std::unordered_map<UUID, SubTextureResourceData*>& GetSubTextureResourceDataCache() { return ResourceSerializer::SubTextureResourceDataCache; }
		static std::unordered_map < UUID, SpriteAtlasResourceData*>& GetSpriteAtlasResourceDataCache() { return ResourceSerializer::SpriteAtlasResourceDataCache; }
		static std::unordered_map<UUID, FontResourceData*>& GetFontResourceDataCache() { return ResourceSerializer::FontResourceDataCache; }

	private:
		static UUID m_CurrentDirectory;

		static std::unordered_map<UUID, Ref<Font>> RuntimeFontResourceCache;
		static std::unordered_map<UUID, Ref<Texture2D>> RuntimeTextureResourceCache;
	};
}