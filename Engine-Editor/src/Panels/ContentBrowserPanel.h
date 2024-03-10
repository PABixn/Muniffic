#pragma once

#include <filesystem>
#include "Engine/Renderer/Texture.h"
#include "DeleteFilePanel.h"
#include "RenameFolderPanel.h"
#include "DeleteDirectoryPanel.h"

namespace eg {
	
	class ContentBrowserPanel {
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel() = default;
		void OnImGuiRender();
		void SetDeleteFilePanel(DeleteFilePanel* deleteFilePanel) { m_DeleteFilePanel = deleteFilePanel; }
		void SetRenameFolderPanel(RenameFolderPanel* renameFolderPanel) { m_RenameFolderPanel = renameFolderPanel; }
		void SetDeleteDirectoryPanel(DeleteDirectoryPanel* deleteDirectoryPanel) { m_DeleteDirectoryPanel = deleteDirectoryPanel; }
		std::filesystem::path GetCurrentPath() { return m_CurrentDirectory; }

	private:
		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;
		void DrawCenteredText(const std::string& text, const float& cellSize);
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_ImageIcon;
		DeleteFilePanel* m_DeleteFilePanel;
		RenameFolderPanel* m_RenameFolderPanel;
		DeleteDirectoryPanel* m_DeleteDirectoryPanel;
	};
}