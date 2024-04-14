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
#include "Engine/Resources/AssetDirectoryManager.h"

namespace eg
{
	ContentBrowserPanel::ContentBrowserPanel()
	: m_BaseDirectory(AssetDirectoryManager::getRootDirectoryUUID()), m_CurrentDirectory(m_BaseDirectory)
	{
		m_DirectoryIcon = Texture2D::Create("resources/icons/contentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("resources/icons/contentBrowser/FileIcon.png");
		ResourceDatabase::SetCurrentDirectoryUUID(m_CurrentDirectory);
	}

	void ContentBrowserPanel::InitPanels()
	{
		m_DeleteFilePanel = CreateScope<DeleteFilePanel>();
		m_RenameFolderPanel = CreateScope<RenameFolderPanel>();
		m_DeleteDirectoryPanel = CreateScope<DeleteDirectoryPanel>();
		m_RenameResourcePanel = CreateScope<RenameResourcePanel>();
		m_CreateDirectoryPanel = CreateScope<CreateDirectoryPanel>();
	}

	void ContentBrowserPanel::RenderFile(UUID key, const std::string& name, ResourceType type)
	{
		static float thumbnailSize = 128.0f;

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
				return;
			}

	}

	void ContentBrowserPanel::OnImGuiRender() {
		
		ImGui::Begin("Content Browser", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);

		if (m_CurrentDirectory != std::filesystem::path(Project::GetAssetDirectory())) {
			if (ImGui::Button("<-"))
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

		ImGui::TextWrapped(name.c_str());

		ImGui::NextColumn();

		ImGui::PopID();
	}

	void ContentBrowserPanel::OnImGuiRender() {
		if (m_DeleteFilePanel->IsShown())
			m_DeleteFilePanel->OnImGuiRender();

		if (m_RenameFolderPanel->IsShown())
			m_RenameFolderPanel->OnImGuiRender();

		if (m_DeleteDirectoryPanel->IsShown())
			m_DeleteDirectoryPanel->OnImGuiRender();

		if (m_RenameResourcePanel->IsShown())
			m_RenameResourcePanel->OnImGuiRender();

		if (m_CreateDirectoryPanel->IsShown())
			m_CreateDirectoryPanel->OnImGuiRender();

		ImGui::Begin("Content Browser");

		if (m_CurrentDirectory != AssetDirectoryManager::getRootDirectoryUUID()) {
			if (ImGui::Button("<-"))
			{
				UUID oldPath = m_CurrentDirectory;
				m_CurrentDirectory = AssetDirectoryManager::getParentDirectoryUUID(m_CurrentDirectory);
				Commands::ExecuteRawValueCommand(&m_CurrentDirectory, oldPath, std::string("ContentBrowserPanel-Current Directory"), true);
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserPanel"))
				{
					if (ResourceUtils::CanDrop(AssetDirectoryManager::getParentDirectoryUUID(m_CurrentDirectory)))
					{
						uint64_t uuid = *(uint64_t*)payload->Data;
						if (ResourceDatabase::FindResourceData(uuid))
							Commands::ExecuteMoveResourceCommand(uuid, AssetDirectoryManager::getParentDirectoryUUID(m_CurrentDirectory));
						else
							Commands::ExecuteMoveDirectoryCommand(uuid, AssetDirectoryManager::getParentDirectoryUUID(m_CurrentDirectory));
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
				m_CreateDirectoryPanel->ShowWindow(m_CurrentDirectory);
				//AssetDirectory* newDirectory = new AssetDirectory(UUID(), "New Folder", m_CurrentDirectory);
			}
			ImGui::EndPopup();
		}

		ImGui::SameLine();

		static char buffer[256];
		ImGui::InputText("##Filter", buffer, 256);

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
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

		for (UUID asset : AssetDirectoryManager::getAssets(m_CurrentDirectory))
		{
			ResourceType type = ResourceDatabase::GetResourceType(asset);
			std::string name = ResourceDatabase::GetResourceName(asset);
			if (name.find(buffer) == std::string::npos)
				continue;

			RenderFile(asset, name, type);
		}

		for (UUID directory : AssetDirectoryManager::getSubdirectories(m_CurrentDirectory))
		{
			std::string name = AssetDirectoryManager::getDirectoryName(directory);
			ImGui::PushID(name.c_str());
			
			Ref<Texture2D> icon = m_DirectoryIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize });

			if (ImGui::BeginPopupContextItem("DirectoryOptions"))
			{
				if (ImGui::MenuItem("Delete"))
				{
					m_DeleteDirectoryPanel->ShowWindow(directory);
					ImGui::PopStyleColor();
					ImGui::NextColumn();
					ImGui::EndPopup();
					ImGui::PopID();
					break;
				}

				if(ImGui::MenuItem("Rename"))
				{
					m_RenameFolderPanel->ShowWindow(directory);
				}

				ImGui::EndPopup();
			}

			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("ContentBrowserPanel", &directory, sizeof(uint64_t));
				ImGui::Text(name.c_str());
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserPanel"))
				{
					if (ResourceUtils::CanDrop(directory))
					{
						uint64_t uuid = *(uint64_t*)payload->Data;
						
						if(ResourceDatabase::FindResourceData(uuid))
							Commands::ExecuteMoveResourceCommand(uuid, directory);
						else
							Commands::ExecuteMoveDirectoryCommand(uuid, directory);
					}
				}
				ImGui::EndDragDropTarget();
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				UUID oldPath = m_CurrentDirectory;
				m_CurrentDirectory = directory;
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