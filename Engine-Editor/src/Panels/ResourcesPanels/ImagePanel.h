#pragma once
#include "Engine.h"
#include <filesystem>
#include <functional>


namespace eg {

	class ImagePanel {
	public:
		ImagePanel() = default;
		ImagePanel(const std::filesystem::path& path);

		bool InitImagePanel(const std::filesystem::path& path);

		void OnImGuiRender();
		void ShowImagePanel(bool show) { m_ShowImagePanel = show; }
	private:
		void ResetData();
	private:
		void* m_ResourceData = nullptr;
		Resource* m_LoadedResource = nullptr;
		TextureResourceData m_TextureData;
		std::filesystem::path m_OriginalResourcePath = "";
		Ref<Texture2D> m_PreviewData = nullptr;
		bool m_ShowImagePanel = false;
		std::filesystem::path m_BasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Textures";
	};
}