#include "egpch.h"
#include "ContentBrowserPanel.h"
#include <Imgui/imgui.h>
#include "../Commands/Commands.h"
#include "Engine/Project/Project.h"
#include "Engine/Resources/ResourceUtils.h"
#include "Engine/Resources/ResourceSerializer.h"
#include "Engine/Resources/ResourceDatabase.h"
#include "../EditorLayer.h"
#include "Engine/Resources/AssetDirectoryManager.h"

namespace eg
{
	ContentBrowserPanel::ContentBrowserPanel()
	: m_BaseDirectory(AssetDirectoryManager::getRootDirectoryUUID()), m_CurrentDirectory(m_BaseDirectory)
	{
		m_DirectoryIcon = Texture2D::Create("resources/icons/contentBrowser/FolderIcon.png");
		m_FileIcon = Texture2D::Create("resources/icons/contentBrowser/FileIcon.png");
		m_PlusIcon = Texture2D::Create("resources/icons/contentBrowser/PlusIcon.png");
		m_ArrowIcon = Texture2D::Create("resources/icons/contentBrowser/ArrowIcon.png");
		ResourceDatabase::SetCurrentDirectoryUUID(m_CurrentDirectory);
		auto& io = ImGui::GetIO();
		m_PoppinsRegularFont = io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-Regular.ttf", 20.0f, NULL, io.Fonts->GetGlyphRangesDefault());
		IM_ASSERT(m_PoppinsRegularFont != nullptr);
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
		static float thumbnailSize = 100.0f;

		ImGui::PushID(name.c_str());

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		ImGui::ImageButton((ImTextureID)m_FileIcon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

		ImGui::PushFont(m_PoppinsRegularFont);
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

		ImGui::PopFont();
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

		ImGui::PushStyleColor(ImGuiCol_WindowBg, static_cast<EditorLayer*>(Application::Get().GetFirstLayer())->m_DarkShade);
		ImGui::Begin("Content Browser");

		ImGuiStyle& style = ImGui::GetStyle();
		auto btnColor = style.Colors[ImGuiCol_Button];
		auto btnColorHovered = style.Colors[ImGuiCol_ButtonHovered];
		auto btnColorActive = style.Colors[ImGuiCol_ButtonActive];

		style.Colors[ImGuiCol_Button] = ImVec4(0, 0, 0, 0);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0, 0, 0, 0);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0, 0, 0, 0);
		ImGui::PushFont(m_PoppinsRegularFont);

		ImDrawList* drawList = ImGui::GetWindowDrawList();
		

		if (m_CurrentDirectory != AssetDirectoryManager::getRootDirectoryUUID()) {

			drawList->ChannelsSplit(2);
			drawList->ChannelsSetCurrent(1);
			if (ImGui::ImageButton((ImTextureID)m_ArrowIcon->GetRendererID(), { 30, 30 }))
			{
				UUID oldPath = m_CurrentDirectory;
				m_CurrentDirectory = AssetDirectoryManager::getParentDirectoryUUID(m_CurrentDirectory);
				Commands::ExecuteRawValueCommand(&m_CurrentDirectory, oldPath, std::string("ContentBrowserPanel-Current Directory"), true);
			}
			drawList->ChannelsSetCurrent(0);
			ImVec2 buttonPos = ImGui::GetItemRectMin();
			ImVec2 buttonSize = ImGui::GetItemRectSize();

			ImVec2 buttonCenter = ImVec2(buttonPos.x + buttonSize.x * 0.5f, buttonPos.y + buttonSize.y * 0.5f);

			if (ImGui::IsItemHovered() || ImGui::IsItemActive()) {
				drawList->AddCircleFilled(buttonCenter, 20.0f, ImGui::ColorConvertFloat4ToU32(
					static_cast<EditorLayer*>(Application::Get().GetFirstLayer())->m_NormalShade
				));
			}
			drawList->ChannelsMerge();

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
		

		ImGui::SameLine();
		drawList->ChannelsSplit(2);
		drawList->ChannelsSetCurrent(1);
		if(ImGui::ImageButton((ImTextureID)m_PlusIcon->GetRendererID(), ImVec2(30,30)))
		{
			ImGui::OpenPopup("CreateNewResource");
		}

		drawList->ChannelsSetCurrent(0);
		ImVec2 buttonPos = ImGui::GetItemRectMin();
		ImVec2 buttonSize = ImGui::GetItemRectSize();

		ImVec2 buttonCenter = ImVec2(buttonPos.x + buttonSize.x * 0.5f, buttonPos.y + buttonSize.y * 0.5f);

		if (ImGui::IsItemHovered() || ImGui::IsItemActive()) {
			drawList->AddCircleFilled(buttonCenter, 20.0f, ImGui::ColorConvertFloat4ToU32(
				static_cast<EditorLayer*>(Application::Get().GetFirstLayer())->m_NormalShade
			));
		}
		drawList->ChannelsMerge();

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

		static float padding = 25.0f;
		static float thumbnailSize = 100.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

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
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

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

		/*if (ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512))
			Commands::ExecuteRawValueCommand(&thumbnailSize, size, std::string("ContentBrowserPanel-Thumbnail Size"));*/
			
		/*if (ImGui::SliderFloat("Padding", &padding, 0, 32))
			Commands::ExecuteRawValueCommand(&padding, offset, std::string("ContentBrowserPanel-Padding"));*/
		ImGui::PopFont();
		ImGui::End();
		ImGui::PopStyleColor();
		style.Colors[ImGuiCol_Button] = btnColor;
		style.Colors[ImGuiCol_ButtonHovered] = btnColorHovered;
		style.Colors[ImGuiCol_ButtonActive] = btnColorActive;
	}

}