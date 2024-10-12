#pragma once

#include <filesystem>
#include "Engine/Renderer/Texture.h"
#include "DeleteDirectoryPanel.h"
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
		void DrawIcon(ResourceType type);
		void ShowDeleteFolderPopup(UUID directory);
		void ShowRenameFolderPopup(UUID directory);
		void ShowCreateFolderPopup();
		void ShowDeleteFilePopup(UUID file, ResourceType type);
		void ShowRenameFilePopup(UUID file, ResourceType type);
		void ShowFilePopups(UUID file, ResourceType type);
		void ShowFolderPopups(UUID folder);
		void ShowFileMenu(UUID file, ResourceType type);
		void ShowFolderMenu(UUID directory);

		UUID GetCurrentDirectoryUUID() { return m_CurrentDirectory; }
		void SetCurrentDirectoryUUID(UUID uuid) { m_CurrentDirectory = uuid; }

		void Update(float ts);
	private:
		UUID m_BaseDirectory;
		UUID m_CurrentDirectory;
		Scope<DeleteDirectoryPanel> m_DeleteDirectoryPanel = nullptr;
		Scope<AnimationEditorPanel> m_AnimationEditorPanel = nullptr;
	};
}