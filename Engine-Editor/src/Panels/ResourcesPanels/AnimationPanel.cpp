#include "egpch.h"
#include "AnimationPanel.h"

#include "Engine/Resources/ResourceSerializer.h"
#include "Engine/Resources/ResourceDatabase.h"
#include "Engine/Resources/ResourceUtils.h"
#include "Imgui/imgui.h"


namespace eg {

	namespace Utils {
		ImVec4 ColorFromHexNoAlpha(unsigned int hexColor)
		{
            EG_PROFILE_FUNCTION();
			float r = ((hexColor >> 16) & 0xFF) / 255.0f;
			float g = ((hexColor >> 8) & 0xFF) / 255.0f;
			float b = (hexColor & 0xFF) / 255.0f;
			float a = 1.0f; // Full opacity by default
			return ImVec4(r, g, b, a);
		}
	}

	constexpr const int DRAGINTWIDTH = 125;
	constexpr const int BASEGAP = 20;
	constexpr const int ADDITIONALGAP = 64;
	constexpr const int ROUNDING = 10;

	const ImVec4 BASEINPUTCOLOR = Utils::ColorFromHexNoAlpha(0x403659);
	const ImVec4 HOVEREDCOLOR = Utils::ColorFromHexNoAlpha(0x5A4A7D);
	const ImVec4 ACTIVEINPUTCOLOR = Utils::ColorFromHexNoAlpha(0x83799E);
	const ImVec4 BGCOLOR = Utils::ColorFromHexNoAlpha(0x281F3A);

	const ImVec4 UNSELECTEDFRAMECOLOR = Utils::ColorFromHexNoAlpha(0xA64D4D);
	const ImVec4 SELECTEDFRAMECOLOR = Utils::ColorFromHexNoAlpha(0x4DA674);
	//const ImVec4 SELECTEDFRAMECOLOR = Utils::ColorFromHexNoAlpha(0x609686FF);

	AnimationPanel::AnimationPanel(const std::filesystem::path& path)
	{
	}

	bool AnimationPanel::OpenAnimationPanel(const std::filesystem::path& path)
	{
        EG_PROFILE_FUNCTION();
		EG_PROFILE_FUNCTION();
		ShowAnimationPanel(true);
		m_FrameData = CreateRef<FrameData>();

		std::filesystem::path textureBasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Animations";
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
		(m_TextureData)->ParentDirectory = AssetDirectoryManager::GetRootAssetTypeDirectory(ResourceType::Image);
		m_TextureData->ResourceName = path.stem().string();
		m_TextureData->Extension = path.extension().string();
		m_TextureData->Height = ((ImageResourceData*)m_LoadedResource->Data)->height;
		m_TextureData->Width = ((ImageResourceData*)m_LoadedResource->Data)->width;
		m_TextureData->Channels = ((ImageResourceData*)m_LoadedResource->Data)->channelCount;
		m_FrameHeight = ((ImageResourceData*)m_LoadedResource->Data)->height;
		m_FrameWidth = ((ImageResourceData*)m_LoadedResource->Data)->width;

		m_PreviewOriginImage = Texture2D::Create(path.string());
		m_PreviewData = Animation::Create();
		m_ResourceData = new AnimationResourceData();
		m_ResourceData->ParentDirectory = 0;
		m_ResourceData->ResourceName = std::filesystem::path(m_LoadedResource->Name).stem().string();
		m_ResourceData->Extension = ".anim";

		m_ImageAspectRatio = (float)m_PreviewOriginImage->GetWidth() / (float)m_PreviewOriginImage->GetHeight();
		if (m_ImageAspectRatio < 1.0f)
		{
			m_BasePreviewWidthImage = 256 * m_ImageAspectRatio;
			m_BasePreviewHeightImage = 256;
		}
		else
		{
			m_BasePreviewWidthImage = 256;
			m_BasePreviewHeightImage = 256 / m_ImageAspectRatio;
		}

		m_PreviewAspectRatio = m_ImageAspectRatio;
		m_BasePreviewHeight = m_BasePreviewHeightImage;
		m_BasePreviewWidth = m_BasePreviewWidthImage;

		SetSelectedFrames();
		return true;
	}

	void AnimationPanel::SetFrames()
	{
        EG_PROFILE_FUNCTION();
		EG_PROFILE_FUNCTION();
		m_PreviewData->ClearFrames();

		m_FrameWidth = m_TextureData->Width / m_MaxColumn;
		m_FrameHeight = m_TextureData->Height / m_MaxRow;

		std::sort(m_SelectedFrames.begin(), m_SelectedFrames.end());
		for (auto& frame : m_SelectedFrames) {
			Ref<FrameData> frameData = CreateRef<FrameData>();

			glm::vec2 min = { frame.second * (float)m_FrameWidth / (float)m_TextureData->Width, 1.0f - ((frame.first + 1) * (float)m_FrameHeight) / (float)m_TextureData->Height };
			glm::vec2 max = { (frame.second + 1) * (float)m_FrameWidth / (float)m_TextureData->Width,1.0f - (frame.first * (float)m_FrameHeight) / (float)m_TextureData->Height };

			frameData->SetSubTexture(SubTexture2D::Create(m_PreviewOriginImage, min, max));

			frameData->SetFrameDuration(1);
			m_PreviewData->AddFrame(frameData);
		}

		m_PreviewAspectRatio = (float)m_FrameWidth / (float)m_FrameHeight;
		if (m_PreviewAspectRatio < 1.0f)
		{
			m_BasePreviewWidth = 256 * m_PreviewAspectRatio;
			m_BasePreviewHeight = 256;
		}
		else
		{
			m_BasePreviewWidth = 256;
			m_BasePreviewHeight = 256 / m_PreviewAspectRatio;
		}
		m_PreviewData->SetFrame(0);
	}

	void AnimationPanel::SetSelectedFrames()
	{
        EG_PROFILE_FUNCTION();
		EG_PROFILE_FUNCTION();
		m_SelectedFrames.clear();
		for (int i = m_Row; i < m_RowCount + m_Row; i++)
			for (int j = m_Column; j < m_ColumnCount + m_Column; j++)
				m_SelectedFrames.emplace_back(std::make_pair(i, j));
		SetFrames();
	}

	void AnimationPanel::ClearOutdatedFrames()
	{
        EG_PROFILE_FUNCTION();
		EG_PROFILE_FUNCTION();
		std::vector<std::pair<int, int>> newSelectedFrames;
		for (auto& frame : m_SelectedFrames)
		{
			if (frame.first > m_MaxRow || frame.second > m_MaxColumn)
				continue;
			newSelectedFrames.emplace_back(frame);
		}
		m_SelectedFrames = newSelectedFrames;
		SetFrames();
	}

	void AnimationPanel::DeleteData()
	{
        EG_PROFILE_FUNCTION();
		EG_PROFILE_FUNCTION();
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

		m_PreviewOriginImage = nullptr;
		m_PreviewData = nullptr;
	}

	void AnimationPanel::SaveData()
	{
        EG_PROFILE_FUNCTION();
		m_ResourceData->Frames.clear();
		m_ResourceData->Frames.reserve(m_PreviewData->GetFrameCount());

		m_TextureUUID = ResourceDatabase::AddResource(m_OriginalResourcePath, (void*)m_TextureData, ResourceType::Image);
		for (int i = 0; i < m_PreviewData->GetFrameCount(); i++)
		{
			Ref<SubTexture2D> subTexture = m_PreviewData->GetFrame(i)->GetSubTexture();

			SubTextureResourceData* data = new SubTextureResourceData();
			data->ParentDirectory = AssetDirectoryManager::GetRootAssetTypeDirectory(ResourceType::SubTexture);
			data->Texture = m_TextureUUID;
			data->ResourceName = m_ResourceData->ResourceName + std::to_string(i);
			data->TexCoords[0] = subTexture->GetMin();
			data->TexCoords[1] = { subTexture->GetMax().x, subTexture->GetMin().y };
			data->TexCoords[2] = subTexture->GetMax();
			data->TexCoords[3] = { subTexture->GetMin().x, subTexture->GetMax().y };

			UUID subTextureUUID = ResourceDatabase::AddResource(m_OriginalResourcePath, (void*)data, ResourceType::SubTexture);

			FrameResourceData* dataFrame = new FrameResourceData();
			dataFrame->ParentDirectory = AssetDirectoryManager::GetRootAssetTypeDirectory(ResourceType::Frame);
			dataFrame->ResourceName = m_ResourceData->ResourceName + std::to_string(i);
			dataFrame->Duration = m_PreviewData->GetFrame(i)->GetFrameDuration();
			dataFrame->SubTexture = subTextureUUID;
			dataFrame->ClassName = m_PreviewData->GetFrame(i)->GetClassname();
			dataFrame->FunctionCallName = m_PreviewData->GetFrame(i)->GetFunctionCallName();
			UUID uuid = ResourceDatabase::AddResource(m_OriginalResourcePath, (void*)dataFrame, ResourceType::Frame);
			m_ResourceData->Frames.push_back(uuid);
		}

		m_ResourceData->ParentDirectory = AssetDirectoryManager::GetRootAssetTypeDirectory(ResourceType::Animation);
		m_ResourceData->FrameRate = m_PreviewData->GetFrameRate();
		m_ResourceData->FrameCount = m_PreviewData->GetFrameCount();
		m_ResourceData->Loop = m_PreviewData->IsLooped();
		m_ResourceData->Extension = ".anim";
		UUID animID = ResourceDatabase::AddResource(m_OriginalResourcePath, (void*)m_ResourceData, ResourceType::Animation);
	}

	void AnimationPanel::DrawSubtexture(const int& i,const int& j)
	{
        EG_PROFILE_FUNCTION();
		if (j != 0)
			ImGui::SameLine();
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
		std::pair<int, int> frameIndex = std::make_pair(i, j);
		auto it = std::find(m_SelectedFrames.begin(), m_SelectedFrames.end(), frameIndex);
		bool isSelected = it != m_SelectedFrames.end();


		bool isSelectedFrame = std::find(m_SelectedFrames.begin(), m_SelectedFrames.end(), std::make_pair(i, j)) != m_SelectedFrames.end();
		//#A64D4D i #4DA674 / #
		ImVec4 borderColor = isSelected ? SELECTEDFRAMECOLOR : UNSELECTEDFRAMECOLOR;
		ImVec2 size = ImVec2(
			m_BasePreviewWidthImage / (int)(m_PreviewOriginImage->GetWidth() / m_FrameWidth),
			m_BasePreviewHeightImage / (int)(m_PreviewOriginImage->GetHeight() / m_FrameHeight)
		);
		ImVec2 min = ImVec2(
			j * (float)m_FrameWidth / (float)m_TextureData->Width,
			1.0f - (i) * (float)m_FrameHeight / (float)m_TextureData->Height
		);
		ImVec2 max = ImVec2(
			(j + 1) * (float)m_FrameWidth / (float)m_TextureData->Width,
			1.0f - (i + 1) * (float)m_FrameHeight / (float)m_TextureData->Height
		);

		ImGui::Image(
			(void*)m_PreviewOriginImage->GetRendererID(),
			size,
			min,
			max,
			{ 1,1,1,1 },
			borderColor);

		if (ImGui::IsItemClicked(ImGuiMouseButton_Left) && isSelected) {
			m_SelectedFrames.erase(it);
			SetFrames();
		}
		else if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
			m_SelectedFrames.emplace_back(frameIndex);
			SetFrames();
		}
		ImGui::PopStyleVar();
	}

	void AnimationPanel::DrawFrameSelection()
	{
        EG_PROFILE_FUNCTION();
		EG_PROFILE_FUNCTION();
		float contentRegionAvailX = ImGui::GetContentRegionAvail().x;
		ImGui::PushItemWidth(DRAGINTWIDTH);

		if (ImGui::DragInt("Column count", &m_MaxColumn, 1.0f, 1, m_TextureData->Width))
		{
			if (m_MaxColumn < 1)
				m_MaxColumn = 1;
			if(m_Column > m_MaxColumn - 1)
				m_Column = m_MaxColumn - 1;
			if(m_ColumnCount > m_MaxColumn - m_Column)
				m_ColumnCount = m_MaxColumn - m_Column;
			ClearOutdatedFrames();
			SetFrames();
		}


		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + BASEGAP + ADDITIONALGAP);

		if (ImGui::DragInt("Row count", &m_MaxRow, 1.0f, 1, m_TextureData->Height))
		{
			if (m_MaxRow < 1)
				m_MaxRow = 1;
			if (m_Row > m_MaxRow - 1)
				m_Row = m_MaxRow - 1;
			if (m_RowCount > m_MaxRow - m_Row)
				m_RowCount = m_MaxRow - m_Row;
			ClearOutdatedFrames();
			SetFrames();
		}


		if (ImGui::DragInt("Begin Column", &m_Column, 1.0f, 0, m_MaxColumn-1))
		{
			if (m_Column > m_MaxColumn - 1)
			{
				m_Column = m_MaxColumn - 1;
				m_ColumnCount = 1;
			}
			else if (m_Column < 0)
				m_Column = 0;
			else if (m_Column > m_MaxColumn - m_ColumnCount)
				m_ColumnCount = m_MaxColumn - m_Column;
			SetSelectedFrames();
		}

		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + BASEGAP + ADDITIONALGAP);

		if (ImGui::DragInt("Begin Row", &m_Row, 1.0f, 0, m_MaxRow - 1))
		{
			if (m_Row > m_MaxRow - 1)
			{
				m_Row = m_MaxRow - 1;
				m_RowCount = 1;
			}
			else if (m_Row < 0)
				m_Row = 0;
			else if (m_Row > m_MaxRow - m_RowCount)
				m_RowCount = m_MaxRow - m_Row;
			SetSelectedFrames();
		}

		if (ImGui::DragInt("Selected Columns Count", &m_ColumnCount, 1.0f, 1, m_MaxColumn-m_Column))
		{
			if (m_ColumnCount < 1)
				m_ColumnCount = 1;
			else if (m_Column + m_ColumnCount > m_MaxColumn)
				m_ColumnCount = m_MaxColumn - m_Column;
			SetSelectedFrames();
		}

		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + BASEGAP);

		if (ImGui::DragInt("Selected Rows Count", &m_RowCount, 1.0f, 1, m_MaxRow-m_Row))
		{
			if (m_RowCount < 1)
				m_RowCount = 1;
			else if (m_Row + m_RowCount > m_MaxRow)
				m_RowCount = m_MaxRow - m_Row;
			SetSelectedFrames();
		}
		ImGui::PopItemWidth();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + BASEGAP);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(20, 20));

		if (ImGui::Button("Select all", ImVec2(90, 40))) {
			for (int i = 0; i < (int)(m_PreviewOriginImage->GetHeight() / m_FrameHeight); i++)
				for (int j = 0; j < (int)(m_PreviewOriginImage->GetWidth() / m_FrameWidth); j++) {
					std::pair<int, int> frameIndex = std::make_pair(i, j);
					m_SelectedFrames.emplace_back(frameIndex);

				}
			SetFrames();
		}

		ImGui::SameLine();
		if (ImGui::Button("Select none", ImVec2(90, 40))) {
			m_SelectedFrames.clear();
			SetFrames();
		}

		ImGui::PopStyleVar(1);
	}

	void AnimationPanel::DrawAnimInfo()
	{
        EG_PROFILE_FUNCTION();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + BASEGAP);
		ImGui::Checkbox("Play", m_PreviewData->IsPlayingPtr());

		ImGui::SameLine();

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + BASEGAP);
		ImGui::Checkbox("Loop", m_PreviewData->IsLoopedPtr());

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + BASEGAP);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ROUNDING);

		if (ImGui::DragInt("Frame Rate", m_PreviewData->GetFrameRatePtr(), 1.0f, 0.0f, 500)) {
			SetFrames();
		}

		char buffer[512];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, m_ResourceData->ResourceName.c_str(), sizeof(buffer));

		if (ImGui::InputText("Animation Name", buffer, sizeof(buffer)))
			m_ResourceData->ResourceName = std::string(buffer);
		ImGui::PopStyleVar();
	}

	void AnimationPanel::DrawAnimationPreview()
	{
        EG_PROFILE_FUNCTION();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + BASEGAP);
		ImGui::Text("Animation Preview:");
		if (m_PreviewData->GetFrameCount() > 0)
		{
			Ref<SubTexture2D> subTexture = m_PreviewData->GetFrame()->GetSubTexture();
			ImGui::Image(
				(void*)subTexture->GetTexture()->GetRendererID(),
				ImVec2(m_BasePreviewWidth, m_BasePreviewHeight),
				{
					subTexture->GetMin().x , subTexture->GetMax().y
				},
				{
					subTexture->GetMax().x , subTexture->GetMin().y
				});
		}
	}

	void AnimationPanel::OnImGuiRender()
	{
        EG_PROFILE_FUNCTION();
		EG_PROFILE_FUNCTION();
		if (!m_ShowAnimationPanel)
			return;

		ImGui::PushStyleColor(ImGuiCol_WindowBg, BGCOLOR);
		ImGui::PushStyleColor(ImGuiCol_Tab, BGCOLOR);
		ImGui::PushStyleColor(ImGuiCol_TabHovered, HOVEREDCOLOR);
		ImGui::PushStyleColor(ImGuiCol_TabActive, ACTIVEINPUTCOLOR);
		ImGui::PushStyleColor(ImGuiCol_TabUnfocusedActive, ACTIVEINPUTCOLOR);
		ImGui::PushStyleColor(ImGuiCol_TabUnfocused, BGCOLOR);
		ImGui::PushStyleColor(ImGuiCol_TitleBg, BGCOLOR);
		ImGui::Begin("Animation Preview");

		ImGui::PushStyleColor(ImGuiCol_FrameBg, BASEINPUTCOLOR);
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, HOVEREDCOLOR);
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ACTIVEINPUTCOLOR);

		ImGui::PushStyleColor(ImGuiCol_Button, BASEINPUTCOLOR);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HOVEREDCOLOR);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ACTIVEINPUTCOLOR);

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ROUNDING);

		DrawFrameSelection();

		ImGui::Text("Image Preview:");

		for (int i = 0; i < (int)(m_PreviewOriginImage->GetHeight() / m_FrameHeight); i++)
		{
			for (int j = 0; j < (int)(m_PreviewOriginImage->GetWidth() / m_FrameWidth); j++)
			{
				DrawSubtexture(i, j);
			}
		}

		DrawAnimInfo();

		DrawAnimationPreview();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(20, 20));
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + BASEGAP);

		if (ImGui::Button("Save", ImVec2(90, 40)))
		{

			SaveData();
			CloseAnimationPanel();
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(90, 40)))
			CloseAnimationPanel();

		ImGui::PopStyleColor(12);
		ImGui::PopStyleVar(2);
		ImGui::End();
		ImGui::PopStyleColor();
	}

	void AnimationPanel::CloseAnimationPanel()
	{
        EG_PROFILE_FUNCTION();
		EG_PROFILE_FUNCTION();
		DeleteData();
		ResetData();
		ShowAnimationPanel(false);
	}

	void AnimationPanel::ResetData()
	{
        EG_PROFILE_FUNCTION();
		EG_PROFILE_FUNCTION();
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
		m_SelectedFrames.clear();
		m_FrameData = nullptr;
	}
}
