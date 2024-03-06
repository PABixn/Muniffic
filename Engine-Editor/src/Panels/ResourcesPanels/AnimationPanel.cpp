#include "egpch.h"
#include "AnimationPanel.h"
#include "Engine/Resources/ResourceSerializer.h"
#include "Engine/Resources/ResourceDatabase.h"

#include "Imgui/imgui.h"
namespace eg {
	AnimationPanel::AnimationPanel(const std::filesystem::path& path)
	{
		InitAnimationPanel(path);
	}

	bool AnimationPanel::InitAnimationPanel(const std::filesystem::path& path)
	{
		std::filesystem::path textureBasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Textures";
		bool resourceLoad = false;
		m_LoadedResource = new Resource();
		resourceLoad = resourceSystemLoad(path.string(), ResourceType::Image, m_LoadedResource);

		if (!resourceLoad) {
			return false;
		}
		m_OriginalResourcePath = path;
		m_TextureData =
		m_TextureData = new TextureResourceData();
		(m_TextureData)->ResourcePath = "Textures";
		((TextureResourceData*)m_TextureData)->ImageName = m_LoadedResource->Path.stem().string();
		((TextureResourceData*)m_TextureData)->Extension = m_LoadedResource->Path.extension().string();
		((TextureResourceData*)m_TextureData)->Height = ((ImageResourceData*)m_LoadedResource->Data)->height;
		((TextureResourceData*)m_TextureData)->Width = ((ImageResourceData*)m_LoadedResource->Data)->width;
		((TextureResourceData*)m_TextureData)->Channels = ((ImageResourceData*)m_LoadedResource->Data)->channelCount;
		m_PreviewOriginImage = Texture2D::Create(path.string());
		std::vector<Ref<SubTexture2D>> frames;
		frames.push_back(SubTexture2D::Create(m_PreviewOriginImage, { 0, 0 }, { 1, 1 }));
		m_PreviewData = Animation::Create(frames);
	}

	void AnimationPanel::SetFrames()
	{
		m_PreviewData->ClearFrames();
		for (int i = m_Column; i < m_Column + m_ColumnCount; i++)
			for (int j = m_Row; j < m_Row + m_RowCount; j++)
				m_PreviewData->AddFrame(SubTexture2D::Create(m_PreviewOriginImage, { i * m_FrameWidth, j * m_FrameHeight }, { (i + 1) * m_FrameWidth, (j + 1) * m_FrameHeight }));
	}

	void AnimationPanel::OnImGuiRender()
	{
		if (m_ShowImagePanel)
		{
			ImGui::Begin("Animation Preview");
			if (ImGui::DragInt("Frame Width: %d", &m_FrameWidth, 1.0f, 0, m_PreviewOriginImage->GetWidth()))
			{
				if (m_ColumnCount > (int)(m_PreviewOriginImage->GetWidth() / m_FrameWidth) - m_Column)
				{
					m_ColumnCount = (int)(m_PreviewOriginImage->GetWidth() / m_FrameWidth) - m_Column;
					SetFrames();
				}
			}
			if (ImGui::DragInt("Frame Height: %d", &m_FrameHeight, 1.0f, 0, m_PreviewOriginImage->GetHeight()))
			{
				if (m_RowCount > (int)(m_PreviewOriginImage->GetHeight() / m_FrameHeight) - m_Row)
				{
					m_RowCount = (int)(m_PreviewOriginImage->GetHeight() / m_FrameHeight) - m_Row;
					SetFrames();
				}
			}
			if (ImGui::DragInt("Column: %d", &m_Column, 0.0f, 0, (int)(m_PreviewOriginImage->GetWidth() / m_FrameWidth)))
			{
				if(m_ColumnCount > (int)(m_PreviewOriginImage->GetWidth() / m_FrameWidth) - m_Column)
					m_ColumnCount = (int)(m_PreviewOriginImage->GetWidth() / m_FrameWidth) - m_Column;
				SetFrames();
			}
			if (ImGui::DragInt("Row: %d", &m_Row, 1.0f, 0, (int)(m_PreviewOriginImage->GetHeight() / m_FrameWidth)))
			{
				if (m_RowCount > (int)(m_PreviewOriginImage->GetHeight() / m_FrameHeight) - m_Row)
					m_RowCount = (int)(m_PreviewOriginImage->GetHeight() / m_FrameHeight) - m_Row;
				SetFrames();
			}
			if(ImGui::DragInt("Column Count: %d", &m_ColumnCount, 1.0f, 0, m_Column - (int)(m_PreviewOriginImage->GetWidth() / m_FrameWidth)))
			{
				SetFrames();
			}
			if (ImGui::DragInt("Row Count: %d", &m_RowCount, 1.0f, 0, m_Row - (int)(m_PreviewOriginImage->GetHeight() / m_FrameHeight)))
			{
				SetFrames();
			}
			ImGui::Text("Image Preview:");
			ImGui::Image((void*)m_PreviewOriginImage->GetRendererID(), ImVec2(128, 128));
			ImGui::Checkbox("Play", m_PreviewData->IsPlayingPtr());
			ImGui::DragFloat("Frame Rate: %f", m_PreviewData->GetFrameRatePtr(), 1.0f, 0.0f, 500);
			ImGui::Checkbox("Loop", m_PreviewData->IsLoopingPtr());
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, m_ResourceData->AnimationName.c_str(), sizeof(buffer));

			//if (ImGui::InputText("##ImageName", buffer, sizeof(buffer)))
				//m_ResourceData->AnimationName = std::string(buffer);
			ImGui::Text("Animation: %s", m_ResourceData->AnimationName);
			ImGui::Image((void*)m_PreviewData->GetFrame()->GetTexture()->GetRendererID(), ImVec2(128, 128), { m_PreviewData->GetFrame()->GetMin().x , m_PreviewData->GetFrame()->GetMin().y}, { m_PreviewData->GetFrame()->GetMax().x , m_PreviewData->GetFrame()->GetMax().y });
			if (ImGui::Button("Save"))
			{
				for (int i = 0; i < m_PreviewData->GetFrameCount(); i++)
				{
					TextureResourceData* data = new TextureResourceData();
					data->ResourcePath = "Textures";
					data->ImageName = m_ResourceData->AnimationName + std::to_string(i);
					data->Extension = ".png";
					data->Height = m_PreviewData->GetFrame(i)->GetTexture()->GetHeight();
					data->Width = m_PreviewData->GetFrame(i)->GetTexture()->GetWidth();
					data->Channels = ((ImageResourceData*)m_LoadedResource->Data)->channelCount;
					data->m_TexCoords[0] = m_PreviewData->GetFrame(i)->GetMin();
					data->m_TexCoords[1] = { m_PreviewData->GetFrame(i)->GetMax().x, m_PreviewData->GetFrame(i)->GetMin().y };
					data->m_TexCoords[2] = m_PreviewData->GetFrame(i)->GetMax();
					data->m_TexCoords[3] = { m_PreviewData->GetFrame(i)->GetMin().x, m_PreviewData->GetFrame(i)->GetMax().y };
					data->IsSubTexture = true;

					ResourceDatabase::AddResource(m_OriginalResourcePath, (void*)data, ResourceType::Image);

					//m_ResourceData->m_frames.push_back(ResourceDatabase::GetResourceUUID(m_OriginalResourcePath, ResourceType::Image));
					delete data;
				}
				m_ResourceData->m_frameRate = m_PreviewData->GetFrameRate();
				m_ResourceData->m_frameCount = m_PreviewData->GetFrameCount();
				m_ResourceData->m_loop = m_PreviewData->IsLooping();
				m_ResourceData->name = m_ResourceData->AnimationName;
				m_ResourceData->Extension = ".anim";
				
				ResourceDatabase::AddResource(m_OriginalResourcePath, (void*)m_ResourceData, ResourceType::Animation);
			}
			ImGui::End();
		}
	}

	void AnimationPanel::ResetData()
	{
		m_FrameWidth = 0;
		m_FrameHeight = 0;
		m_Column = 0;
		m_Row = 0;
		m_ColumnCount = 0;
		m_RowCount = 0;
		m_TextureData = nullptr;
		m_OriginalResourcePath = "";
		m_PreviewOriginImage = nullptr;
		m_PreviewData = nullptr;
		m_ShowImagePanel = false;
		m_BasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Animation";
	}
	void AnimationPanel::SaveAnimation()
	{
	}
}