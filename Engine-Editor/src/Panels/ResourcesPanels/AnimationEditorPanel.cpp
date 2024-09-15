#include "AnimationEditorPanel.h"
#include "Imgui/imgui.h"
#include "Engine/Scripting/ScriptEngine.h"
namespace eg {
	static bool isDisplayed = false;
	static Ref<FrameData> displayedFrame;
	static int clickedFrame = -1; 
	static bool isDragging = false;
	static int draggingIndex = -1;
	//static ImVec2 initialMousePos;
	//static ImVec2 initialRectPos;

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

		for (auto& anim : *(m_Anim->GetFrames())) {
			m_FramesData.emplace_back(anim);
		}
		SetFrames();

		return m_Anim != nullptr;
	}

	void AnimationEditorPanel::OnImGuiRender() {
		if (!m_ShowAnimationEditorPanel)
			return;

		ImGui::Begin("Animation Editor Panel", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::SetWindowSize(ImVec2(1000, 800));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.5f - 50 * 0.5f);
		if (ImGui::ImageButton((ImTextureID)(*m_Anim->IsPlayingPtr() ? m_StopIcon->GetRendererID() : m_PlayIcon->GetRendererID()), ImVec2(50, 50))) {
			if (*m_Anim->IsPlayingPtr()) {
				m_Anim->Stop();
				//m_ShowInitialFrame = true;
				m_PlayAnimation = false;
			}
			else {
				m_Anim->Play();
				m_PlayAnimation = true;
			}
		}

		if (m_PlayAnimation)
			DrawAnimationPreview();

		ImGui::PopStyleColor(3);

		if (!m_PlayAnimation)
			DrawFramePreview(m_Anim->GetFrame(0), true);

		ImGui::BeginChild("Animation timeline", ImVec2(ImGui::GetWindowSize().x, 100), true);
		for (float i = ImGui::GetWindowPos().x + 6; i < ImGui::GetWindowPos().x + ImGui::GetWindowSize().x; i += 22.0f) {
			ImGui::GetWindowDrawList()->AddLine(ImVec2(i, ImGui::GetWindowPos().y), ImVec2(i, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y), IM_COL32(255, 255, 255, 255), 0.8f);
		}

		int i = 0;
		for (auto frame : m_FramesData) {
			ImVec2 cursorPos = ImGui::GetCursorScreenPos();
			float rectWidth = 20 * frame->GetFrameDuration() + 2 * (frame->GetFrameDuration() - 1);

			if (m_FramesToSwap.find(i) != m_FramesToSwap.end())
				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursorPos.x, cursorPos.y), ImVec2(cursorPos.x + rectWidth, cursorPos.y + 40), IM_COL32(65, 51, 122, 255));
			else if(isDragging && draggingIndex == i)
				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursorPos.x, cursorPos.y), ImVec2(cursorPos.x + rectWidth, cursorPos.y + 40), IM_COL32(255, 105, 180, 255));
			else
				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursorPos.x, cursorPos.y), ImVec2(cursorPos.x + rectWidth, cursorPos.y + 40), IM_COL32(194, 239, 235, 255));
			ImGui::SetCursorScreenPos(ImVec2(cursorPos.x, cursorPos.y));
			ImGui::InvisibleButton(("rectBtn" + std::to_string(i)).c_str(), ImVec2(rectWidth, 40));
			ImGui::SetCursorScreenPos(ImVec2(cursorPos.x + rectWidth + 2, cursorPos.y));

			if (!m_PlayAnimation) {
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
					if (clickedFrame < 0) {
						clickedFrame = i;
					}
					else if (clickedFrame == i) {
						isDisplayed = !isDisplayed;
						//m_ShowInitialFrame = true;
					}
					if (clickedFrame != i) {
						//m_ShowInitialFrame = false;
					}
					displayedFrame = frame;
					clickedFrame = i;
					if (!m_Resize && !m_Move) {
						if (m_FramesToSwap.size() < 2 && m_FramesToSwap.find(i) == m_FramesToSwap.end()) {
							m_FramesToSwap.insert({ i,frame });
						}
						else if (m_FramesToSwap.find(i) != m_FramesToSwap.end()) {
							m_FramesToSwap.erase(i);
						}
					}
					else if (m_Move || m_Resize) {
						m_FramesToSwap.clear();
					}
				}

				if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
					ImGui::OpenPopup("AnimationOptions");
					m_ClickedFrame = i;
				}
			}

			if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
				if (!isDragging) {
					isDragging = true;
					draggingIndex = i;
				}
			}

			if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && draggingIndex == i) {
				isDragging = false;
				draggingIndex = -1;
			}

			i++;
		}

		if (ImGui::BeginPopup("AnimationOptions")) {
			if (m_FramesToSwap.size() != 2){
				if (ImGui::MenuItem("Remove frame")) {
					m_FramesData.erase(m_FramesData.begin() + m_ClickedFrame);
					SetFrames();
				}
				if (ImGui::MenuItem("Add function call")) {
					ImGui::OpenPopup("FunctionCall");
				}
			}
			else{
				if(ImGui::MenuItem("Swap frames")){
					auto anim1 = m_FramesToSwap.begin();
					m_FramesData[anim1->first] = m_FramesToSwap.rbegin()->second;
					m_FramesData[m_FramesToSwap.rbegin()->first] = anim1->second;
					SetFrames();
					m_FramesToSwap.clear();
				}
			}
			ImGui::EndPopup();
		}


		if (ImGui::BeginPopup("FunctionCall")) {
			ImGui::Text("Function name:");
			static char functionName[128];
			ImGui::InputText("##functionName", functionName, 128);
			bool opened = ImGui::TreeNodeEx("Available Function calls:", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_MoreSpaceBetweenTextAndArrow);
			if (opened)
			{
					const auto& scriptMethods = ScriptEngine::GetAllScriptMethodMaps();
					for (auto& [key, value] : scriptMethods) {
						bool openedClass = ImGui::TreeNodeEx(key.c_str(), ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_MoreSpaceBetweenTextAndArrow);
						if (!openedClass)
							continue;
						for (auto& [key2, value2] : value) {
							if(!ScriptEngine::isMethodInternal(key2))
								if (ImGui::Button(key2.c_str()))
								{
									m_FramesData[m_ClickedFrame]->SetFunctionCallName(key2);
									m_FramesData[m_ClickedFrame]->SetClassName(key);
									ImGui::CloseCurrentPopup();
								}
					}
				}
				ImGui::TreePop();
			}
			ImGui::EndPopup();
		}

		if (isDragging && draggingIndex != -1) {
			int deltaX = ImGui::GetIO().MouseDelta.x;
			if (m_Resize) {
				float estNewDuration = std::max(1, m_FramesData[draggingIndex]->GetFrameDuration() + deltaX / 2);
				m_FramesData[draggingIndex]->SetFrameDuration(estNewDuration);
				SetFrames();
			}
			else if (m_Move) {
				//wtf is this
				float estNewIndex = draggingIndex + (float)deltaX / 2;
				int newIndex = (int)(round(estNewIndex));
				newIndex = std::clamp(newIndex, 0, static_cast<int>(m_FramesData.size()) - 1);

				if (newIndex != draggingIndex) {
					//Animation::FrameData temp = m_FramesData[draggingIndex];
					//m_FramesData.erase(m_FramesData.begin() + draggingIndex);
					//m_FramesData.insert(m_FramesData.begin() + newIndex, temp);
					//draggingIndex = newIndex;
					SetFrames();
				}
			}
		}

		ImGui::EndChild();

		if (!m_PlayAnimation && isDisplayed) {
			DrawFramePreview(displayedFrame);
		}

		ImGui::DragInt("Frame Rate %f:", m_Anim->GetFrameRatePtr(), 1.0f, 1.0f, 500);

		if (ImGui::ImageButton((ImTextureID)(m_Resize ? m_LenghtSelectedIcon->GetRendererID() : m_LenghtIcon->GetRendererID()), ImVec2(50, 50),ImVec2(0,0), ImVec2(1,1),0)) {
			if(m_Move)
				m_Move = false;
			m_Resize = !m_Resize;
		}
		ImGui::SameLine();
		if(ImGui::ImageButton((ImTextureID)(m_Move ? m_MoveSelectedIcon->GetRendererID() : m_MoveIcon->GetRendererID()), ImVec2(50, 50), ImVec2(0, 0), ImVec2(1, 1), 0)){
			if(m_Resize)
				m_Resize = false;
			m_Move = !m_Move;
		}

		//ImGui::SetCursorPos(ImVec2(15, ImGui::GetWindowHeight() - 100));
		if (ImGui::Button("Save")) {
			m_Anim->Save();
			ShowAnimationEditorPanel(false);
			m_FramesData.clear();
			m_Anim = nullptr;
		}
		if (ImGui::Button("Close")) {
			ShowAnimationEditorPanel(false);
			m_FramesData.clear();
			m_Anim = nullptr;
		}
		ImGui::End();
			
	}

	void AnimationEditorPanel::DrawFramePreview(Ref<FrameData> frame,bool addSpace) {
		Ref<SubTexture2D> subTexture = frame->GetSubTexture();
		if (subTexture == nullptr) {
			frame = (*m_Anim->GetFrames())[0];
		}
		auto minCoords = subTexture.get()->GetMin();
		auto maxCoords = subTexture.get()->GetMax();
		SubTexture2D* imageSubTexture = subTexture.get();
		float spaceY = ImGui::GetCursorPosY();
		if (addSpace)
			spaceY = ImGui::GetCursorPosY() + 330;
		ImVec2 cursorPos = { ImGui::GetCursorPos().x, spaceY };
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 300) * 0.5f);
		ImGui::SetCursorPosY(100);

		if (m_Anim->GetFrameCount() > 0) {
			ImGui::Image((ImTextureID)imageSubTexture->GetTexture()->GetRendererID(), ImVec2(300, 300), ImVec2(minCoords.x, maxCoords.y), ImVec2(maxCoords.x, minCoords.y));
		}

		ImGui::SetCursorPos(cursorPos);
	}

	void AnimationEditorPanel::DrawAnimationPreview() {
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 300) * 0.5f);
		ImGui::SetCursorPosY(100);
		float spaceY = ImGui::GetCursorPos().y + 322;
		int i = 0;
		Ref<SubTexture2D> subTexture = m_Anim->GetFrame()->GetSubTexture();
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

	void AnimationEditorPanel::SetFrames() {
		m_Anim->ClearFrames();

		for (auto& anim : m_FramesData) {
			m_Anim->AddFrame(anim);
		}

		m_Anim->SetFrame(0);
	}
	void AnimationEditorPanel::ResetData()
	{
		m_FramesToSwap.clear();
		m_FramesData.clear();
		m_Anim = nullptr;
		m_PlayAnimation = false;
		m_ShowLongerFrame = false;
		m_ShowInitialFrame = true;
		m_Resize = false;
		m_Move = false;
		m_ClickedFrame = -1;
		clickedFrame = -1;
		isDisplayed = false;
		displayedFrame = CreateRef<FrameData>();
		isDragging = false;
		draggingIndex = -1;
	}
}