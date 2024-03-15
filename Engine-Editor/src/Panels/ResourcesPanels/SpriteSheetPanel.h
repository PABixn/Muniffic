#pragma once

#include "Engine.h"
#include <filesystem>
#include <functional>


namespace eg {

	class SpriteSheetPanel {
	public:
		SpriteSheetPanel() = default;
		SpriteSheetPanel(const std::filesystem::path& path);

		bool InitSpriteSheetPanel(const std::filesystem::path& path);

		void OnImGuiRender();
		void ShowSpriteSheetPanel(bool show) { m_ShowSpriteSheetPanel = show; }
	private:
		void ResetData();
	private:
		int m_SpriteSize[2] = {10,10};
		std::vector<SubTexture2D*> m_Sprites;
		void* m_ResourceData = nullptr;
		Resource* m_LoadedResource = nullptr;
		TextureResourceData m_TextureData;
		std::filesystem::path m_OriginalResourcePath = "";
		Ref<Texture2D> m_PreviewData = nullptr;
		bool m_ShowSpriteSheetPanel = false;
		std::filesystem::path m_BasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Textures";
	};
}