#pragma once
#include "Engine.h"
#include <filesystem>

namespace eg
{
	class AnimationPanel {
	public:
		AnimationPanel() = default;
		AnimationPanel(const std::filesystem::path& path);

		bool InitAnimationPanel() { m_ShowAnimationPanel = false; return true; }

		void OnImGuiRender();
		void OnUpdate(float dt) { m_PreviewData->Update(dt, 1.0f); }

		bool OpenAnimationPanel(const std::filesystem::path& path);
		void CloseAnimationPanel();

		bool IsAnimationPanelOpen() { return m_ShowAnimationPanel; }
	public:
		std::vector<std::pair<int, int>> m_SelectedFrames;
		Ref<Texture2D> m_PreviewOriginImage = nullptr;
		int m_FrameWidth = 0, m_FrameHeight = 0;
		int m_BasePreviewWidthImage = 1, m_BasePreviewHeightImage = 1;
		TextureResourceData* m_TextureData = nullptr;
	private:
		void ShowAnimationPanel(bool show) { m_ShowAnimationPanel = show; }
		void ResetData();
		void SetFrames();
		void DeleteData();
	private:
		AnimationResourceData* m_ResourceData = nullptr;
		Resource* m_LoadedResource = nullptr;
		std::filesystem::path m_ResourcePath = "";
		
		int m_Column = 0, m_Row = 0;
		int m_ColumnCount = 1, m_RowCount = 1;
		
		float m_ImageAspectRatio = 1;
		
		float m_PreviewAspectRatio = 1;
		int m_BasePreviewWidth = 1, m_BasePreviewHeight = 1;

		//TextureResourceData m_TextureData;
		std::filesystem::path m_OriginalResourcePath = "";

		UUID m_TextureUUID;
		
		Ref<Animation> m_PreviewData = nullptr;
		bool m_ShowAnimationPanel = false;
		std::filesystem::path m_BasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Animation";
	};
}