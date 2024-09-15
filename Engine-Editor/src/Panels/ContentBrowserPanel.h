#pragma once

#include <filesystem>
#include "Engine/Renderer/Texture.h"
#include "RenameFolderPanel.h"
#include "DeleteDirectoryPanel.h"

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
		//void DrawCenteredText(const std::string& text, const float& cellSize);
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_ImageIcon;
		Ref<Texture2D> m_PlusIcon;
		Ref<Texture2D> m_ArrowIcon;
		ImFont* m_PoppinsRegularFont;
		Scope<RenameFolderPanel> m_RenameFolderPanel;
		Scope<DeleteDirectoryPanel> m_DeleteDirectoryPanel;
	};
}