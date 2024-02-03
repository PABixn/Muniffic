#pragma once
#include "Engine.h"
#include <filesystem>
#include "ResourcesPanels/ImagePanel.h"

namespace eg {

	class AddResourcePanel {
	public:
		AddResourcePanel() = default;

		void OnImGuiRender();

		bool ChooseNewResource(const std::string filter);
	private:
		ResourceType m_ChosenResourceType = ResourceType::None;
		std::filesystem::path m_ResourcePath;

		ImagePanel m_ImagePanel;
	};
}