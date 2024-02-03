#pragma once
#include <filesystem>

namespace eg {

	class ImagePanel {
	public:
		ImagePanel() = default;
		ImagePanel(const std::filesystem::path& path);

		void OnImGuiRender();
	private:
		uint32_t width = 0, height = 0, originalHeight = 0, originalWidth = 0;
		std::filesystem::path m_ImagePath;
	};
}