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
	private:
		void ShowAnimationPanel(bool show) { m_ShowAnimationPanel = show; }
		void ResetData();
		void SetFrames();
		void SetSelectedFrames();
		void ClearOutdatedFrames();
		void DeleteData();
		void SaveData();
		void DrawSubtexture(const int& i, const int& j);
		void DrawFrameSelection();
		void DrawAnimInfo();
		void DrawAnimationPreview();
	private:
		int m_FrameWidth = 0, m_FrameHeight = 0;
		int m_Column = 0, m_Row = 0;
		int m_ColumnCount = 1, m_RowCount = 1;
		int m_MaxColumn = 1, m_MaxRow = 1;
		float m_ImageAspectRatio = 1;
		int m_BasePreviewWidthImage = 1, m_BasePreviewHeightImage = 1;
		float m_PreviewAspectRatio = 1;
		int m_BasePreviewWidth = 1, m_BasePreviewHeight = 1;
		bool m_ShowAnimationPanel = false;
		int gap = 0;

		UUID m_TextureUUID;
		Resource* m_LoadedResource = nullptr;
		AnimationResourceData* m_ResourceData = nullptr;
		TextureResourceData* m_TextureData = nullptr;
		Ref<Texture2D> m_PreviewOriginImage = nullptr;
		Ref<Animation> m_PreviewData = nullptr;
		Ref<FrameData> m_FrameData;
		std::filesystem::path m_ResourcePath = "";
		std::vector<std::pair<int,int>> m_SelectedFrames;
		std::filesystem::path m_OriginalResourcePath = "";
		std::filesystem::path m_BasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Animation";
	};
}