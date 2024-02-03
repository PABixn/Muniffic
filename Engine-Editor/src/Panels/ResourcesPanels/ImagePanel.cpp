#include "egpch.h"
#include "ImagePanel.h"
#include <imgui/imgui.h>

namespace eg
{
	ImagePanel::ImagePanel(const std::filesystem::path& path)
				: m_ImagePath(path)
	{
	}

	void ImagePanel::OnImGuiRender()
	{
		ImGui::Begin("Image");
		if (m_ImagePath.empty())
		{
			ImGui::Text("No image chosen");
		}
		else
		{
			ImGui::Text("Image: %s", m_ImagePath.string().c_str());
			ImGui::Text("Width: %d", width);
			ImGui::Text("Height: %d", height);
			ImGui::Text("Original Width: %d", originalWidth);
			ImGui::Text("Original Height: %d", originalHeight);
			//ImGui::Image((void*)(intptr_t)textureID, ImVec2(200, 200));
		}
		ImGui::End();
	}
}