#pragma once

#include <filesystem>
#include "Engine/Renderer/Texture.h"
#include "DeleteFilePanel.h"
#include "RenameFolderPanel.h"
#include "DeleteDirectoryPanel.h"
#include "RenameResourcePanel.h"
#include "CreateDirectoryPanel.h"
#include "ResourcesPanels/AnimationEditorPanel.h"

namespace eg {
	
	class ContentBrowserPanel {
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel() = default;
		void OnImGuiRender();
		void RenderFile(UUID key, const std::string& name, ResourceType type);
		friend class ProjectDirectoryPanel;
		void InitPanels();

		UUID GetCurrentDirectoryUUID() { return m_CurrentDirectory; }
		void SetCurrentDirectoryUUID(UUID uuid) { m_CurrentDirectory = uuid; }

		void Update(float ts);
	private:
		UUID m_BaseDirectory;
		UUID m_CurrentDirectory;
		void DrawCenteredText(const std::string& text, const float& cellSize);
		Ref<Texture2D> m_DirectoryIcon = nullptr;
		Ref<Texture2D> m_FileIcon = nullptr;
		Ref<Texture2D> m_ImageIcon = nullptr;
		Scope<DeleteFilePanel> m_DeleteFilePanel = nullptr;
		Scope<RenameFolderPanel> m_RenameFolderPanel = nullptr;
		Scope<RenameResourcePanel> m_RenameResourcePanel = nullptr;
		Scope<DeleteDirectoryPanel> m_DeleteDirectoryPanel = nullptr;
		Scope<CreateDirectoryPanel> m_CreateDirectoryPanel = nullptr;
		Scope<AnimationEditorPanel> m_AnimationEditorPanel = nullptr;
	};
}