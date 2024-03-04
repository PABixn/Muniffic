#pragma once
#include "Engine.h"
#include <filesystem>
#include "Engine/Resources/Systems/ResourceSystem.h"
#include "ResourcesPanels/ImagePanel.h"


namespace eg {
	class EditorLayer;
	class AddResourcePanel {
	public:
		AddResourcePanel();

		void OnImGuiRender();

		bool ChooseNewResource(const std::string filter);
		void showResourcePanel(bool show) { m_showResourcePanel = show; }
	private:
		Ref<ImagePanel> m_ImagePanel;

		bool m_showResourcePanel = false;
		std::filesystem::path m_ResourcePath;
	};
}