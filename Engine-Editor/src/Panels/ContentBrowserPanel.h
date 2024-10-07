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
		void LoadIcons();
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
		void DrawCenteredText(const std::string& text, const float& cellSize);
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		Ref<Texture2D> m_ImageIcon;
		Ref<Texture2D> m_ScriptFileIcon;
		Ref<Texture2D> m_AudioFileIcon;
		Ref<Texture2D> m_AnimationFileIcon;
		Ref<Texture2D> m_PlusIcon;
		Ref<Texture2D> m_ArrowIcon;
		Scope<DeleteDirectoryPanel> m_DeleteDirectoryPanel = nullptr;
		Scope<AnimationEditorPanel> m_AnimationEditorPanel = nullptr;
	};
}