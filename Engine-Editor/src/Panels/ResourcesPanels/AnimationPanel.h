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
		void OnUpdate(float dt) { m_PreviewData->Update(dt, 1.0f); }

		void ShowAnimationPanel(bool show) { m_ShowImagePanel = show; }
	private:
		void ResetData();
		void SaveAnimation();
	private:
		void* m_ResourceData = nullptr;
		Resource* m_LoadedResource = nullptr;
		int m_FrameWidth = 0, m_FrameHeight = 0;
		int m_Column = 0, m_Row = 0;
		int m_ColumnCount = 0, m_RowCount = 0;
		TextureResourceData m_TextureData;
		std::filesystem::path m_OriginalResourcePath = "";
		Ref<Texture2D> m_PreviewOriginImage = nullptr;
		Ref<Animation> m_PreviewData = nullptr;
		bool m_ShowImagePanel = false;
		std::filesystem::path m_BasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Animation";
	};
}