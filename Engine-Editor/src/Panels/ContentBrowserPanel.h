#pragma once

#include <filesystem>
#include "Engine/Renderer/Texture.h"
#include "DeleteFilePanel.h"
#include "RenameFolderPanel.h"

namespace eg {
	
	class ContentBrowserPanel {
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel() = default;
		void OnImGuiRender();
		void SetDeleteFilePanel(DeleteFilePanel* deleteFilePanel) { m_DeleteFilePanel = deleteFilePanel; }
		void SetRenameFolderPanel(RenameFolderPanel* renameFolderPanel) { m_RenameFolderPanel = renameFolderPanel; }

	private:
		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_CurrentDirectory;
		
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		DeleteFilePanel* m_DeleteFilePanel;
		RenameFolderPanel* m_RenameFolderPanel;
	};
}