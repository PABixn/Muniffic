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
#include "../IconLoader.h"

namespace eg
{
	ContentBrowserPanel::ContentBrowserPanel()
	: m_BaseDirectory(AssetDirectoryManager::getRootDirectoryUUID()), m_CurrentDirectory(m_BaseDirectory)
	{
        EG_PROFILE_FUNCTION();
		ResourceDatabase::SetCurrentDirectoryUUID(m_CurrentDirectory);
	}

	void ContentBrowserPanel::Update(float ts)
	{
        EG_PROFILE_FUNCTION();
		if(m_AnimationEditorPanel && m_AnimationEditorPanel->IsAnimationEditorOpen())
			m_AnimationEditorPanel->Update(ts);
	}

	void ContentBrowserPanel::InitPanels()
	{
        EG_PROFILE_FUNCTION();
        EG_PROFILE_FUNCTION();
		m_DeleteDirectoryPanel = CreateScope<DeleteDirectoryPanel>();
	}

	void ContentBrowserPanel::RenderFile(UUID key, const std::string& name, ResourceType type)
	{
        EG_PROFILE_FUNCTION();
        EG_PROFILE_FUNCTION();
		static float thumbnailSize = 128.0f;

		ImGui::PushID(name.c_str());
		auto& style = ImGui::GetStyle();

		DrawIcon(type);

		ShowFileMenu(key, type);

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("ContentBrowserPanel", &key, sizeof(int64_t));
			ImGui::EndDragDropSource();
		}

		ImGui::TextWrapped(name.c_str());

		ImGui::NextColumn();

		ImGui::PopID();
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		if (m_DeleteDirectoryPanel->IsShown())
			m_DeleteDirectoryPanel->OnImGuiRender();

		ImGui::Begin("Content Browser");

		ImGuiStyle& style = ImGui::GetStyle();
		auto btnColor = style.Colors[ImGuiCol_Button];
		auto btnColorHovered = style.Colors[ImGuiCol_ButtonHovered];
		auto btnColorActive = style.Colors[ImGuiCol_ButtonActive];

		ImGui::PushFont(static_cast<EditorLayer*>(Application::Get().GetFirstLayer())->m_PoppinsRegularFontBig);

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		if (m_CurrentDirectory != AssetDirectoryManager::getRootDirectoryUUID()) {

			drawList->ChannelsSplit(2);
			drawList->ChannelsSetCurrent(1);

			if (ImGui::ImageButton((ImTextureID)IconLoader::GetIcon(Icons::ContentBrowser_Arrow)->GetRendererID(), { 30, 30 }))
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
						int64_t uuid = *(int64_t*)payload->Data;
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
		if(ImGui::ImageButton((ImTextureID)IconLoader::GetIcon(Icons::ContentBrowser_Plus)->GetRendererID(), ImVec2(30,30)))
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
		ImGui::PopStyleColor(3);

		bool isClicked = false;
		if (ImGui::BeginPopup("CreateNewResource"))
		{
			if (ImGui::MenuItem("Create Folder"))
			{
				isClicked = true;
			}
			ImGui::EndPopup();
		}
		if (isClicked) {
			ImGui::OpenPopup("CreateNewDirectory");
			isClicked = false;
		}

		ShowCreateFolderPopup();

		ImGui::SameLine();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.f);
		
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 10.f));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 30.f);

		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 inputSize = ImVec2(300, 30);
		ImVec2 padd = ImGui::GetStyle().FramePadding;

		static char buffer[256];
		ImGui::InputTextWithHint("##Filter", "Search", buffer, 256);

		bool isHovered = ImGui::IsItemHovered();
		bool isActive = ImGui::IsItemActive();

		if (isHovered || isActive) {
			ImVec4 hoverColor = isHovered ? ImVec4(0.45f, 0.42f, 0.55f, 1.00f) : ImVec4(0.55f, 0.50f, 0.70f, 1.00f);
			ImColor borderColor = ImColor(hoverColor);

			float totalWidth = ImGui::CalcItemWidth();

			drawList->AddRect(pos, ImVec2(pos.x + totalWidth, pos.y + inputSize.y + padd.y), borderColor, 30.0f, 0, 2.0f);
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleVar(3);

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
			ImGui::PushID(directory);

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0));
			ImGui::ImageButton((ImTextureID)IconLoader::GetIcon(Icons::ContentBrowser_Directory)->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
			ImGui::PopStyleColor();

			ShowFolderMenu(directory);

			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("ContentBrowserPanel", &directory, sizeof(int64_t));
				ImGui::Text(name.c_str());
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserPanel"))
				{
					if (ResourceUtils::CanDrop(directory))
					{
						int64_t uuid = *(int64_t*)payload->Data;

						if (ResourceDatabase::FindResourceData(uuid))
							Commands::ExecuteMoveResourceCommand(uuid, directory);
						else
							Commands::ExecuteMoveDirectoryCommand(uuid, directory);
					}
				}
				ImGui::EndDragDropTarget();
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				UUID oldPath = m_CurrentDirectory;
				m_CurrentDirectory = directory;
				Commands::ExecuteRawValueCommand(&m_CurrentDirectory, oldPath, std::string("ContentBrowserPanel-Current Directory"), true);
			}

			ImGui::TextWrapped(name.c_str(), cellSize);

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		float size = thumbnailSize, offset = padding;

		ImGui::PopFont();
		ImGui::End();

		if (m_AnimationEditorPanel)
		{
			m_AnimationEditorPanel->OnImGuiRender();
			if (!m_AnimationEditorPanel->IsAnimationEditorOpen())
				m_AnimationEditorPanel = nullptr;
		}

	}


	void ContentBrowserPanel::ShowFolderMenu(UUID directory) {
		bool isDeleteClicked = false;
		bool isRenameClicked = false;
		if (ImGui::BeginPopupContextItem("DirectoryOptions"))
		{
			if (ImGui::MenuItem("Delete"))
			{
				isDeleteClicked = true;
			}

			if (ImGui::MenuItem("Rename"))
			{
				isRenameClicked = true;
			}

			ImGui::EndPopup();
		}

		if (isDeleteClicked) {
			ImGui::OpenPopup("DeleteFolder");
			isDeleteClicked = false;
		}
		if (isRenameClicked) {
			ImGui::OpenPopup("RenameFolder");
			isRenameClicked = false;
		}

		ShowFolderPopups(directory);
	}

	void ContentBrowserPanel::ShowFolderPopups(UUID directory) {
		ImGui::PushStyleColor(ImGuiCol_PopupBg, static_cast<EditorLayer*>(Application::Get().GetFirstLayer())->m_DarkShade);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.f, 10.f));

		ShowDeleteFolderPopup(directory);
		ShowRenameFolderPopup(directory);

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();
	}

	void ContentBrowserPanel::ShowDeleteFolderPopup(UUID directory) {
		ImGuiStyle& style = ImGui::GetStyle();

		if (ImGui::BeginPopupModal("DeleteFolder", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

			ImVec2 spacing = style.ItemSpacing;
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.f, 5.f));
			style.ItemSpacing = ImVec2(10.f, 10.f);

			ImGui::Text("Warning: You will not be able to restore files");

			if (ImGui::Button("Delete directory AND ALL FILES from disk"))
			{
				Commands::ExecuteDeleteDirectoryCommand(directory);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			style.ItemSpacing = spacing;
			ImGui::PopStyleVar(2);
			ImGui::EndPopup();
		}
	}

	void ContentBrowserPanel::ShowRenameFolderPopup(UUID directory) {
		ImGuiStyle& style = ImGui::GetStyle();

		if (ImGui::BeginPopupModal("RenameFolder", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

			ImVec2 spacing = style.ItemSpacing;
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.f, 5.f));
			style.ItemSpacing = ImVec2(10.f, 10.f);

			ImGui::Text("Enter new folder name");
			static char buffer[256] = "New folder name";
			ImGui::InputText("##FolderName", buffer, 256);
			if (ImGui::Button("Rename"))
			{
				Commands::ExecuteRenameDirectoryCommand(directory, buffer);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			style.ItemSpacing = spacing;
			ImGui::PopStyleVar(2);
			ImGui::EndPopup();
		}
	}

	void ContentBrowserPanel::ShowCreateFolderPopup() {
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::PushStyleColor(ImGuiCol_PopupBg, static_cast<EditorLayer*>(Application::Get().GetFirstLayer())->m_DarkShade);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.f, 10.f));

		if (ImGui::BeginPopupModal("CreateNewDirectory", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

			ImVec2 spacing = style.ItemSpacing;

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.f, 5.f));

			style.ItemSpacing = ImVec2(20.f, 10.f);

			ImVec4 buttonColor = style.Colors[ImGuiCol_Button];
			ImGui::Text("Enter new folder name");
			static char buffer[256] = "New Folder";

			ImGui::InputText("##FolderName", buffer, 256);

			style.ItemSpacing = ImVec2(10.f, 10.f);
			if (ImGui::Button("Create directory"))
			{
				Commands::ExecuteCreateDirectoryCommand(buffer, m_CurrentDirectory);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			style.ItemSpacing = spacing;

			ImGui::PopStyleVar(2);
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();
	}

	void ContentBrowserPanel::ShowFileMenu(UUID key, ResourceType type) {
		bool isDeleteClicked = false;
		bool isRenameClicked = false;
		AnimationResourceData* animData = (AnimationResourceData*)ResourceDatabase::GetResourceData(key);
		if (ImGui::BeginPopupContextItem("FileOptions"))
		{
			if (ImGui::MenuItem("Delete"))
			{
				isDeleteClicked = true;
			}

			if (ImGui::MenuItem("Rename"))
			{
				isRenameClicked = true;
			}
			if (animData && animData->Type == ResourceType::Animation) {
				if (ImGui::MenuItem("Open animation editor panel")) {
					m_AnimationEditorPanel = CreateScope<AnimationEditorPanel>(key);
				}
			}

			ImGui::EndPopup();
		}

		if (isDeleteClicked) {
			ImGui::OpenPopup("DeleteFile");
			isDeleteClicked = false;
		}

		if (isRenameClicked) {
			ImGui::OpenPopup("RenameFile");
			isRenameClicked = false;
		}

		ShowFilePopups(key, type);
	}

	void ContentBrowserPanel::ShowFilePopups(UUID key, ResourceType type) {
		ImGui::PushStyleColor(ImGuiCol_PopupBg, static_cast<EditorLayer*>(Application::Get().GetFirstLayer())->m_DarkShade);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.f, 10.f));

		ShowDeleteFilePopup(key, type);
		ShowRenameFilePopup(key, type);

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();
	}

	void ContentBrowserPanel::ShowRenameFilePopup(UUID key, ResourceType type) {
		ImGuiStyle& style = ImGui::GetStyle();

		if (ImGui::BeginPopupModal("RenameFile", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

			ImVec2 spacing = style.ItemSpacing;
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.f, 5.f));
			style.ItemSpacing = ImVec2(10.f, 10.f);

			ImGui::Text("Enter new resource name");
			static char buffer[256] = "New resource name";
			ImGui::InputText("##ResourceName", buffer, 256);

			if (ImGui::Button("Rename"))
			{
				Commands::ExecuteRenameResourceCommand(key, buffer);
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			style.ItemSpacing = spacing;
			ImGui::PopStyleVar(2);
			ImGui::EndPopup();
		}
	}

	void ContentBrowserPanel::ShowDeleteFilePopup(UUID key, ResourceType type) {
		ImGuiStyle& style = ImGui::GetStyle();

		if (ImGui::BeginPopupModal("DeleteFile", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

			ImVec2 spacing = style.ItemSpacing;
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.f, 5.f));
			style.ItemSpacing = ImVec2(10.f, 10.f);

			if (type == ResourceType::Font && key == Font::GetDefaultFontUUID())
			{
				ImGui::Text("Cannot delete default font");
			}
			else
			{
				ImGui::Text("Warning: Choose delete method");
				if (ImGui::Button("Delete file from project"))
				{
					Commands::ExecuteDeleteResourceCommand(key, type, false);
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Delete file from disk"))
				{
					Commands::ExecuteDeleteResourceCommand(key, type, true);
					ImGui::CloseCurrentPopup();
				}
			}

			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
			}
			style.ItemSpacing = spacing;
			ImGui::PopStyleVar(2);
			ImGui::EndPopup();
		}
	}

	void ContentBrowserPanel::DrawIcon(ResourceType type) {
		static float thumbnailSize = 100.0f;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		if (type == ResourceType::Script)
			ImGui::ImageButton((ImTextureID)IconLoader::GetIcon(Icons::ContentBrowser_FileScript)->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
		else if (type == ResourceType::Audio)
			ImGui::ImageButton((ImTextureID)IconLoader::GetIcon(Icons::ContentBrowser_FileAudio)->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
		else if (type == ResourceType::Animation)
			ImGui::ImageButton((ImTextureID)IconLoader::GetIcon(Icons::ContentBrowser_FileAnimation)->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
		else
			ImGui::ImageButton((ImTextureID)IconLoader::GetIcon(Icons::ContentBrowser_File)->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

		ImGui::PopStyleColor();
	}
}
