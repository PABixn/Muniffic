#include "egpch.h"
#include "SpriteSheetPanel.h"
#include "Engine/Resources/ResourceDatabase.h"
#include "Engine/Utils/PlatformUtils.h"
#include "Engine/Resources/ResourceSerializer.h"
#include "Engine/Project/Project.h"

#include <imgui/imgui.h>

namespace eg
{

	SpriteSheetPanel::SpriteSheetPanel(const std::filesystem::path& path)
	{
		m_TextureData.ResourcePath = path;
		m_Sprites = {};
	}

	bool SpriteSheetPanel::InitSpriteSheetPanel(const std::filesystem::path& path)
	{
		m_TextureData.ResourcePath = path;
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
			((TextureResourceData*)m_ResourceData)->ResourcePath = "Textures";
			((TextureResourceData*)m_ResourceData)->ImageName = m_TextureData.ResourcePath.stem().string();
			((TextureResourceData*)m_ResourceData)->Extension = m_TextureData.ResourcePath.extension().string();
			((TextureResourceData*)m_ResourceData)->Height = ((ImageResourceData*)m_LoadedResource->Data)->height;
			((TextureResourceData*)m_ResourceData)->Width = ((ImageResourceData*)m_LoadedResource->Data)->width;
			((TextureResourceData*)m_ResourceData)->Channels = ((ImageResourceData*)m_LoadedResource->Data)->channelCount;
			m_PreviewData = Texture2D::Create(path.string());
		}
	}

	void SpriteSheetPanel::OnImGuiRender()
	{
		if (!m_ShowSpriteSheetPanel)
			return;
		ImGui::SetNextWindowSize(ImVec2(750,700));
		ImGui::Begin("SpriteSheet");

		if (((TextureResourceData*)m_ResourceData)->ResourcePath.empty())
		{
			ImGui::Text("No image chosen");
		}
		else
		{
			//ImGui::Image((void*)(intptr_t)m_PreviewData->GetRendererID(), ImVec2(200, 200));
			for (int j = 0; j < (int)(m_PreviewData->GetHeight() / m_SpriteSize[1]); j++) {
				for (int i = 0; i < (int)(m_PreviewData->GetWidth() / m_SpriteSize[0]); i++)
				{
					if (i != 0)
						ImGui::SameLine();
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
					//bool isSelected = i >= m_Column && i < m_Column + m_ColumnCount && j >= m_Row && j < m_Row + m_RowCount;
					ImVec4 borderColor = ImVec4{ 0.1f, 0.1f, 0.1f, 0.1f };
					ImGui::PushStyleColor(ImGuiCol_Border, borderColor);
					ImGui::Image(
						(void*)(intptr_t)m_PreviewData->GetRendererID(),
						ImVec2(200 / (int)(m_PreviewData->GetWidth() / m_SpriteSize[0]),
							200 / (int)(m_PreviewData->GetHeight() / m_SpriteSize[1])),
						{ i * (float)m_SpriteSize[0] / (float)((TextureResourceData*)m_ResourceData)->Width,
							j * (float)m_SpriteSize[1] / (float)((TextureResourceData*)m_ResourceData)->Height },
						{ (i + 1) * (float)m_SpriteSize[0] / (float)((TextureResourceData*)m_ResourceData)->Width,
							(j + 1) * (float)m_SpriteSize[1] / (float)((TextureResourceData*)m_ResourceData)->Height });
					ImGui::PopStyleColor();
					ImGui::PopStyleVar();
				}
			}
			ImGui::Text("Image: %s", (((TextureResourceData*)m_ResourceData)->ResourcePath / std::filesystem::path(((TextureResourceData*)m_ResourceData)->ImageName + ((TextureResourceData*)m_ResourceData)->Extension)).string().c_str());
			ImGui::Text("Destination: %s", (m_BasePath / std::filesystem::path(((TextureResourceData*)m_ResourceData)->ImageName + ((TextureResourceData*)m_ResourceData)->Extension)).string().c_str());
			ImGui::Text("Create sprites:");
			ImGui::Text("Original Height: %d", ((TextureResourceData*)m_ResourceData)->Height);
			ImGui::Text("Original Width: %d", ((TextureResourceData*)m_ResourceData)->Width);
			ImGui::Text("Width: ");
			ImGui::SameLine();
			ImGui::SliderInt("##width", &m_SpriteSize[0], 10, ((TextureResourceData*)m_ResourceData)->Width);
			ImGui::Text("Height: ");
			ImGui::SameLine();
			ImGui::SliderInt("##height", &m_SpriteSize[1], 10, ((TextureResourceData*)m_ResourceData)->Height);

			if (ImGui::Button("Save"))
			{
				int spriteIndex = 0;
			
				for (int x = 0; x < ((TextureResourceData*)m_ResourceData)->Width; x += m_SpriteSize[0])
				{
					for (int y = 0; y < ((TextureResourceData*)m_ResourceData)->Height; y += m_SpriteSize[1])
					{
						SubTexture2D sprite = *SubTexture2D::CreateFromCoords(m_PreviewData, { x, y }, { m_SpriteSize[0], m_SpriteSize[1] });
						
						TextureResourceData* spriteResourceData = new TextureResourceData();
						((TextureResourceData*)spriteResourceData)->ResourcePath = "Textures";
						((TextureResourceData*)spriteResourceData)->ImageName = m_TextureData.ResourcePath.stem().string() + "sprite" + std::to_string(spriteIndex++);
						((TextureResourceData*)spriteResourceData)->Extension = m_TextureData.ResourcePath.extension().string();
						((TextureResourceData*)spriteResourceData)->Height = m_SpriteSize[0];
						((TextureResourceData*)spriteResourceData)->Width = m_SpriteSize[1];
						((TextureResourceData*)spriteResourceData)->Channels = ((ImageResourceData*)m_LoadedResource->Data)->channelCount;
						
						ResourceDatabase::AddResource(sprite.GetPath(), spriteResourceData, ResourceType::Image);
					}
				}
				m_ShowSpriteSheetPanel = false;
			}

			if (ImGui::Button("Cancel"))
			{
				m_ShowSpriteSheetPanel = false;
			}
		}
		ImGui::End();
	}
	void SpriteSheetPanel::ResetData()
	{
		m_TextureData = TextureResourceData();
		m_TextureData.ResourcePath = "";
		m_TextureData.ImageName = "";
		m_TextureData.Height = 0;
		m_TextureData.Width = 0;
		m_TextureData.Channels = 0;
		m_TextureData.Extension = "png";
		m_BasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Textures";
		m_OriginalResourcePath = "";
		m_PreviewData = nullptr;
	}
}