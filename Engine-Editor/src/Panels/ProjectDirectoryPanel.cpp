#include "egpch.h"
#include "ProjectDirectoryPanel.h"
#include <Imgui/imgui.h>
#include <cstring>
#include "Engine/Resources/ResourceUtils.h"
#include "../EditorLayer.h"

namespace eg {
	ProjectDirectoryPanel::ProjectDirectoryPanel() {
	}
	uint64_t getPathID(const std::string path) {
		static std::map<std::string, uint64_t> paths;
		try
		{
			return paths.at(path);
		}
		catch (const std::out_of_range&)
		{
			uint64_t id = paths.size() + 69;
			paths.insert({ path, id });
			return id;
		}
	}

	void ProjectDirectoryPanel::OnImGuiRender() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		ImGui::Begin("Project Directory");
		ImGui::PopStyleVar();
		auto s = Project::GetProjectDirectory();
		drawDirectoryTreeNode(s);
		ImGui::End();
	}
	void ProjectDirectoryPanel::drawDirectoryTreeNode(const std::filesystem::path& path) {

		auto t = ImGui::GetItemID();
		auto i = getPathID(path.string());
		ImGui::PushID(i);
		t = ImGui::GetItemID();
		std::string tag = path.filename().string();
		//bool is_open;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanFullWidth;
		ImTextureID textureID;
		if (std::filesystem::is_directory(path)) {
			flags |= ImGuiTreeNodeFlags_DirectoryEntity;
			flags |= ImGuiTreeNodeFlags_EntityWithChildren;
			textureID = (ImTextureID)(m_ContentBrowserPanel->m_DirectoryIcon)->GetRendererID();
		}
		else if (std::filesystem::is_regular_file(path)) {
			flags |= ImGuiTreeNodeFlags_FileEntity;
			textureID = (ImTextureID)(m_ContentBrowserPanel->m_FileIcon)->GetRendererID();
		}
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3.f, 5.f)); 
		bool is_open = ImGui::CustomTreeNodeWithPicEx((void*)(getPathID(path.string())), flags, tag.c_str(), textureID);
		t = ImGui::GetItemID();
		ImGui::PopStyleVar();
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			ResourceType type = ResourceUtils::GetResourceTypeByExtension(path.extension().string());
			if (type == ResourceType::Image) {
				auto* s = (*(dynamic_cast<EditorLayer*>(Application::Get().GetFirstLayer()))).GetSceneHierarchyPanel();
				(*s).SetPreviewAbsoluteImagePath(std::filesystem::path(path));
			}
		}
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			ResourceType type = ResourceUtils::GetResourceTypeByExtension(path.extension().string());
			if (type == ResourceType::Image) {
				auto* s = (*(dynamic_cast<EditorLayer*>(Application::Get().GetFirstLayer()))).GetSceneHierarchyPanel();
				(*s).SetPreviewAbsoluteImagePath(std::filesystem::path(path));
			}
		}
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
			if (std::filesystem::is_directory(path)) {
				UUID oldPath = (m_ContentBrowserPanel->m_CurrentDirectory);
				//ResourceDatabase::
				//(m_ContentBrowserPanel->m_CurrentDirectory) = ;
				//Commands::ExecuteRawValueCommand(&(m_ContentBrowserPanel->m_CurrentDirectory), oldPath, std::string("ContentBrowserPanel-Current Directory"), true);
			}
		}
		ImGui::PopID();
		//draw subfolders
		if (is_open)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			if (std::filesystem::is_directory(path))
			{
				for (auto& directoryEntry : std::filesystem::directory_iterator(path)) {
					drawDirectoryTreeNode(directoryEntry.path());
				}
			}
			ImGui::TreePop();
		}
	}
}