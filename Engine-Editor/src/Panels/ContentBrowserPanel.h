#pragma once

#include <filesystem>
#include "Engine/Renderer/Texture.h"
#include "DeleteFilePanel.h"
#include "RenameFolderPanel.h"
#include "DeleteDirectoryPanel.h"
#include "RenameResourcePanel.h"

namespace eg {
	
	class ContentBrowserPanel {
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel() = default;
		void OnImGuiRender();
		void SetDeleteFilePanel(DeleteFilePanel* deleteFilePanel) { m_DeleteFilePanel = deleteFilePanel; }
		void SetRenameFolderPanel(RenameFolderPanel* renameFolderPanel) { m_RenameFolderPanel = renameFolderPanel; }
		void SetDeleteDirectoryPanel(DeleteDirectoryPanel* deleteDirectoryPanel) { m_DeleteDirectoryPanel = deleteDirectoryPanel; }
		void SetRenameResourcePanel(RenameResourcePanel* renameResourcePanel) { m_RenameResourcePanel = renameResourcePanel; }
		std::filesystem::path GetCurrentPath() { return m_CurrentDirectory; }

	private:
		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;
		
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		DeleteFilePanel* m_DeleteFilePanel;
		RenameFolderPanel* m_RenameFolderPanel;
		RenameResourcePanel* m_RenameResourcePanel;
		DeleteDirectoryPanel* m_DeleteDirectoryPanel;
	};
}