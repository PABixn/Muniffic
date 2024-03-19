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
		m_SpriteAtlasData.ResourcePath = path;
	}

	bool SpriteSheetPanel::InitSpriteSheetPanel(const std::filesystem::path& path)
	{
		m_SpriteAtlasData.ResourcePath = path;
		m_BasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Textures";
		bool resourceLoad = false;
		m_LoadedResource = new Resource();
		resourceLoad = resourceSystemLoad(path.string(), ResourceType::Image, m_LoadedResource);
		m_SpriteSize[0] = 10;
		m_SpriteSize[1] = 10;

		if (!resourceLoad) {
			return false;
		}
		else {
			m_OriginalResourcePath = path;
			m_ResourceData = new SpriteAtlasResourceData();
			((SpriteAtlasResourceData*)m_ResourceData)->ResourcePath = "SpriteAtlases";
			((SpriteAtlasResourceData*)m_ResourceData)->AtlasName = m_SpriteAtlasData.ResourcePath.stem().string();
			((SpriteAtlasResourceData*)m_ResourceData)->Extension = m_SpriteAtlasData.ResourcePath.extension().string();
			((SpriteAtlasResourceData*)m_ResourceData)->Height = ((ImageResourceData*)m_LoadedResource->Data)->height;
			((SpriteAtlasResourceData*)m_ResourceData)->Width = ((ImageResourceData*)m_LoadedResource->Data)->width;
			((SpriteAtlasResourceData*)m_ResourceData)->Channels = ((ImageResourceData*)m_LoadedResource->Data)->channelCount;
			((SpriteAtlasResourceData*)m_ResourceData)->Sprites = {};
			m_PreviewData = Texture2D::Create(path.string());

			m_TextureUUID = ResourceDatabase::AddResource(m_OriginalResourcePath, (void*)m_ResourceData, ResourceType::SpriteAtlas);
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
			for (int j = 0; j < (int)(m_PreviewData->GetHeight() / m_SpriteSize[1]); j++) {
				for (int i = 0; i < (int)(m_PreviewData->GetWidth() / m_SpriteSize[0]); i++)
				{
					if (i != 0)
						ImGui::SameLine();
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
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
						SubTextureResourceData* spriteResourceData = new SubTextureResourceData();
						((SubTextureResourceData*)spriteResourceData)->ResourcePath = "SubTextures";
						((SubTextureResourceData*)spriteResourceData)->SubTextureName = m_SpriteAtlasData.ResourcePath.stem().string() + "sprite" + std::to_string(spriteIndex++);
						((SubTextureResourceData*)spriteResourceData)->m_Texture = m_TextureUUID;
						((SubTextureResourceData*)spriteResourceData)->m_TexCoords[0] = glm::vec2(x, y);
						((SubTextureResourceData*)spriteResourceData)->m_TexCoords[1] = glm::vec2(x + m_SpriteSize[0], y);
						((SubTextureResourceData*)spriteResourceData)->m_TexCoords[2] = glm::vec2(x + m_SpriteSize[0], y + m_SpriteSize[1]);
						((SubTextureResourceData*)spriteResourceData)->m_TexCoords[3] = glm::vec2(x, y + m_SpriteSize[1]); 
						((SpriteAtlasResourceData*)m_ResourceData)->Sprites.push_back(ResourceDatabase::AddResource(m_OriginalResourcePath, spriteResourceData, ResourceType::SubTexture));
						delete spriteResourceData;
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
		m_SpriteAtlasData = SpriteAtlasResourceData();
		m_SpriteAtlasData.ResourcePath = "";
		m_SpriteAtlasData.AtlasName = "";
		m_SpriteAtlasData.Height = 0;
		m_SpriteAtlasData.Width = 0;
		m_SpriteAtlasData.Channels = 0;
		m_SpriteAtlasData.Extension = "png";
		m_BasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Textures";
		m_OriginalResourcePath = "";
		m_PreviewData = nullptr;
		m_SpriteSize[0] = 10;
		m_SpriteSize[1] = 10;
	}
}