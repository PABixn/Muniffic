#pragma once
#include "Engine.h"
#include <filesystem>

namespace eg
{
	class AnimationPanel {
	public:
		AnimationPanel() = default;
		AnimationPanel(const std::filesystem::path& path);

		bool InitAnimationPanel(const std::filesystem::path& path);

		void OnImGuiRender();
		void ShowAnimationPanel(bool show) { m_ShowImagePanel = show; }
	private:
		void ResetData();
	private:
		void* m_ResourceData = nullptr;
		Resource* m_LoadedResource = nullptr;
		TextureResourceData m_TextureData;
		std::filesystem::path m_OriginalResourcePath = "";
		Ref<Texture2D> m_PreviewOriginImage = nullptr;
		Ref<Animation> m_PreviewData = nullptr;
		bool m_ShowImagePanel = false;
		std::filesystem::path m_BasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Animation";
	};
}