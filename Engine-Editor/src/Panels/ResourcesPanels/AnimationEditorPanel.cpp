#include "AnimationEditorPanel.h"

#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Resources/AssetDirectoryManager.h"
#include "../../IconLoader.h"

namespace eg {

	constexpr const float LINEWIDTH = 3;
	constexpr const float LINEOFFSET = 1.5f;
	constexpr const int BASEGAP = 10;

	constexpr const float ROUNDING = 5.0f;
	constexpr const float MINFRAMEWIDTH = 30.0f;

	constexpr const ImU32 BASEINPUTCOLOR = IM_COL32(0x40, 0x36, 0x59, 0xff);//0x403659FF;
	constexpr const ImU32 HOVEREDCOLOR = IM_COL32(0x5a, 0x4a, 0xa7, 0xff);//0x5A4A7DFF;
	constexpr const ImU32 ACTIVEINPUTCOLOR = IM_COL32(0x83, 0x79, 0x93, 0xff);// 0x83799EFF;
	constexpr const ImU32 SELECTEDFRAMECOLOR = IM_COL32(0xc6, 0xbb, 0xe2, 0xff);//0xC6BBE2FF;
	constexpr const ImU32 BGCOLOR = IM_COL32(0x28, 0x1f, 0x3a, 0xff);// 0x281F3AFF;

	float hoverTime = 0.0f;
	float hoverTimeMax = 1.5f;
	int hoverFrame = -1;
	float frameWidth = 0;

	AnimationEditorPanel::AnimationEditorPanel(UUID asset)
	{
        EG_PROFILE_FUNCTION();
		OpenAnimationEditorPanel(asset);
	}

	bool AnimationEditorPanel::OpenAnimationEditorPanel(UUID asset) {
        EG_PROFILE_FUNCTION();
		ResetData();
		ShowAnimationEditorPanel(true);
		m_Anim = Animation::Create(asset);
		SetFrames();

		return m_Anim != nullptr;
	}

	void AnimationEditorPanel::OnImGuiRender() {
        EG_PROFILE_FUNCTION();
		if (!m_ShowAnimationEditorPanel)
			return;

		ImGui::PushStyleColor(ImGuiCol_WindowBg, BGCOLOR);
		ImGui::PushStyleColor(ImGuiCol_Tab, BGCOLOR);
		ImGui::PushStyleColor(ImGuiCol_TabHovered, HOVEREDCOLOR);
		ImGui::PushStyleColor(ImGuiCol_TabActive, ACTIVEINPUTCOLOR);
		ImGui::PushStyleColor(ImGuiCol_TabUnfocusedActive, ACTIVEINPUTCOLOR);
		ImGui::PushStyleColor(ImGuiCol_TabUnfocused, BGCOLOR);
		ImGui::PushStyleColor(ImGuiCol_TitleBg, BGCOLOR);

		UpdateDisplayedFrame();
		ImGui::Begin("Animation Editor Panel", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::SetWindowSize(ImVec2(1000, 800));

		DrawPlayButton();

		DrawAnimationPreview();

		DrawFrameTrack();

		DrawFrameRate();

		DrawExitButtons();

		ImGui::End();

		ImGui::PopStyleColor(7);
	}

	void AnimationEditorPanel::DrawAnimationPreview() {
        EG_PROFILE_FUNCTION();

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
        EG_PROFILE_FUNCTION();
		if (m_PlayAnimation)
			m_Anim->Update(dt, 1.0f);
	}

	void AnimationEditorPanel::SetFrames() {
        EG_PROFILE_FUNCTION();

		m_Anim->SetFrame(0);
		ResetSelectedFrames();
	}

	void AnimationEditorPanel::ResetData()
	{
        EG_PROFILE_FUNCTION();
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
        EG_PROFILE_FUNCTION();
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
        EG_PROFILE_FUNCTION();
		return m_SelectedFrames.first == frame || m_SelectedFrames.second == frame;
	}

	void AnimationEditorPanel::ResetSelectedFrames()
	{
        EG_PROFILE_FUNCTION();
		m_SelectedFrames.first = -1;
		m_SelectedFrames.second = -1;
	}

	void AnimationEditorPanel::UpdateDisplayedFrame()
	{
        EG_PROFILE_FUNCTION();
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
        EG_PROFILE_FUNCTION();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.5f - 50 * 0.5f);
		if (ImGui::ImageButton((ImTextureID)(*m_Anim->IsPlayingPtr() ? IconLoader::GetIcon(Icons::Editor_StopButton)->GetRendererID() : IconLoader::GetIcon(Icons::Editor_PlayButton)->GetRendererID()), ImVec2(50, 50))) {
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
        EG_PROFILE_FUNCTION();
		if (m_OpenFunctionCallPopup)
		{
			ImGui::OpenPopup("FunctionCallPopup");
			m_OpenFunctionCallPopup = false;
		}
		ImGui::PushStyleColor(ImGuiCol_PopupBg, BGCOLOR);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, BASEINPUTCOLOR);
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, HOVEREDCOLOR);
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ACTIVEINPUTCOLOR);
		ImGui::PushStyleColor(ImGuiCol_Header, BASEINPUTCOLOR);
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, HOVEREDCOLOR);
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ACTIVEINPUTCOLOR);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, ROUNDING);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ROUNDING);

		if (ImGui::BeginPopup("FunctionCallPopup")) {
			ImGui::Text("Function name:");
			static char functionName[128];
			ImGui::InputText("##functionName", functionName, 128);
			int treeNodeFlags = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_MoreSpaceBetweenTextAndArrow;
			bool opened = ImGui::TreeNodeEx("Available Function calls:", treeNodeFlags);
			if (opened)
			{
				const auto& scriptMethods = ScriptEngine::GetAllScriptMethodMaps();
				for (auto& [className, value] : scriptMethods) {
					bool openedClass = ImGui::TreeNodeEx(className.c_str(), treeNodeFlags);
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
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(7);
	}

	void AnimationEditorPanel::DrawAnimationOptions()
	{
        EG_PROFILE_FUNCTION();
		ImGui::PushStyleColor(ImGuiCol_PopupBg, BGCOLOR);
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, HOVEREDCOLOR);
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ACTIVEINPUTCOLOR);
		ImGui::PushStyleColor(ImGuiCol_Header, SELECTEDFRAMECOLOR);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, ROUNDING);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ROUNDING);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 5));

		if (m_OpenOptionsPopup)
		{
			ImGui::OpenPopup("AnimationOptions");
			m_OpenOptionsPopup = false;
		}

		if (ImGui::BeginPopup("AnimationOptions")) {
			if (m_SelectedFrames.second < 0) {
				if (ImGui::Selectable("Remove frame", false, ImGuiSelectableFlags_DontClosePopups)) {
					m_DeletedFrames.push_back(m_Anim->GetFrame(m_ClickedFrame)->GetFrameID());
					m_Anim->RemoveFrame(m_ClickedFrame);
					SetFrames();
				}
				if (ImGui::Selectable("Add function call", false, ImGuiSelectableFlags_DontClosePopups)) {
					m_OpenFunctionCallPopup = true;
				}
				if (ImGui::Selectable("Remove function call", false, ImGuiSelectableFlags_DontClosePopups)) {
					m_Anim->GetFrame(m_ClickedFrame)->SetFunctionCallName("");
					m_Anim->GetFrame(m_ClickedFrame)->SetClassName("");
				}
			}
			else {
				if (ImGui::Selectable("Swap frames", false, ImGuiSelectableFlags_DontClosePopups)) {
					m_Anim->SwapFrames(m_SelectedFrames.first, m_SelectedFrames.second);
					SetFrames();
				}
			}
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor(4);
	}

	void AnimationEditorPanel::DrawFrameTrack()
	{
        EG_PROFILE_FUNCTION();
		float buttonGap = 10;
		float rightMargin = 20;
		ImVec2 buttonSize = ImVec2(30, 30);
		float trackWidth = ImGui::GetContentRegionAvail().x - buttonSize.x - buttonGap - rightMargin;
		size_t animationDuration = m_Anim->GetAnimDuration();
		float startX = ImGui::GetCursorScreenPos().x + buttonSize.x + buttonGap ;
		float startY = ImGui::GetCursorScreenPos().y + 25;
		frameWidth = trackWidth / animationDuration < MINFRAMEWIDTH ? MINFRAMEWIDTH : trackWidth / animationDuration;

		DrawActionButtons(buttonSize, buttonGap);

		m_HoveredFrame = -1;
		m_FrameReleased = false;

		ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, BGCOLOR);
		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ACTIVEINPUTCOLOR);
		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, HOVEREDCOLOR);
		ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, BASEINPUTCOLOR);

		ImGui::SetCursorScreenPos(ImVec2(startX - 2, startY - 20));
		ImGui::BeginChild("FrameTrack", ImVec2(trackWidth + 16, 80), false, ImGuiWindowFlags_HorizontalScrollbar);
		float scroll = ImGui::GetScrollX();
		ImGui::SetCursorScreenPos(ImVec2(startX - scroll, startY));

		int i = 0;
		ImVec2 cursorPos, cursorPos2;
		float rectWidth = 0;

		for (auto& frame : m_Anim->GetFrames()) {
			rectWidth = frameWidth * frame->GetFrameDuration()-2;
			cursorPos = ImGui::GetCursorScreenPos();
			cursorPos2 = ImVec2(cursorPos.x + rectWidth, cursorPos.y + 40);

			HandleFrameHover(i, cursorPos, cursorPos2);

			ImVec2 textSize = ImGui::CalcTextSize(std::to_string(i).c_str(), NULL, NULL, NULL);
			ImVec2 textStart = ImVec2(cursorPos.x + rectWidth * 0.5f - textSize.x * 0.5f, cursorPos.y - 18);

			ImGui::GetWindowDrawList()->AddRectFilled(cursorPos, cursorPos2, GetFrameColor(i));

			if(i)
				ImGui::GetWindowDrawList()->AddLine(ImVec2(cursorPos.x - LINEOFFSET, cursorPos.y - 17), ImVec2(cursorPos.x - LINEOFFSET, cursorPos2.y+1), IM_COL32_WHITE, LINEWIDTH);

			if (frame->GetFunctionCallName() != "" && frame->GetClassname() != "")
				ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(cursorPos.x + rectWidth / 2, cursorPos.y + 20), 5, IM_COL32_WHITE);

			ImGui::GetWindowDrawList()->AddText(textStart, IM_COL32_WHITE, std::to_string(i).c_str());

			ImGui::SetCursorScreenPos(ImVec2(cursorPos.x, cursorPos.y));
			ImGui::InvisibleButton(("rectBtn" + std::to_string(i)).c_str(), ImVec2(rectWidth, 40));
			ImGui::SetCursorScreenPos(ImVec2(cursorPos.x + rectWidth + 2, cursorPos.y));

			HandleFrameLeftClick(i);
			HandleFrameRightClick(i);
			HandleFrameDrag(i);
			HandleFrameLeftClickRelease(i);

			i++;
		}

		ImGui::GetWindowDrawList()->AddRect(ImVec2(startX - 2, startY - 18), ImVec2(cursorPos2.x, startY + 42), IM_COL32_WHITE, 2, 0, 2);
		ImGui::EndChild();
		ImGui::PopStyleColor(4);

		if (m_HoveredFrame < 0)
		{
			hoverFrame = -1;
			hoverTime = 0.0f;
		}
		ImGui::SetCursorScreenPos(ImVec2(startX - buttonSize.x - buttonGap, ImGui::GetCursorScreenPos().y + 60));

		DrawAnimationOptions();
		//DrawFunctionCallPopup();
		//DrawFunctionInfoPopup();

		if (m_FrameReleased) {
			HandleMove(m_HoveredFrame);
			m_IsDragging = false;
			m_DraggingIndex = -1;
		}

		HandleResize();

	}

	void AnimationEditorPanel::DrawFunctionInfoPopup()
	{
        EG_PROFILE_FUNCTION();
		if(hoverTime < hoverTimeMax)
			return;

		ImGui::PushStyleColor(ImGuiCol_PopupBg, BGCOLOR);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, BASEINPUTCOLOR);
		ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, ROUNDING);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ROUNDING);
		ImGui::OpenPopup("Frame Info Popup");

		if (ImGui::BeginPopup("Frame Info Popup")) {
			ImGui::Text("Frame Length: %d", m_Anim->GetFrame(hoverFrame)->GetFrameDuration());
			if (m_Anim->GetFrame(hoverFrame)->GetClassname() != "")
			{
				ImGui::Text("Function name", m_Anim->GetFrame(hoverFrame)->GetFunctionCallName().c_str());
				ImGui::Text("Class name", m_Anim->GetFrame(hoverFrame)->GetClassname().c_str());
			}
		}
		ImGui::EndPopup();
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);

	}

	void AnimationEditorPanel::DrawFrameRate()
	{
        EG_PROFILE_FUNCTION();
		ImGui::PushStyleColor(ImGuiCol_FrameBg, BASEINPUTCOLOR);
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, HOVEREDCOLOR);
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, SELECTEDFRAMECOLOR);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ROUNDING);
		ImGui::DragInt("Frame Rate", m_Anim->GetFrameRatePtr(), 1.0f, 1.0f, 500);
		ImGui::PopStyleVar();
		ImGui::PopStyleColor(3);
	}

	void AnimationEditorPanel::HandleFrameHover(int frameIndex, const ImVec2& cursorPos, const ImVec2& cursorPos2)
	{
        EG_PROFILE_FUNCTION();
		if (ImGui::IsMouseHoveringRect(cursorPos, cursorPos2))

		{
			m_HoveredFrame = frameIndex;
			if (frameIndex != hoverFrame)
			{
				hoverTime = 0.0f;
			}
			else
			{
				hoverTime += ImGui::GetIO().DeltaTime;
			}
			hoverFrame = frameIndex;
		}
	}

	void AnimationEditorPanel::HandleFrameLeftClick(int clickedFrameIndex)
	{
        EG_PROFILE_FUNCTION();
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
        EG_PROFILE_FUNCTION();
		if (m_PlayAnimation)
			return;
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
			m_OpenOptionsPopup = true;
			m_ClickedFrame = clickedFrameIndex;
		}
	}

	void AnimationEditorPanel::HandleFrameDrag(int clickedFrameIndex)
	{
        EG_PROFILE_FUNCTION();
		if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
			if (!m_IsDragging) {
				m_IsDragging = true;
				m_DraggingIndex = clickedFrameIndex;
				if (m_Resize)
				{
					m_FrameStartDuration = m_Anim->GetFrame(clickedFrameIndex)->GetFrameDuration();
					ImGui::ResetMouseDragDelta();
					m_ResizeOffset = 0;
				}
			}
		}
	}

	void AnimationEditorPanel::HandleFrameLeftClickRelease(int clickedFrameIndex)
	{
        EG_PROFILE_FUNCTION();
		if (!ImGui::IsMouseReleased(ImGuiMouseButton_Left) )
			return;

		m_FrameReleased = true;
	}

	void AnimationEditorPanel::HandleResize()
	{
        EG_PROFILE_FUNCTION();
		if(!m_Resize)
			return;
		if (!m_IsDragging || m_DraggingIndex == -1)
			return;

		m_ResizeOffset += ImGui::GetIO().MouseDelta.x;
		float estNewDuration = std::max(1, (int)(m_FrameStartDuration + m_ResizeOffset / (frameWidth)));

		if(estNewDuration != m_Anim->GetFrame(m_DraggingIndex)->GetFrameDuration())
			m_Anim->GetFrame(m_DraggingIndex)->SetFrameDuration(estNewDuration);
		SetFrames();

	}

	void AnimationEditorPanel::HandleMove(int moveTo)
	{
        EG_PROFILE_FUNCTION();
		if (!m_Move)
			return;
		if (!m_IsDragging || m_DraggingIndex == -1)
			return;

		if (moveTo != m_DraggingIndex) {
			m_Anim->MoveFrame(m_DraggingIndex, moveTo);
			SetFrames();
		}

	}

	void AnimationEditorPanel::DrawActionButtons(const ImVec2& buttonSize, float gap)
	{
        EG_PROFILE_FUNCTION();
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5);
		ImVec4 moveIconTint = m_Move ? ImVec4(1, 1, 1, 1) : ImVec4(0.75, 0.75, 0.75, 1);
		ImVec4 resizeIconTint = m_Resize ? ImVec4(1, 1, 1, 1) : ImVec4(0.75, 0.75, 0.75, 1);
		if (ImGui::ImageButton((ImTextureID)(IconLoader::GetIcon(Icons::AnimationEditor_LengthChange)->GetRendererID()), buttonSize, ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), resizeIconTint)) {
			if (m_Move)
				m_Move = false;
			m_Resize = !m_Resize;
		}
		ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + gap));
		if (ImGui::ImageButton((ImTextureID)(IconLoader::GetIcon(Icons::AnimationEditor_MoveIcon)->GetRendererID()), buttonSize, ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0, 0, 0, 0), moveIconTint)) {
			if (m_Resize)
				m_Resize = false;
			m_Move = !m_Move;
		}
		ImGui::PopStyleVar();
	}

	void AnimationEditorPanel::DrawExitButtons()
	{
        EG_PROFILE_FUNCTION();
		ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + BASEGAP));

		ImGui::PushStyleColor(ImGuiCol_Button, BASEINPUTCOLOR);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HOVEREDCOLOR);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ACTIVEINPUTCOLOR);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ROUNDING);

		ImVec2 buttonSize = ImVec2(90, 40);

		if (ImGui::Button("Save", buttonSize)) {
			m_Anim->Save();
			FrameData::DeleteAssets(m_DeletedFrames);
			ShowAnimationEditorPanel(false);
			ResetData();
		}

		ImGui::SameLine();

		ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x + BASEGAP, ImGui::GetCursorScreenPos().y));

		if (ImGui::Button("Close", buttonSize)) {
			ShowAnimationEditorPanel(false);
			ResetData();
		}

		ImGui::PopStyleVar();
		ImGui::PopStyleColor(3);
	}

	uint32_t AnimationEditorPanel::GetFrameColor(int frame)
	{
        EG_PROFILE_FUNCTION();
		if (m_IsDragging && m_DraggingIndex == frame)
			return ACTIVEINPUTCOLOR;
		if (IsFrameSelected(frame))
			return SELECTEDFRAMECOLOR;
		if(m_HoveredFrame == frame)
			return HOVEREDCOLOR;
		return BASEINPUTCOLOR;
	}
}
