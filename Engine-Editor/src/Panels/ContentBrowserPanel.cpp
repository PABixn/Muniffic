#include "egpch.h"
#include "ContentBrowserPanel.h"
#include <Imgui/imgui.h>
#include "../Commands/Commands.h"

namespace eg {

	// Once we have a proper file system, this will be replaced with a call to the file system
	static const std::filesystem::path s_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
	:m_CurrentDirectory(s_AssetPath)
	{
		m_DirectoryIcon = Texture2D::Create("resources/icons/contentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("resources/icons/contentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender() {
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(s_AssetPath)) {
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);
		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {\
			const auto& path = directoryEntry.path();
			auto name = path.filename().string();
			ImGui::PushID(name.c_str());

			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginDragDropSource())
			{
				auto relativePath = std::filesystem::relative(path, s_AssetPath);
				const wchar_t* pathStr = relativePath.c_str();
				ImGui::SetDragDropPayload("ContentBrowserPanel", pathStr, (wcslen(pathStr)+1) * sizeof(wchar_t));
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				if (directoryEntry.is_directory())
					m_CurrentDirectory /= path.filename();
			}
			ImGui::TextWrapped(name.c_str());

			ImGui::NextColumn();

			ImGui::PopID();
		}
		ImGui::Columns(1);

		float size = thumbnailSize, offset = padding;

		if(ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512))
			Commands::ExecuteRawValueCommand(&thumbnailSize, size, std::string("ContentBrowserPanel-Thumbnail Size"));
			
		if(ImGui::SliderFloat("Padding", &padding, 0, 32))
			Commands::ExecuteRawValueCommand(&padding, offset, std::string("ContentBrowserPanel-Padding"));

		ImGui::End();
	}

}