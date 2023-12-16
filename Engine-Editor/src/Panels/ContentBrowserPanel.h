#pragma once
#include <filesystem>
#include "Engine/Renderer/Texture.h"
namespace eg {
	
	class ContentBrowserPanel {
	public:
		ContentBrowserPanel();
		~ContentBrowserPanel() = default;

		void OnImGuiRender();
	private:
		std::filesystem::path m_CurrentDirectory;
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};
}