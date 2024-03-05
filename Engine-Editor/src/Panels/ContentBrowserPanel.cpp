#include "egpch.h"
#include "ContentBrowserPanel.h"
#include <Imgui/imgui.h>
#include "../Commands/Commands.h"
#include "Engine/Project/Project.h"
#include "Engine/Resources/ResourceUtils.h"
#include "Engine/Resources/ResourceSerializer.h"
#include "Engine/Resources/ResourceDatabase.h"
#include "../EditorLayer.h"

namespace eg
{
	ContentBrowserPanel::ContentBrowserPanel()
	: m_BaseDirectory(Project::GetProjectDirectory() / Project::GetAssetDirectory()), m_CurrentDirectory(m_BaseDirectory)
	{
		m_DirectoryIcon = Texture2D::Create("resources/icons/contentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("resources/icons/contentBrowser/FileIcon.png");
		ResourceDatabase::SetCurrentPath(&m_CurrentDirectory);
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

			//accept dragging files from windows

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserPanel"))
				{
					uint64_t uuid = *(uint64_t*)payload->Data;
					ResourceType type = ResourceSerializer::ResourceTypeInfo[uuid];
					std::filesystem::path keyPath = ResourceUtils::GetKeyPath(uuid);

					std::filesystem::path droppedPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / keyPath;

					if (std::filesystem::exists(droppedPath))
					{
						std::filesystem::path newPath = m_CurrentDirectory.parent_path() / keyPath.filename();

						int result = std::rename(droppedPath.string().c_str(), newPath.string().c_str());
						
						if (result == 0)
						{
							if (type == ResourceType::Image)
								ResourceSerializer::TextureResourceDataCache[uuid]->ResourcePath = ResourceUtils::GetResourcePath(m_CurrentDirectory.parent_path());
						}
						else
						{
							EG_CORE_ERROR("Failed to move file");
						}
					}
					else
					{
						EG_CORE_ERROR("File does not exist");
					}
				}
				ImGui::EndDragDropTarget();
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

		ResourceType type = ResourceUtils::GetCurrentResourceDirectoryType(m_CurrentDirectory);

		if (type == ResourceType::Image)
		{
			for (auto& [key, value] : ResourceSerializer::TextureResourceDataCache)
			{
				if(value->ResourcePath != ResourceUtils::GetResourcePath(m_CurrentDirectory))
					continue;

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
					ImGui::SetDragDropPayload("ContentBrowserPanel", &key, sizeof(uint64_t));
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
					uint64_t uuid = *(uint64_t*)payload->Data;
					ResourceType type = ResourceSerializer::ResourceTypeInfo[uuid];
					std::filesystem::path keyPath = ResourceUtils::GetKeyPath(uuid);

					std::filesystem::path droppedPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / keyPath;

					if (std::filesystem::exists(droppedPath))
					{
						std::filesystem::path newPath = path / keyPath.filename();

						std::rename(droppedPath.string().c_str(), newPath.string().c_str());

						if (type == ResourceType::Image)
						{
							ResourceSerializer::TextureResourceDataCache[uuid]->ResourcePath = ResourceUtils::GetResourcePath(path);
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