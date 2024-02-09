#include "egpch.h"
#include "ImagePanel.h"
#include <imgui/imgui.h>
#include "Engine/Utils/PlatformUtils.h"
#include "Engine/Resources/ResourceSerializer.h"
#include "Engine/Project/Project.h"

namespace eg
{
	ImagePanel::ImagePanel(const std::filesystem::path& path)
	{
		m_TextureData.imagePath = path;
	}

	bool ImagePanel::InitImagePanel(const std::filesystem::path& path)
	{
		m_TextureData.imagePath = path;
		bool resourceLoad = false;
		m_LoadedResource = new Resource();
		resourceLoad = resourceSystemLoad(m_TextureData.imagePath.string(), ResourceType::Image, m_LoadedResource);
		

		if (!resourceLoad) {
			return false;
		}
		else {
			m_ResourceData = new TextureResourceData();
			((TextureResourceData*)m_ResourceData)->imagePath = m_TextureData.imagePath;
			((TextureResourceData*)m_ResourceData)->imageName = m_TextureData.imagePath.stem().string();
			m_DestinationPath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Textures";
			((TextureResourceData*)m_ResourceData)->height = ((ImageResourceData*)m_LoadedResource->Data)->height;
			((TextureResourceData*)m_ResourceData)->width = ((ImageResourceData*)m_LoadedResource->Data)->width;
			((TextureResourceData*)m_ResourceData)->channels = ((ImageResourceData*)m_LoadedResource->Data)->channelCount;
			((TextureResourceData*)m_ResourceData)->top = 0;
			((TextureResourceData*)m_ResourceData)->bottom = ((ImageResourceData*)m_LoadedResource->Data)->height;
			((TextureResourceData*)m_ResourceData)->left = 0;
			((TextureResourceData*)m_ResourceData)->right = ((ImageResourceData*)m_LoadedResource->Data)->width;
			m_PreviewData = Texture2D::Create("resources/icons/PlayButton.png");
		}
	}

	void ImagePanel::OnImGuiRender()
	{
		if (!m_ShowImagePanel)
			return;
		ImGui::Begin("Image");

		if (((TextureResourceData*)m_ResourceData)->imagePath.empty())
		{
			ImGui::Text("No image chosen");
		}
		else
		{
			ImGui::Image((void*)(intptr_t)m_PreviewData->GetRendererID(), ImVec2(200, 200));
			ImGui::Text("Image: %s", ((TextureResourceData*)m_ResourceData)->imagePath.string().c_str());
			if (ImGui::Button("Change Image"))
			{
				std::string path = FileDialogs::OpenFile("Image (*.png)\0*.png\0");
				if (!path.empty())
					((TextureResourceData*)m_ResourceData)->imagePath = path;
			}
			ImGui::Text("Destination: %s", m_DestinationPath.string().c_str());
			if (ImGui::Button("Change Destination"))
			{
				std::string path = FileDialogs::OpenFile("");
				if (!path.empty())
					((TextureResourceData*)m_ResourceData)->imagePath = path;
			}

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, ((TextureResourceData*)m_ResourceData)->imageName.c_str(), sizeof(buffer));


			ImGui::Text("Original Height: %d", ((TextureResourceData*)m_ResourceData)->height);
			ImGui::Text("Original Width: %d", ((TextureResourceData*)m_ResourceData)->width);
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				((TextureResourceData*)m_ResourceData)->imageName = std::string(buffer);
			}
			//ImGui::Text("Image Name: %s", ((TextureResourceData*)m_ResourceData)->imageName.c_str());
			ImGui::DragInt("Width", &((TextureResourceData*)m_ResourceData)->width);
			ImGui::DragInt("Height", &((TextureResourceData*)m_ResourceData)->height);
			ImGui::DragInt("Top", &((TextureResourceData*)m_ResourceData)->top);
			ImGui::DragInt("Bottom", &((TextureResourceData*)m_ResourceData)->bottom);
			ImGui::DragInt("Left", &((TextureResourceData*)m_ResourceData)->left);
			ImGui::DragInt("Right", &((TextureResourceData*)m_ResourceData)->right);
			ImGui::DragInt("Channels", &((TextureResourceData*)m_ResourceData)->channels, 1, 1, 4);
			//Todo:: Add image preview, ability to change the image and choose the path to the resource

			//ImGui::Image((void*)(intptr_t)textureID, ImVec2(200, 200));
			if (ImGui::Button("Save"))
			{
				ResourceSerializer serializer;
				std::string name = ((TextureResourceData*)m_ResourceData)->imageName;
				
				serializer.SerializeTextureResource(m_DestinationPath /(name + ".mnmeta"), *((TextureResourceData*)m_ResourceData));
				name += ((TextureResourceData*)m_ResourceData)->imagePath.extension().string();
				std::filesystem::path finalPath = m_DestinationPath / name;
				if(finalPath != ((TextureResourceData*)m_ResourceData)->imagePath)
					std::filesystem::copy(((TextureResourceData*)m_ResourceData)->imagePath, finalPath, std::filesystem::copy_options::overwrite_existing);
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
		m_TextureData.imagePath = "";
		m_TextureData.imageName = "";
		m_TextureData.height = 0;
		m_TextureData.width = 0;
		m_TextureData.channels = 0;
		m_TextureData.top = 0;
		m_TextureData.bottom = 0;
		m_TextureData.left = 0;
		m_TextureData.right = 0;
		m_DestinationPath = "";
		m_PreviewData = nullptr;
	}
}