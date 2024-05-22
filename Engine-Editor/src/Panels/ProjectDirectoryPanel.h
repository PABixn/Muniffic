#pragma once
#include "filesystem"
#include "Engine/Renderer/Texture.h"
#include "ContentBrowserPanel.h"

namespace eg {
	class ProjectDirectoryPanel{
	public:
		ProjectDirectoryPanel();
		void OnImGuiRender();
		void SetContentBrowserPanel(Ref<ContentBrowserPanel> val) { m_ContentBrowserPanel = val; };
	private:
		std::string m_SearchedName;
		Ref<ContentBrowserPanel> m_ContentBrowserPanel;

		void drawDirectoryTreeNode(const std::filesystem::path& path);
	};
}