#include "egpch.h"
#include "ContentBrowserPanel.h"
#include <Imgui/imgui.h>
#include "../Commands/Commands.h"
#include "Engine/Project/Project.h"
#include "Engine/Resources/ResourceUtils.h"
#include "Engine/Resources/ResourceSerializer.h"
#include "../EditorLayer.h"

namespace eg
{
	ContentBrowserPanel::ContentBrowserPanel()
	: m_BaseDirectory(Project::GetProjectDirectory() / Project::GetAssetDirectory()), m_CurrentDirectory(m_BaseDirectory)
	{
		m_DirectoryIcon = Texture2D::Create("resources/icons/contentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("resources/icons/contentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnImGuiRender() {
		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != std::filesystem::path(Project::GetAssetDirectory())) {
			if (ImGui::Button("<-"))
			{
				std::filesystem::path oldPath = m_CurrentDirectory;
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
				Commands::ExecuteRawValueCommand(&m_CurrentDirectory, oldPath, std::string("ContentBrowserPanel-Current Directory"), true);
			}
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

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		if (m_DeleteFilePanel->GetResult() != FileDeleteMethod::Cancel)
		{
			if (m_DeleteFilePanel->GetResult() == FileDeleteMethod::DeleteFromProject)
				Commands::ExecuteDeleteResourceCommand(m_DeleteFilePanel->GetKeyPath(), m_DeleteFilePanel->GetType());
			else if (m_DeleteFilePanel->GetResult() == FileDeleteMethod::DeleteFromDisk)
				Commands::ExecuteDeleteResourceCommand(m_DeleteFilePanel->GetKeyPath(), m_DeleteFilePanel->GetType(), true);

			m_DeleteFilePanel->SetResult(FileDeleteMethod::Cancel);
			m_DeleteFilePanel->SetKeyPath(std::filesystem::path());
			m_DeleteFilePanel->SetType(ResourceType::None);
		}

		ResourceType type = ResourceUtils::GetResourceTypeFromText(m_CurrentDirectory.filename().string());

		if (type == ResourceType::Image)
		{
			for (auto& [key, value] : ResourceSerializer::TextureResourceDataCache)
			{
				auto name = value->ImageName + value->Extension;
				ImGui::PushID(name.c_str());
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::ImageButton((ImTextureID)m_FileIcon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

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
					ImGui::EndPopup();
				}

				if (ImGui::BeginDragDropSource())
				{
					std::filesystem::path path(value->GetKeyPath());
					const wchar_t* pathStr = path.c_str();
					ImGui::SetDragDropPayload("ContentBrowserPanel", pathStr, (wcslen(pathStr) + 1) * sizeof(wchar_t));
					ImGui::EndDragDropSource();
				}

				ImGui::PopStyleColor();

				ImGui::TextWrapped(name.c_str());

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
			
			Ref<Texture2D> icon = m_DirectoryIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

			if (ImGui::BeginPopupContextItem("DirectoryOptions"))
			{
				if (ImGui::MenuItem("Delete"))
				{
					remove(path);
					ImGui::PopStyleColor();
					ImGui::NextColumn();
					ImGui::EndPopup();
					ImGui::PopID();
					break;
				}
				if(ImGui::MenuItem("Change name"))
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
					std::filesystem::path keyPath = std::filesystem::path((const wchar_t*)payload->Data);
					ResourceType type = ResourceUtils::GetResourceTypeFromBackslashKeyPath(keyPath);
					std::filesystem::path droppedPath = Project::GetProjectName() / Project::GetAssetDirectory() / droppedPath;
					if (std::filesystem::exists(droppedPath))
					{
						std::filesystem::path newPath = path / droppedPath.filename();
						std::filesystem::rename(Project::GetProjectDirectory().parent_path() / droppedPath, newPath);

						if (type == ResourceType::Image)
						{
							ResourceSerializer::TextureResourceDataCache[keyPath]->ResourcePath = path;
						}
					}
					else
					{
						EG_CORE_ERROR("File does not exist");
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