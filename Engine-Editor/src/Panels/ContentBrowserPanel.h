#pragma once

#include <filesystem>
#include "Engine/Renderer/Texture.h"
#include "DeleteFilePanel.h"
#include "RenameFolderPanel.h"
#include "DeleteDirectoryPanel.h"
#include "RenameResourcePanel.h"
#include "CreateDirectoryPanel.h"

namespace eg {
	
	class ContentBrowserPanel {
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel() = default;
		void OnImGuiRender();
		void RenderFile(UUID key, const std::string& name, ResourceType type);
		void InitPanels();

		UUID GetCurrentDirectoryUUID() { return m_CurrentDirectory; }
		void SetCurrentDirectoryUUID(UUID uuid) { m_CurrentDirectory = uuid; }

	private:
		UUID m_BaseDirectory;
		UUID m_CurrentDirectory;
		void DrawCenteredText(const std::string& text, const float& cellSize);
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_ImageIcon;
		Scope<DeleteFilePanel> m_DeleteFilePanel;
		Scope<RenameFolderPanel> m_RenameFolderPanel;
		Scope<RenameResourcePanel> m_RenameResourcePanel;
		Scope<DeleteDirectoryPanel> m_DeleteDirectoryPanel;
		Scope<CreateDirectoryPanel> m_CreateDirectoryPanel;
	};
}