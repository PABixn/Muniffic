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

	void ContentBrowserPanel::DrawCenteredText(const std::string& text, const float& cellSize) {
		auto textWidth = ImGui::CalcTextSize(text.c_str()).x;
		auto CursorX = ImGui::GetCursorPosX();
		float offset = (cellSize - textWidth) * 0.43f;
		if (textWidth < cellSize) {
			ImGui::SetCursorPosX(CursorX + offset);
			ImGui::TextWrapped(text.c_str());
		}
		else {
			int a = ceil(textWidth / cellSize) + 1;
			for (int i = 0; i < a; i++) {
				if (i >= 2)
				{
					auto r = text.substr((text.length() / a) * i, (text.length() / a) - 3);
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

	void ContentBrowserPanel::InitPanels()
	{
		m_DeleteFilePanel = CreateScope<DeleteFilePanel>();
		m_RenameFolderPanel = CreateScope<RenameFolderPanel>();
		m_DeleteDirectoryPanel = CreateScope<DeleteDirectoryPanel>();
		m_RenameResourcePanel = CreateScope<RenameResourcePanel>();
		m_CreateDirectoryPanel = CreateScope<CreateDirectoryPanel>();
		m_ContentBrowserRightClickPanel = CreateRef<ContentBrowserRightClickPanel>();
	}

	void ContentBrowserPanel::RenderFile(UUID key, const std::string& name, ResourceType type)
	{
		static float thumbnailSize = 128.0f;

		ImGui::PushID(name.c_str());

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::ImageButton((ImTextureID)m_FileIcon->GetRendererID(), { thumbnailSize, thumbnailSize });

		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("ContentBrowserPanel", &key, sizeof(int64_t));
			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
			if (type == ResourceType::Image)
			{
				auto* s = (*(dynamic_cast<EditorLayer*>(Application::Get().GetFirstLayer()))).GetSceneHierarchyPanel();
				(*s).SetPreviewAbsoluteImagePath(std::filesystem::path(ResourceDatabase::GetResourcePath(key)));
			}
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
			ImGui::OpenPopup("FileOptions");
		}
		/*if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			EditorLayer* e = (EditorLayer*)Application::Get().GetFirstLayer();
			e->GetAnimationEditorPanel()->OpenAnimationEditorPanel(key);
			m_ContentBrowserRightClickPanel->OpenContentBrowserRightClickPanel(key,ImGui::GetMousePos().x, ImGui::GetMousePos().y);
		}*/
		DrawCenteredText(name.c_str(), thumbnailSize);
		AnimationResourceData* animData = (AnimationResourceData*)ResourceDatabase::GetResourceData(key);
		if (ImGui::BeginPopupContextItem("FileOptions"))
		{
			if (ImGui::MenuItem("Delete"))
			{
				m_DeleteFilePanel->ShowWindow(key, type);
				ImGui::NextColumn();
				ImGui::EndPopup();
				ImGui::PopStyleColor();
				ImGui::PopID();
				return;
			}
			if (animData->Type == ResourceType::Animation) {
				if (ImGui::MenuItem("Open animation editor panel")) {
					EditorLayer* e = (EditorLayer*)Application::Get().GetFirstLayer();
					e->GetAnimationEditorPanel()->OpenAnimationEditorPanel(key);
					ImGui::EndPopup();
					ImGui::PopStyleColor();
					ImGui::PopID();
					return;
				}
			}
			ImGui::EndPopup();
		}
		ImGui::PopStyleColor();
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

		if (ImGui::Button("+"))
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

		for (UUID asset : AssetDirectoryManager::getAssets(m_CurrentDirectory))
		{
			//AnimationResourceData* animData = (AnimationResourceData*)ResourceDatabase::GetResourceData(asset);
			//animData->
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

			Ref<Texture2D> icon = m_DirectoryIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetRendererID(), { thumbnailSize, thumbnailSize }, { 0, 0 }, { 1, 1 });

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

				if (ImGui::MenuItem("Rename"))
				{
					m_RenameFolderPanel->ShowWindow(directory);
				}

				ImGui::EndPopup();
			}

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

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
				UUID oldPath = m_CurrentDirectory;
				m_CurrentDirectory = directory;
				Commands::ExecuteRawValueCommand(&m_CurrentDirectory, oldPath, std::string("ContentBrowserPanel-Current Directory"), true);
			}

			DrawCenteredText(name.c_str(), cellSize);

			ImGui::NextColumn();

			ImGui::PopID();
		}

		ImGui::Columns(1);

		float size = thumbnailSize, offset = padding;

		if (ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512))
			Commands::ExecuteRawValueCommand(&thumbnailSize, size, std::string("ContentBrowserPanel-Thumbnail Size"));

		if (ImGui::SliderFloat("Padding", &padding, 0, 32))
			Commands::ExecuteRawValueCommand(&padding, offset, std::string("ContentBrowserPanel-Padding"));

		ImGui::End();
		EditorLayer* e = (EditorLayer*)Application::Get().GetFirstLayer();
		e->GetAnimationEditorPanel()->OnImGuiRender();
		//m_ContentBrowserRightClickPanel->OnImGuiRender();
	}

}