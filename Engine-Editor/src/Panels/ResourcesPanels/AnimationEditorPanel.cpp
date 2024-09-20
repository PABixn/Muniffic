#include "AnimationEditorPanel.h"
#include "Imgui/imgui.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Resources/AssetDirectoryManager.h"
namespace eg {

	static int baseFrameWidth = 20;

	bool AnimationEditorPanel::OpenAnimationEditorPanel(UUID asset) {
		ResetData();
		ShowAnimationEditorPanel(true);
		m_Anim = Animation::Create(asset);
		m_PlayIcon = Texture2D::Create("resources/icons/PlayButton.png");
		m_StopIcon = Texture2D::Create("resources/icons/StopButton.png");
		m_LenghtIcon = Texture2D::Create("resources/icons/animationEditorPanel/lengthChangeIcon.png");
		m_LenghtSelectedIcon = Texture2D::Create("resources/icons/animationEditorPanel/lengthChangeSelectedIcon.png");
		m_MoveIcon = Texture2D::Create("resources/icons/animationEditorPanel/moveIcon.png");
		m_MoveSelectedIcon = Texture2D::Create("resources/icons/animationEditorPanel/moveSelectedIcon.png");

		SetFrames();

		return m_Anim != nullptr;
	}

	void AnimationEditorPanel::OnImGuiRender() {
		if (!m_ShowAnimationEditorPanel)
			return;
		UpdateDisplayedFrame();
		ImGui::Begin("Animation Editor Panel", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::SetWindowSize(ImVec2(1000, 800));

		DrawPlayButton();

		DrawAnimationPreview();

		DrawFrameTrack();

		DrawActionButtons();

		DrawExitButtons();
		
		ImGui::End();
		HandleResize();

	}

	void AnimationEditorPanel::DrawAnimationPreview() {

		Ref<SubTexture2D> subTexture = m_Anim->GetFrame(m_DisplayedFrameIndex)->GetSubTexture();

		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 300) * 0.5f);
		ImGui::SetCursorPosY(100);
		float spaceY = ImGui::GetCursorPos().y + 322;
		int i = 0;
		
		if (m_Anim->GetFrameCount() > 0) {
			ImGui::Image(
				(void*)subTexture->GetTexture()->GetRendererID(),
				ImVec2(300, 300),
				ImVec2(
					subTexture->GetMin().x,
					subTexture->GetMax().y),
				ImVec2(
					subTexture->GetMax().x,
					subTexture->GetMin().y)
			);
			
		}
		ImGui::SetCursorPosY(spaceY);
	}

	void AnimationEditorPanel::Update(float dt)
	{
		if (m_PlayAnimation)
			m_Anim->Update(dt, 1.0f);
	}

	void AnimationEditorPanel::SetFrames() {

		m_Anim->SetFrame(0);
		ResetSelectedFrames();
	}

	void AnimationEditorPanel::ResetData()
	{
		ResetSelectedFrames();
		m_Anim = nullptr;
		m_PlayAnimation = false;
		m_ShowLongerFrame = false;
		m_ShowInitialFrame = true;
		m_Resize = false;
		m_Move = false;
		m_ClickedFrame = -1;
		m_ClickedFrame = -1;
		m_IsDisplayed = false;
		m_DisplayedFrameIndex = 0;
		m_IsDragging = false;
		m_DraggingIndex = -1;
	}

	void AnimationEditorPanel::UpdateSelectedFrames()
	{
		if (m_Move || m_Resize) {
			ResetSelectedFrames();
		}
		if (m_SelectedFrames.first == m_ClickedFrame) {
			m_SelectedFrames.first = m_SelectedFrames.second;
			m_SelectedFrames.second = -1;
			return;
		}
		else if (m_SelectedFrames.second == m_ClickedFrame) {
			m_SelectedFrames.second = -1;
			return;
		}
		if (m_SelectedFrames.first < 0) {
			m_SelectedFrames.first = m_ClickedFrame;
		}
		else {
			m_SelectedFrames.second = m_ClickedFrame;
		}
	}

	bool AnimationEditorPanel::IsFrameSelected(int frame)
	{
		return m_SelectedFrames.first == frame || m_SelectedFrames.second == frame;
	}

	void AnimationEditorPanel::ResetSelectedFrames()
	{
		m_SelectedFrames.first = -1;
		m_SelectedFrames.second = -1;
	}

	void AnimationEditorPanel::UpdateDisplayedFrame()
	{
		if(m_PlayAnimation)
		{
			m_DisplayedFrameIndex = m_Anim->GetCurrentFrame();
		}
		else if (m_ClickedFrame >= 0)
		{
			m_DisplayedFrameIndex = m_ClickedFrame;
		}
		else
		{
			m_DisplayedFrameIndex = 0;
		}
	}

	void AnimationEditorPanel::DrawPlayButton()
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.5f - 50 * 0.5f);
		if (ImGui::ImageButton((ImTextureID)(*m_Anim->IsPlayingPtr() ? m_StopIcon->GetRendererID() : m_PlayIcon->GetRendererID()), ImVec2(50, 50))) {
			if (*m_Anim->IsPlayingPtr()) {
				m_Anim->Stop();
				m_PlayAnimation = false;
				ResetSelectedFrames();
			}
			else {
				m_Anim->Play();
				m_PlayAnimation = true;
				ResetSelectedFrames();
			}
		}

		ImGui::PopStyleColor(3);
	}

	void AnimationEditorPanel::DrawFunctionCallPopup()
	{
		if (m_OpenFunctionCallPopup)
		{
			ImGui::OpenPopup("FunctionCallPopup");
			m_OpenFunctionCallPopup = false;
		}
		if (ImGui::BeginPopup("FunctionCallPopup")) {
			ImGui::Text("Function name:");
			static char functionName[128];
			ImGui::InputText("##functionName", functionName, 128);
			bool opened = ImGui::TreeNodeEx("Available Function calls:", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_MoreSpaceBetweenTextAndArrow);
			if (opened)
			{
				const auto& scriptMethods = ScriptEngine::GetAllScriptMethodMaps();
				for (auto& [className, value] : scriptMethods) {
					bool openedClass = ImGui::TreeNodeEx(className.c_str(), ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_MoreSpaceBetweenTextAndArrow);
					if (!openedClass)
						continue;
					for (auto& [MethodName, value2] : value) {
						if (ScriptEngine::isMethodInternal(MethodName) || ScriptEngine::GetMethodParameterCount(className, MethodName) != 0)
							continue;
						if (ImGui::Button(MethodName.c_str()))
						{
							m_Anim->GetFrame(m_ClickedFrame)->SetFunctionCallName(MethodName);
							m_Anim->GetFrame(m_ClickedFrame)->SetClassName(className);
							ImGui::CloseCurrentPopup();
						}
					}
					ImGui::TreePop();
				}
				ImGui::TreePop();

			}
			ImGui::EndPopup();
		}
	}

	void AnimationEditorPanel::DrawAnimationOptions()
	{
		if (ImGui::BeginPopup("AnimationOptions")) {
			if (m_SelectedFrames.second < 0) {
				if (ImGui::MenuItem("Remove frame")) {
					m_DeletedFrames.push_back(m_Anim->GetFrame(m_ClickedFrame)->GetFrameID());
					m_Anim->RemoveFrame(m_ClickedFrame);
					SetFrames();
				}
				if (ImGui::MenuItem("Add function call")) {
					m_OpenFunctionCallPopup = true;
				}
			}
			else {
				if (ImGui::MenuItem("Swap frames")) {
					m_Anim->SwapFrames(m_SelectedFrames.first, m_SelectedFrames.second);
					SetFrames();
				}
			}
			ImGui::EndPopup();
		}
	}

	void AnimationEditorPanel::DrawFrameTrack()
	{
		ImGui::BeginChild("Animation timeline", ImVec2(ImGui::GetWindowSize().x, 100), true);
		for (float i = ImGui::GetWindowPos().x + 6; i < ImGui::GetWindowPos().x + ImGui::GetWindowSize().x; i += 22.0f) {
			ImGui::GetWindowDrawList()->AddLine(ImVec2(i, ImGui::GetWindowPos().y), ImVec2(i, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y), IM_COL32(255, 255, 255, 255), 0.8f);
		}

		std::vector<ImU32> colors;
		colors.reserve(m_Anim->GetFrameCount());

		int i = 0;
		m_HoveredFrame = -1;
		m_FrameReleased = false;

		i = 0;
		for (auto frame : m_Anim->GetFrames()) {
			ImVec2 cursorPos = ImGui::GetCursorScreenPos();
			float rectWidth = baseFrameWidth * frame->GetFrameDuration() + 2 * (frame->GetFrameDuration() - 1);
			ImVec2 cursorPos2 = ImVec2(cursorPos.x + rectWidth, cursorPos.y + 40);
			if (ImGui::IsMouseHoveringRect(cursorPos, cursorPos2))
			{
				m_HoveredFrame = i;
			}
			ImGui::GetWindowDrawList()->AddRectFilled(cursorPos, cursorPos2, (ImU32)GetFrameColor(i));

			ImGui::SetCursorScreenPos(ImVec2(cursorPos.x, cursorPos.y));
			ImGui::InvisibleButton(("rectBtn" + std::to_string(i)).c_str(), ImVec2(rectWidth, 40));
			ImGui::SetCursorScreenPos(ImVec2(cursorPos.x + rectWidth + 2, cursorPos.y));

			HandleFrameLeftClick(i);
			HandleFrameRightClick(i);
			HandleFrameDrag(i);
			HandleFrameLeftClickRelease(i);

			i++;
		}
		
		
		DrawAnimationOptions();
		DrawFunctionCallPopup();

		if (m_FrameReleased) {
			HandleMove(m_HoveredFrame);
			m_IsDragging = false;
			m_DraggingIndex = -1;
		}
		ImGui::EndChild();
	}

	void AnimationEditorPanel::HandleFrameHover(int frameIndex)
	{
	}

	void AnimationEditorPanel::HandleFrameLeftClick(int clickedFrameIndex)
	{
		if (m_PlayAnimation || !ImGui::IsItemClicked(ImGuiMouseButton_Left))
			return;
		if (m_ClickedFrame == clickedFrameIndex) {
			m_IsDisplayed = !m_IsDisplayed;
			m_ClickedFrame = -1;
		}
		else {
			m_ClickedFrame = clickedFrameIndex;
		}
		UpdateSelectedFrames();
		EG_TRACE("Clicked frame: {0}", clickedFrameIndex);
		
	}

	void AnimationEditorPanel::HandleFrameRightClick(int clickedFrameIndex)
	{
		if (m_PlayAnimation)
			return;
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			ImGui::OpenPopup("AnimationOptions");
			m_ClickedFrame = clickedFrameIndex;
		}
	}

	void AnimationEditorPanel::HandleFrameDrag(int clickedFrameIndex)
	{
		if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
			if (!m_IsDragging) {
				m_IsDragging = true;
				m_DraggingIndex = clickedFrameIndex;
				if (m_Resize)
				{
					m_FrameStartDuration = m_Anim->GetFrame(clickedFrameIndex)->GetFrameDuration();
					ImGui::ResetMouseDragDelta();
				}
			}
		}
	}

	void AnimationEditorPanel::HandleFrameLeftClickRelease(int clickedFrameIndex)
	{
		if (!ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			return;
		m_FrameReleased = true;
		m_FrameStartDuration = 1;
		ResetSelectedFrames();
	}

	void AnimationEditorPanel::HandleResize()
	{
		if(!m_Resize)
			return;
		if (!m_IsDragging || m_DraggingIndex == -1)
			return;
		m_ResizeOffset += ImGui::GetIO().MouseDelta.x;
		float estNewDuration = std::max(1, (int)(m_FrameStartDuration + m_ResizeOffset / (baseFrameWidth+2)));
		m_Anim->GetFrame(m_DraggingIndex)->SetFrameDuration(estNewDuration);
		SetFrames();
		
	}

	void AnimationEditorPanel::HandleMove(int moveTo)
	{
		if (!m_Move)
			return;
		if (!m_IsDragging || m_DraggingIndex == -1)
			return;

		if (moveTo != m_DraggingIndex) {
			m_Anim->MoveFrame(m_DraggingIndex, moveTo);
			SetFrames();
		}
		
	}

	void AnimationEditorPanel::DrawActionButtons()
	{
		ImGui::DragInt("Frame Rate %f:", m_Anim->GetFrameRatePtr(), 1.0f, 1.0f, 500);

		if (ImGui::ImageButton((ImTextureID)(m_Resize ? m_LenghtSelectedIcon->GetRendererID() : m_LenghtIcon->GetRendererID()), ImVec2(50, 50), ImVec2(0, 0), ImVec2(1, 1), 0)) {
			if (m_Move)
				m_Move = false;
			m_Resize = !m_Resize;
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((ImTextureID)(m_Move ? m_MoveSelectedIcon->GetRendererID() : m_MoveIcon->GetRendererID()), ImVec2(50, 50), ImVec2(0, 0), ImVec2(1, 1), 0)) {
			if (m_Resize)
				m_Resize = false;
			m_Move = !m_Move;
		}
	}

	void AnimationEditorPanel::DrawExitButtons()
	{
		if (ImGui::Button("Save")) {
			m_Anim->Save();
			FrameData::DeleteAssets(m_DeletedFrames);
			ShowAnimationEditorPanel(false);
			ResetData();
		}
		if (ImGui::Button("Close")) {
			ShowAnimationEditorPanel(false);
			ResetData();
		}
	}

	uint32_t AnimationEditorPanel::GetFrameColor(int frame)
	{
		if (m_IsDragging && m_DraggingIndex == frame)
			return IM_COL32(255, 105, 180, 255);
		if (IsFrameSelected(frame))
			return IM_COL32(65, 51, 122, 255);
		if(m_HoveredFrame == frame)
			return IM_COL32(255, 255, 255, 255);
		return IM_COL32(194, 239, 235, 255);
	}
}