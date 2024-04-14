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

	ImagePanel::ImagePanel(const std::filesystem::path& path)
	{
		m_TexturePath = path;
	}

	bool ImagePanel::OpenImagePanel(const std::filesystem::path& path)
	{
		m_TexturePath = path;
		/*m_TextureData.ResourcePath = path;*/
		m_BasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Textures";
		bool resourceLoad = false;
		m_LoadedResource = new Resource();
		resourceLoad = resourceSystemLoad(path.string(), ResourceType::Image, m_LoadedResource);
		
		if (!resourceLoad) {
			return false;
		}
		else {
			m_OriginalResourcePath = path;
			m_ResourceData = new TextureResourceData();
			((TextureResourceData*)m_ResourceData)->ParentDirectory = AssetDirectoryManager::GetRootAssetTypeDirectory(ResourceType::Image);
			((TextureResourceData*)m_ResourceData)->ResourceName = m_TexturePath.stem().string();
			((TextureResourceData*)m_ResourceData)->Extension = m_TexturePath.extension().string();
			((TextureResourceData*)m_ResourceData)->Height = ((ImageResourceData*)m_LoadedResource->Data)->height;
			((TextureResourceData*)m_ResourceData)->Width = ((ImageResourceData*)m_LoadedResource->Data)->width;
			((TextureResourceData*)m_ResourceData)->Channels = ((ImageResourceData*)m_LoadedResource->Data)->channelCount;
			m_PreviewData = Texture2D::Create(path.string());
		}
	}

	void ImagePanel::OnImGuiRender()
	{
		if (!m_ShowImagePanel)
			return;
		ImGui::Begin("Image");

		if (((TextureResourceData*)m_ResourceData)->ParentDirectory == 0)
		{
			ImGui::Text("No image chosen");
		}
		else
		{
			/*ImGui::Image((void*)(intptr_t)m_PreviewData->GetRendererID(), ImVec2(200, 200));
			ImGui::Text("Image: %s", (((TextureResourceData*)m_ResourceData)->ResourcePath / std::filesystem::path(((TextureResourceData*)m_ResourceData)->ResourceName + ((TextureResourceData*)m_ResourceData)->Extension)).string().c_str());
			if (ImGui::Button("Change Image"))
			{
				std::string path = FileDialogs::OpenFile("Image (*.png)\0*.png\0");
				if (!path.empty())
					((TextureResourceData*)m_ResourceData)->ResourcePath = path;
			}
			ImGui::Text("Destination: %s", (m_BasePath / std::filesystem::path(((TextureResourceData*)m_ResourceData)->ResourceName + ((TextureResourceData*)m_ResourceData)->Extension)).string().c_str());
			if (ImGui::Button("Change Destination"))
			{
				std::string path = FileDialogs::OpenFile("");
				if (!path.empty())
					((TextureResourceData*)m_ResourceData)->ResourcePath = path;
			}*/

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
			ImGui::DragInt("Width", &((TextureResourceData*)m_ResourceData)->Width);
			ImGui::DragInt("Height", &((TextureResourceData*)m_ResourceData)->Height);
			//ImGui::DragInt("Top", &((TextureResourceData*)m_ResourceData)->Top);
			//ImGui::DragInt("Bottom", &((TextureResourceData*)m_ResourceData)->Bottom);
			//ImGui::DragInt("Left", &((TextureResourceData*)m_ResourceData)->Left);
			//ImGui::DragInt("Right", &((TextureResourceData*)m_ResourceData)->Right);
			//ImGui::DragInt("Channels", &((TextureResourceData*)m_ResourceData)->Channels, 1, 1, 4);
			//Todo:: Add image preview, ability to change the image and choose the path to the resource

			//ImGui::Image((void*)(intptr_t)textureID, ImVec2(200, 200));
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
		ImGui::End();
	}
	void ImagePanel::ResetData()
	{
		m_TextureData = TextureResourceData();
		m_TextureData.ParentDirectory = 0;
		m_TextureData.ResourceName = "";
		m_TextureData.Height = 0;
		m_TextureData.Width = 0;
		m_TextureData.Channels = 0;
		//m_TextureData.Top = 0;
		//m_TextureData.Bottom = 0;
		//m_TextureData.Left = 0;
		//m_TextureData.Right = 0;
		m_TextureData.Extension = "png";
		m_BasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Textures";
		m_OriginalResourcePath = "";
		m_PreviewData = nullptr;
	}
}