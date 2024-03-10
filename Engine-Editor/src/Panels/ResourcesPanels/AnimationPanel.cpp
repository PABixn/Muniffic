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
			EG_CORE_ERROR("Failed to load resource: {0}", path.string());
			DeleteData();
			ResetData();
			m_LoadedResource = nullptr;
			return false;
		}
		m_OriginalResourcePath = path;
		m_TextureData = new TextureResourceData();
		(m_TextureData)->ResourcePath = "Textures";
		m_TextureData->ImageName = m_LoadedResource->Path.stem().string();
		m_TextureData->Extension = m_LoadedResource->Path.extension().string();
		m_TextureData->Height = ((ImageResourceData*)m_LoadedResource->Data)->height;
		m_TextureData->Width = ((ImageResourceData*)m_LoadedResource->Data)->width;
		m_TextureData->Channels = ((ImageResourceData*)m_LoadedResource->Data)->channelCount;
		m_FrameHeight = ((ImageResourceData*)m_LoadedResource->Data)->height;
		m_FrameWidth = ((ImageResourceData*)m_LoadedResource->Data)->width;
		m_PreviewOriginImage = Texture2D::Create(path.string());
		std::vector<Ref<SubTexture2D>> frames;
		frames.push_back(SubTexture2D::Create(m_PreviewOriginImage, { 0, 0 }, { 1, 1 }));
		m_PreviewData = Animation::Create(frames);
		m_ResourceData = new AnimationResourceData();
		m_ResourceData->ResourcePath = "";
		m_ResourceData->AnimationName = std::filesystem::path(m_LoadedResource->Name).stem().string();
		m_ResourceData->Extension = ".anim";
		SetFrames();
		return true;
	}

	void AnimationPanel::SetFrames()
	{
		m_PreviewData->ClearFrames();
		
			for (int j = m_Row; j < m_Row + m_RowCount; j++)
				for (int i = m_Column; i < m_Column + m_ColumnCount; i++)
			{
				glm::vec2 min = { (i * (float)m_FrameWidth) / (float)m_TextureData->Width, (j * (float)m_FrameHeight) / (float)m_TextureData->Height };
				float maxX = ((i + 1) * (float)m_FrameWidth) / (float)m_TextureData->Width;
				float maxY = ((j + 1) * (float)m_FrameHeight) / (float)m_TextureData->Height;
				glm::vec2 max = { maxX <= 1 ? maxX : 1, maxY <= 1 ? maxY : 1};
				m_PreviewData->AddFrame(SubTexture2D::Create(m_PreviewOriginImage, min, max));
			}
	}

	void AnimationPanel::DeleteData()
	{
		if (m_LoadedResource)
		{
			if (m_LoadedResource->Data)
			{
				if (((ImageResourceData*)m_LoadedResource->Data)->pixels)
					delete ((ImageResourceData*)m_LoadedResource->Data)->pixels;
				delete (ImageResourceData*)m_LoadedResource->Data;
			}
			delete m_LoadedResource;
		}
		
		
		if(m_TextureData)
			delete m_TextureData;
		if(m_ResourceData)
			delete m_ResourceData;

		m_PreviewOriginImage = nullptr;
		m_PreviewData = nullptr;
	}

	void AnimationPanel::OnImGuiRender()
	{
		if (m_ShowAnimationPanel)
		{
			ImGui::Begin("Animation Preview");
			if (ImGui::DragInt("Frame Width: %d", &m_FrameWidth, 1.0f, 1, m_PreviewOriginImage->GetWidth()))
			{
				if (m_ColumnCount > (int)(m_PreviewOriginImage->GetWidth() / m_FrameWidth) - m_Column)
				{
					m_ColumnCount = (int)(m_PreviewOriginImage->GetWidth() / m_FrameWidth) - m_Column;
				}
				SetFrames();
			}
			if (ImGui::DragInt("Frame Height: %d", &m_FrameHeight, 1.0f, 1, m_PreviewOriginImage->GetHeight()))
			{
				if (m_RowCount > (int)(m_PreviewOriginImage->GetHeight() / m_FrameHeight) - m_Row)
				{
					m_RowCount = (int)(m_PreviewOriginImage->GetHeight() / m_FrameHeight) - m_Row;
				}
				SetFrames();
			}
			if (ImGui::DragInt("Column: %d", &m_Column, 1.0f, 0, (int)(m_PreviewOriginImage->GetWidth() / m_FrameWidth)))
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
			int maxColumnCount = (int)(m_PreviewOriginImage->GetWidth() / m_FrameWidth) - m_Column;
			if(ImGui::DragInt("Column Count: %d", &m_ColumnCount, 1.0f, 0, maxColumnCount))
			{
				SetFrames();
			}
			int maxRowCount = (int)(m_PreviewOriginImage->GetHeight() / m_FrameHeight) - m_Row;
			if (ImGui::DragInt("Row Count: %d", &m_RowCount, 1.0f, 0, maxRowCount))
			{
				SetFrames();
			}
			ImGui::Text("Image Preview:");
				for (int j = 0; j < (int)(m_PreviewOriginImage->GetHeight() / m_FrameHeight); j++)
					for (int i = 0; i < (int)(m_PreviewOriginImage->GetWidth() / m_FrameWidth); i++)
				{
					if(i != 0)
					ImGui::SameLine();
					ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
					bool isSelected = i >= m_Column && i < m_Column + m_ColumnCount && j >= m_Row && j < m_Row + m_RowCount;
					ImVec4 borderColor = isSelected ? ImVec4{0.0f, 1.0f, 0.0f, 1.0f} : ImVec4{1.0f, 0.0f, 0.0f, 1.0f};
					ImGui::PushStyleColor(ImGuiCol_Border, borderColor);
					ImGui::Image((void*)m_PreviewOriginImage->GetRendererID(), ImVec2(256/ (int)(m_PreviewOriginImage->GetWidth() / m_FrameWidth), 256/ (int)(m_PreviewOriginImage->GetHeight() / m_FrameHeight)), { i * (float)m_FrameWidth / (float)m_TextureData->Width, j * (float)m_FrameHeight / (float)m_TextureData->Height }, { (i + 1) * (float)m_FrameWidth / (float)m_TextureData->Width, (j + 1) * (float)m_FrameHeight / (float)m_TextureData->Height });
					ImGui::PopStyleColor();
					ImGui::PopStyleVar();
				}
			ImGui::Checkbox("Play", m_PreviewData->IsPlayingPtr());
			ImGui::DragFloat("Frame Rate: %f", m_PreviewData->GetFrameRatePtr(), 1.0f, 0.0f, 500);
			ImGui::Checkbox("Loop", m_PreviewData->IsLoopingPtr());
			char buffer[512];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, m_ResourceData->AnimationName.c_str(), sizeof(buffer));

			if (ImGui::InputText("Animation Name: ", buffer, sizeof(buffer)))
				m_ResourceData->AnimationName = std::string(buffer);
			ImGui::Text("Animation: %s", m_ResourceData->AnimationName.c_str());
			if(m_PreviewData->GetFrameCount() > 0)
				ImGui::Image((void*)m_PreviewData->GetFrame()->GetTexture()->GetRendererID(), ImVec2(128, 128), { m_PreviewData->GetFrame()->GetMin().x , m_PreviewData->GetFrame()->GetMin().y}, { m_PreviewData->GetFrame()->GetMax().x , m_PreviewData->GetFrame()->GetMax().y });

			char buffer2[512];
			memset(buffer2, 0, sizeof(buffer2));
			std::strncpy(buffer2, m_ResourceData->ResourcePath.string().c_str(), sizeof(buffer2));
			if(ImGui::InputText("Resource Path: ", buffer2, sizeof(buffer2)))
					m_ResourceData->ResourcePath = std::filesystem::path(std::string(buffer2));
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
					std::filesystem::path finalPath = data->ResourcePath / m_ResourceData->ResourcePath / (std::filesystem::path(data->ImageName).stem().string() + data->Extension);
					ResourceDatabase::AddResource(finalPath, (void*)data, ResourceType::Image);

					m_ResourceData->m_frames.push_back(ResourceDatabase::GetResourceUUID(finalPath, ResourceType::Image));
					delete data;
				}
				m_ResourceData->m_frameRate = m_PreviewData->GetFrameRate();
				m_ResourceData->m_frameCount = m_PreviewData->GetFrameCount();
				m_ResourceData->m_loop = m_PreviewData->IsLooping();
				m_ResourceData->name = m_ResourceData->AnimationName;
				m_ResourceData->Extension = ".anim";
				
				ResourceDatabase::AddResource(std::filesystem::path("Animation") / m_ResourceData->ResourcePath / (m_ResourceData->AnimationName + m_ResourceData->Extension), (void*)m_ResourceData, ResourceType::Animation);
				CloseAnimationPanel();
			}
			if (ImGui::Button("Cancel"))
				CloseAnimationPanel();
			ImGui::End();
		}
	}

	void AnimationPanel::CloseAnimationPanel()
	{
		DeleteData();
		ResetData();
		ShowAnimationPanel(false);
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
		m_ShowAnimationPanel = false;
		m_BasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Animation";
	}
}