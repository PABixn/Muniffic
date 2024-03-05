#include "egpch.h"
#include "AnimationPanel.h"
#include "Imgui/imgui.h"
#include "Engine/Resources/ResourceSerializer.h"
#include "Engine/Resources/ResourceDatabase.h"

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
		m_ResourceData = new TextureResourceData();
		((TextureResourceData*)m_ResourceData)->ResourcePath = "Textures";
		((TextureResourceData*)m_ResourceData)->ImageName = m_TextureData.ResourcePath.stem().string();
		((TextureResourceData*)m_ResourceData)->Extension = m_TextureData.ResourcePath.extension().string();
		((TextureResourceData*)m_ResourceData)->Height = ((ImageResourceData*)m_LoadedResource->Data)->height;
		((TextureResourceData*)m_ResourceData)->Width = ((ImageResourceData*)m_LoadedResource->Data)->width;
		((TextureResourceData*)m_ResourceData)->Channels = ((ImageResourceData*)m_LoadedResource->Data)->channelCount;
		m_PreviewOriginImage = Texture2D::Create(path.string());
		std::vector<Ref<SubTexture2D>> frames;
		frames.push_back(SubTexture2D::Create(m_PreviewOriginImage, { 0, 0 }, { 1, 1 }));
		m_PreviewData = Animation::Create(frames);
	}

	void AnimationPanel::OnImGuiRender()
	{
		if (m_ShowImagePanel)
		{
			ImGui::Begin("Animation Preview");
			ImGui::DragInt("Frame Width: %d", &m_FrameWidth, 1.0f, 0, 500);
			ImGui::DragInt("Frame Height: %d", &m_FrameHeight, 1.0f, 0, 500);
			ImGui::DragInt("Column: %d", &m_Column, 1.0f, 0, 500);
			ImGui::DragInt("Row: %d", &m_Row, 1.0f, 0, 500);
			ImGui::DragInt("Column Count: %d", &m_ColumnCount, 1.0f, 0, 500);
			ImGui::DragInt("Row Count: %d", &m_RowCount, 1.0f, 0, 500);
			ImGui::Text("Image Preview:");
			ImGui::Image((void*)m_PreviewOriginImage->GetRendererID(), ImVec2(128, 128));
			ImGui::Checkbox("Play", m_PreviewData->IsPlayingPtr());
			ImGui::DragFloat("Frame Rate: %f", m_PreviewData->GetFrameRatePtr(), 1.0f, 0.0f, 500);
			ImGui::Checkbox("Loop", m_PreviewData->IsLoopingPtr());
			ImGui::Text("Animation: %s", m_PreviewData->GetName().c_str());
			ImGui::Image((void*)m_PreviewData->GetFrame()->GetTexture()->GetRendererID(), ImVec2(128, 128), { m_PreviewData->GetFrame()->GetMin().x , m_PreviewData->GetFrame()->GetMin().y}, { m_PreviewData->GetFrame()->GetMax().x , m_PreviewData->GetFrame()->GetMax().y });
			if (ImGui::Button("Save"))
			{
				ResourceDatabase::AddResource(m_OriginalResourcePath, m_ResourceData, ResourceType::Animation);
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
		m_TextureData = TextureResourceData();
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