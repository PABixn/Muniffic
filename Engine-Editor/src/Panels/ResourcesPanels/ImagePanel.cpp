#include "egpch.h"
#include "ImagePanel.h"
#include "Engine/Resources/ResourceDatabase.h"
#include "Engine/Utils/PlatformUtils.h"
#include "Engine/Resources/ResourceSerializer.h"
#include "Engine/Project/Project.h"

#include <imgui/imgui.h>
#include <Engine/Resources/AssetDirectoryManager.h>

namespace eg
{

	constexpr const ImU32 BASEINPUTCOLOR = IM_COL32(0x40, 0x36, 0x59, 0xff); // 0x403659FF;
	constexpr const ImU32 HOVEREDCOLOR = IM_COL32(0x5a, 0x4a, 0xa7, 0xff); // 0x5A4A7DFF;
	constexpr const ImU32 ACTIVEINPUTCOLOR = IM_COL32(0x83, 0x79, 0x93, 0xff); // 0x83799EFF;
	constexpr const ImU32 SELECTEDFRAMECOLOR = IM_COL32(0xc6, 0xbb, 0xe2, 0xff); // 0xC6BBE2FF;
	constexpr const ImU32 BGCOLOR = IM_COL32(0x28, 0x1f, 0x3a, 0xff); // 0x281F3AFF

	constexpr const int ROUNDING = 10;

	ImagePanel::ImagePanel(const std::filesystem::path& path)
	{
        EG_PROFILE_FUNCTION();
		m_TexturePath = path;
	}

	bool ImagePanel::OpenImagePanel(const std::filesystem::path& path)
	{
        EG_PROFILE_FUNCTION();
		m_TexturePath = path;
		/*m_TextureData.ResourcePath = path;*/
		m_BasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Textures";
		bool resourceLoad = false;
		m_LoadedResource = new Resource();
		resourceLoad = resourceSystemLoad(path.string(), ResourceType::Image, m_LoadedResource);

		if (!resourceLoad) {
			return false;
		}
		m_OriginalResourcePath = path;
		m_ResourceData = new TextureResourceData();
		((TextureResourceData*)m_ResourceData)->ParentDirectory = AssetDirectoryManager::GetRootAssetTypeDirectory(ResourceType::Image);
		((TextureResourceData*)m_ResourceData)->ResourceName = m_TexturePath.stem().string();
		((TextureResourceData*)m_ResourceData)->Extension = m_TexturePath.extension().string();
		((TextureResourceData*)m_ResourceData)->Height = ((ImageResourceData*)m_LoadedResource->Data)->height;
		((TextureResourceData*)m_ResourceData)->Width = ((ImageResourceData*)m_LoadedResource->Data)->width;
		((TextureResourceData*)m_ResourceData)->Channels = ((ImageResourceData*)m_LoadedResource->Data)->channelCount;
		m_PreviewData = Texture2D::Create(path.string());
		if (m_PreviewData == nullptr)
			return false;
		m_ShowImagePanel = true;
		return true;
	}

	void ImagePanel::OnImGuiRender()
	{
        EG_PROFILE_FUNCTION();
		if (!m_ShowImagePanel)
			return;
                ImGui::PushStyleColor(ImGuiCol_WindowBg, BGCOLOR);
                ImGui::PushStyleColor(ImGuiCol_Tab, BGCOLOR);
                ImGui::PushStyleColor(ImGuiCol_TabHovered, HOVEREDCOLOR);
                ImGui::PushStyleColor(ImGuiCol_TabActive, ACTIVEINPUTCOLOR);
                ImGui::PushStyleColor(ImGuiCol_TabUnfocusedActive, ACTIVEINPUTCOLOR);
                ImGui::PushStyleColor(ImGuiCol_TabUnfocused, BGCOLOR);
                ImGui::PushStyleColor(ImGuiCol_TitleBg, BGCOLOR);
		ImGui::Begin("Image");
                ImGui::PushStyleColor(ImGuiCol_FrameBg, BASEINPUTCOLOR);
                ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, HOVEREDCOLOR);
                ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ACTIVEINPUTCOLOR);

                ImGui::PushStyleColor(ImGuiCol_Button, BASEINPUTCOLOR);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HOVEREDCOLOR);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ACTIVEINPUTCOLOR);

                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ROUNDING);
		if (((TextureResourceData*)m_ResourceData)->ParentDirectory == 0)
		{
			ImGui::Text("No image chosen");
		}
		else
		{

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, ((TextureResourceData*)m_ResourceData)->ResourceName.c_str(), sizeof(buffer));

			if(ImGui::InputText("##ResourceName", buffer, sizeof(buffer)))
				((TextureResourceData*)m_ResourceData)->ResourceName = std::string(buffer);
			ImGui::Text("Original Height: %d", ((TextureResourceData*)m_ResourceData)->Height);
			ImGui::Text("Original Width: %d", ((TextureResourceData*)m_ResourceData)->Width);
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				((TextureResourceData*)m_ResourceData)->ResourceName = std::string(buffer);
			}
			ImGui::Text("Image Name: %s", ((TextureResourceData*)m_ResourceData)->ResourceName.c_str());
			//Todo:: Add image preview, ability to change the image and choose the path to the resource

			ImGui::Image((void*)(intptr_t)m_PreviewData->GetRendererID(), ImVec2(200, 200), ImVec2(0,1), ImVec2(1, 0));
			if (ImGui::Button("Save"))
			{
				ResourceDatabase::AddResource(m_OriginalResourcePath, m_ResourceData, ResourceType::Image);
				m_ShowImagePanel = false;
			}

			if (ImGui::Button("Cancel"))
			{
				m_ShowImagePanel = false;
			}
		}
        ImGui::PopStyleVar();
		ImGui::PopStyleColor(6);
		ImGui::End();
		ImGui::PopStyleColor(7);
	}
	void ImagePanel::ResetData()
	{
        EG_PROFILE_FUNCTION();
		m_TextureData = TextureResourceData();
		m_TextureData.ParentDirectory = 0;
		m_TextureData.ResourceName = "";
		m_TextureData.Height = 0;
		m_TextureData.Width = 0;
		m_TextureData.Channels = 0;
		m_TextureData.Extension = "png";
		m_BasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Textures";
		m_OriginalResourcePath = "";
		m_PreviewData = nullptr;
	}
}
