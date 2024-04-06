#include "egpch.h"
#include "ContentBrowserPanel.h"
#include <Imgui/imgui.h>
#include "../Commands/Commands.h"
#include "Engine/Project/Project.h"
#include "Engine/Resources/ResourceUtils.h"
#include "Engine/Resources/ResourceSerializer.h"
#include "Engine/Resources/ResourceDatabase.h"
#include "../EditorLayer.h"
#include <sys/stat.h>

namespace eg
{
	ContentBrowserPanel::ContentBrowserPanel()
	: m_BaseDirectory(Project::GetProjectDirectory() / Project::GetAssetDirectory()), m_CurrentDirectory(m_BaseDirectory)
	{
		m_DirectoryIcon = Texture2D::Create("resources/icons/contentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("resources/icons/contentBrowser/FileIcon.png");
		m_ImageIcon = Texture2D::Create("resources/icons/contentBrowser/ImageIcon.png");
	}
	void ContentBrowserPanel::DrawCenteredText(const std::string& text, const float& cellSize){
		auto textWidth = ImGui::CalcTextSize(text .c_str()).x;
		auto CursorX = ImGui::GetCursorPosX();
		float offset = (cellSize - textWidth) * 0.43f;
		if (textWidth < cellSize) {
			ImGui::SetCursorPosX(CursorX + offset);
			ImGui::TextWrapped(text.c_str());
		}
		else {
		int a = ceil(textWidth / cellSize)+1;
		for (int i = 0; i < a; i++) {
			if (i>=2)
			{
				auto r = text.substr((text.length() / a) * i, (text.length() / a)-3);
				r += "...";
				textWidth = ImGui::CalcTextSize(r.c_str()).x;
				CursorX = ImGui::GetCursorPosX();
				offset = (cellSize - textWidth) * 0.45f;
				ImGui::SetCursorPosX(CursorX + offset);
				ImGui::TextWrapped(r.c_str());
				return;
			}
			auto r = text.substr((text.length() / a) * i, (text.length() / a));
			textWidth = ImGui::CalcTextSize(r.c_str()).x;
			CursorX = ImGui::GetCursorPosX();
			offset = (cellSize - textWidth) * 0.45f;
			ImGui::SetCursorPosX(CursorX + offset);
			ImGui::TextWrapped(r.c_str());
		}
		auto r = text.substr((text.length() / a) * a);
		if (r != "")
		{
			textWidth = ImGui::CalcTextSize(r.c_str()).x;
			CursorX = ImGui::GetCursorPosX();
			offset = (cellSize - textWidth) * 0.5f;
			ImGui::SetCursorPosX(CursorX + offset);
			ImGui::TextWrapped(r.c_str());
		}
		}

	}

	void ContentBrowserPanel::OnImGuiRender() {
		
		ImGui::Begin("Content Browser", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);

		if (m_CurrentDirectory != std::filesystem::path(Project::GetAssetDirectory())) {
			if (ImGui::Button("<-"))
			{
				std::filesystem::path oldPath = m_CurrentDirectory;
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
				Commands::ExecuteRawValueCommand(&m_CurrentDirectory, oldPath, std::string("ContentBrowserPanel-Current Directory"), true);
			}

			//accept dragging files from windows

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserPanel"))
				{
					if (ResourceUtils::CanDrop(m_CurrentDirectory.parent_path()))
					{
						uint64_t uuid = *(uint64_t*)payload->Data;

						Commands::ExecuteMoveResourceCommand(uuid, m_CurrentDirectory.parent_path());

						//ResourceDatabase::MoveResource(uuid, m_CurrentDirectory.parent_path());
					}
				}
				ImGui::EndDragDropTarget();
			}
		}
		bool tableView = false;
		if (tableView) {
			ImGui::BeginTable("table view",2, ImGuiTableFlags_BordersInner);
			ImGui::TableSetupColumn(nullptr, ImGuiTableColumnFlags_WidthStretch); 
			for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
			{
				const auto& path = directoryEntry.path();

				if (path.extension() == ".mnmeta")
					continue;

				std::filesystem::path metaPath = path.parent_path() / (path.stem().string() + ".mnmeta");

				if (!directoryEntry.is_directory() && !std::filesystem::exists(metaPath))
					continue;

				ImGui::SetNextItemWidth(0.1f);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				auto name = path.filename().string();
				ImGui::PushID(name.c_str());
				Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { 20,20 });
				ImGui::TableNextColumn();

				ImGui::PopStyleColor();

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					if (directoryEntry.is_directory())
					{
						std::filesystem::path oldPath = m_CurrentDirectory;
						m_CurrentDirectory /= path.filename();
						Commands::ExecuteRawValueCommand(&m_CurrentDirectory, oldPath, std::string("ContentBrowserPanel-Current Directory"), true);
					}
				}

				ImGui::Text(name.c_str());

				ImGui::PopID();
			}

		ImGui::SameLine();

		if(ImGui::Button("+"))
		{
			ImGui::OpenPopup("CreateNewResource");
		}

		if (ImGui::BeginPopup("CreateNewResource"))
		{
			if (ImGui::MenuItem("Create Folder"))
			{
				std::filesystem::path newDirectory = m_CurrentDirectory / "New Folder";
				std::filesystem::create_directory(newDirectory);
			}
			ImGui::EndPopup();
		}

			ImGui::EndTable();
		}
		else {
		static float padding = 16.0f;
		static float thumbnailSize = 100.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);
		ResourceType type = ResourceUtils::GetResourceTypeFromText(m_CurrentDirectory.filename().string());

		if (type == ResourceType::Image)
		{
			for (auto& [key, value] : ResourceSerializer::TextureResourceDataCache)
			{
				if(value->ResourcePath != ResourceUtils::GetResourcePath(m_CurrentDirectory))
					continue;

				auto name = value->ImageName + value->Extension;
				ImGui::PushID(name.c_str());
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				if (ImGui::ImageButton((ImTextureID)m_ImageIcon->GetRendererID(), { thumbnailSize, thumbnailSize }))
				{
					auto* s = (*(dynamic_cast<EditorLayer*>(Application::Get().GetFirstLayer()))).GetSceneHierarchyPanel(); 
					(*s).SetPreviewAbsoluteImagePath(std::filesystem::path(value->GetAbsolutePath()));
					(*s).SetPreviewRelativeImagePath(std::filesystem::path(value->GetRelativePath()));
				}

				if (ImGui::BeginPopupContextItem("FileOptions"))
				{
					if (ImGui::MenuItem("Delete"))
					{
						m_DeleteFilePanel->ShowWindow(key, type);
						ImGui::PopStyleColor();
						ImGui::NextColumn();
						ImGui::EndPopup();
						ImGui::PopID();
						break;
					}

					if (ImGui::MenuItem("Rename"))
					{
						m_RenameResourcePanel->ShowWindow(key);
					}

					ImGui::EndPopup();
				}

				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("ContentBrowserPanel", &key, sizeof(uint64_t));
					ImGui::EndDragDropSource();
				}

				ImGui::PopStyleColor();
				
				DrawCenteredText(name.c_str(), cellSize);

				ImGui::NextColumn();

				ImGui::PopID();
			}
		}

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			
			if (!directoryEntry.is_directory())
				continue;

			auto name = path.filename().string();
			ImGui::PushID(name.c_str());
			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize });

			if (ImGui::BeginPopupContextItem("DirectoryOptions"))
			{
				if (ImGui::MenuItem("Delete"))
				{
					m_DeleteDirectoryPanel->ShowWindow(path);
					ImGui::PopStyleColor();
					ImGui::NextColumn();
					ImGui::EndPopup();
					ImGui::PopID();
					break;
				}

				if(ImGui::MenuItem("Rename"))
				{
					m_RenameFolderPanel->ShowWindow(path);
				}

				ImGui::EndPopup();
			}

			if (ImGui::BeginDragDropSource())
			{
				std::filesystem::path relativePath(path);
				const wchar_t* pathStr = relativePath.c_str();
				ImGui::SetDragDropPayload("ContentBrowserPanel", pathStr, (wcslen(pathStr)+1) * sizeof(wchar_t));
				ImGui::Text(name.c_str());
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserPanel"))
				{
					if (ResourceUtils::CanDrop(path))
					{
						uint64_t uuid = *(uint64_t*)payload->Data;
						
						Commands::ExecuteMoveResourceCommand(uuid, path);
					}
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				std::filesystem::path oldPath = m_CurrentDirectory;
				m_CurrentDirectory /= path.filename();
				Commands::ExecuteRawValueCommand(&m_CurrentDirectory, oldPath, std::string("ContentBrowserPanel-Current Directory"), true);
			}

			DrawCenteredText(name, cellSize);

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		float size = thumbnailSize, offset = padding;
		/*
		if(ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512))
			Commands::ExecuteRawValueCommand(&thumbnailSize, size, std::string("ContentBrowserPanel-Thumbnail Size"));
			
		if(ImGui::SliderFloat("Padding", &padding, 0, 32))
			Commands::ExecuteRawValueCommand(&padding, offset, std::string("ContentBrowserPanel-Padding"));
			*/

		}
		ImGui::End();
	}

}