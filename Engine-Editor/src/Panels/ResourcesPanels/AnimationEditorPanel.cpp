#include "AnimationEditorPanel.h"
#include "Imgui/imgui.h"

namespace eg {
	static bool isDisplayed = false;
	static Animation::FrameData displayedAnim;
	static int clickedFrame = -1; 
	static bool isDragging = false;
	static int draggingIndex = -1;
	//static ImVec2 initialMousePos;
	//static ImVec2 initialRectPos;

	bool AnimationEditorPanel::OpenAnimationEditorPanel(UUID asset) {
		ShowAnimationEditorPanel(true);
		m_Anim = Animation::Create(asset);
		m_PlayIcon = Texture2D::Create("resources/icons/PlayButton.png");
		m_StopIcon = Texture2D::Create("resources/icons/StopButton.png");

		for (auto& anim : m_Anim->GetFrames()) {
			m_FramesData.emplace_back(anim);
			SetFrames();
		}

		return m_Anim != nullptr;
	}

	void AnimationEditorPanel::OnImGuiRender() {
		if (!m_ShowAnimationEditorPanel)
			return;

		ImGui::Begin("Animation Editor Panel", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::SetWindowSize(ImVec2(1000, 600));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		ImGui::SetCursorPosX(ImGui::GetWindowSize().x * 0.5f - 50 * 0.5f);
		if (ImGui::ImageButton((ImTextureID)(*m_Anim->IsPlayingPtr() ? m_StopIcon->GetRendererID() : m_PlayIcon->GetRendererID()), ImVec2(50, 50))) {
			if (*m_Anim->IsPlayingPtr()) {
				m_Anim->Stop();
				m_PlayAnimation = false;
			}
			else {
				m_Anim->Play();
				m_PlayAnimation = true;
			}
		}

		if(m_PlayAnimation)
			DrawAnimationPreview();

		ImGui::PopStyleColor(3);

		if(!m_PlayAnimation)
			DrawFramePreview(m_Anim->GetFrame(0));

		/*for (auto anim : m_Anim->GetFrames()) {
			auto minCoords = anim.SubTexture.get()->GetMin();
			auto maxCoords = anim.SubTexture.get()->GetMax();;
			SubTexture2D* imageSubTexture = anim.SubTexture.get();
			float width = anim.FrameDuration * 100;

			ImVec2 cursorPos = ImGui::GetCursorScreenPos();
			ImVec2 rectMin = ImVec2(cursorPos.x, cursorPos.y);
			ImVec2 rectMax = ImVec2(cursorPos.x + 20, cursorPos.y + 40);

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursorPos.x, cursorPos.y), ImVec2(cursorPos.x + 20 * anim.FrameDuration, cursorPos.y + 40), IM_COL32(207, 159, 255, 255));
			ImGui::SetCursorScreenPos(ImVec2(cursorPos.x, cursorPos.y));
			ImGui::InvisibleButton(("rectBtn" + std::to_string(i)).c_str(), ImVec2(20, 40));

			//DrawAnimationPreview(anim);

			ImGui::Image((ImTextureID)imageSubTexture->GetTexture()->GetRendererID(), ImVec2(300, 300), ImVec2(minCoords.x, maxCoords.y), ImVec2(maxCoords.x, minCoords.y));

			if (!m_PlayAnimation) {
				if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
					if (clickedFrame < 0) {
						clickedFrame = i;
					}
					else if (clickedFrame == i) {
						isDisplayed = !isDisplayed;
					}

					displayedAnim = anim;
					clickedFrame = i;
				}

				if (isDisplayed) {
					DrawFramePreview(displayedAnim);
					ImGui::GetWindowDrawList()->AddRectFilled(rectMin, rectMax, IM_COL32(107, 159, 255, 255));
				}

				// Handle dragging and duplicating frame
				if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
					if (!isDragging) {
						isDragging = true;
						initialMousePos = ImGui::GetMousePos();
						initialRectPos = rectMin;
					}
					else {
						ImVec2 currentMousePos = ImGui::GetMousePos();
						//if (currentMousePos.x - initialMousePos.x > 20) {
							// Duplicate frame
							anim.FrameDuration += 1;
							// Draw new rectangle representing the extended frame
							ImVec2 newRectMin = ImVec2(rectMax.x + 2, rectMin.y);
							ImVec2 newRectMax = ImVec2(newRectMin.x + 20, rectMin.y + 40);
							ImGui::GetWindowDrawList()->AddRectFilled(newRectMin, newRectMax, IM_COL32(107, 159, 255, 255));
							isDragging = false;
						//}
					}
				}
			}

			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
				ImGui::OpenPopup("KeyframesOptions");

			if (ImGui::BeginPopupContextItem("KeyframesOptions")) {
				if (!anim.isKeyFrame) {
					if (ImGui::MenuItem("Add keyframe")) {
						anim.isKeyFrame = true;
					}
				}
				else if(anim.isKeyFrame) {
					if (ImGui::MenuItem("Remove keyframe")) {
						anim.isKeyFrame = false;
					}
				}
				ImGui::EndPopup();
			}

			ImGui::SetCursorScreenPos(ImVec2(cursorPos.x + 22, cursorPos.y));

			if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) && ImGui::IsMouseHoveringRect(cursorPos, ImVec2(cursorPos.x + 20, cursorPos.y + 40))) {
				m_ShowLongerFrame = true;
			}
			
			if (m_ShowLongerFrame) {
				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursorPos.x, cursorPos.y), ImVec2(cursorPos.x + 20, cursorPos.y + 40), IM_COL32(107, 159, 255, 255));
				anim.FrameDuration += 1;
				ImGui::SetCursorScreenPos(ImVec2(cursorPos.x + 22, cursorPos.y));
			}
			i++;
		}*/

		ImGui::BeginChild("Animation timeline", ImVec2(ImGui::GetWindowSize().x, 100), true);
		for (float x = ImGui::GetWindowPos().x + 6; x < ImGui::GetWindowPos().x + ImGui::GetWindowSize().x; x += 22.0f) {
			ImGui::GetWindowDrawList()->AddLine(ImVec2(x, ImGui::GetWindowPos().y), ImVec2(x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y ), IM_COL32(255, 255, 255, 255));
		}

		int i = 0;
		for(auto anim : m_FramesData) {
			ImVec2 cursorPos = ImGui::GetCursorScreenPos(); 
			float rectWidth = 21 * anim.FrameDuration;// + 1 * (anim.FrameDuration - 1);

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursorPos.x, cursorPos.y), ImVec2(cursorPos.x + rectWidth, cursorPos.y + 40), IM_COL32(207, 159, 255, 255));
			ImGui::SetCursorScreenPos(ImVec2(cursorPos.x, cursorPos.y));
			ImGui::InvisibleButton(("rectBtn" + std::to_string(i)).c_str(), ImVec2(rectWidth, 40));
			ImGui::SetCursorScreenPos(ImVec2(cursorPos.x + rectWidth + 1, cursorPos.y));
			
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

		if (isDragging && draggingIndex != -1) {
			int deltaX = ImGui::GetIO().MouseDelta.x;
			m_FramesData[draggingIndex].FrameDuration += deltaX/2;
			m_FramesData[draggingIndex].FrameDuration = std::max(1, m_FramesData[draggingIndex].FrameDuration);
			SetFrames();
		}

		ImGui::EndChild();

		ImGui::SetCursorPos(ImVec2(15, ImGui::GetWindowHeight() - 100));
		if (ImGui::Button("Save")) {
			ShowAnimationEditorPanel(false);
			m_FramesData.clear();
			
		}
		if (ImGui::Button("Close")) {
			ShowAnimationEditorPanel(false);
			m_FramesData.clear();
		}
		ImGui::End();
			
	}

	void AnimationEditorPanel::Menu(Animation::FrameData anim) {
		ImGui::OpenPopup("KeyframesOptions");

		if (ImGui::BeginPopupContextItem("KeyframesOptions")) {
			if (!anim.isKeyFrame) {
				if (ImGui::MenuItem("Add keyframe")) {
					anim.isKeyFrame = true;
				}
			}
			else {
				if (ImGui::MenuItem("Remove keyframe")) {
					anim.isKeyFrame = false;
				}
			}
			ImGui::EndPopup();
		}
	}

	void AnimationEditorPanel::DrawFramePreview(Animation::FrameData anim) {
		if (anim.SubTexture == nullptr) {
			anim = m_Anim->GetFrames()[0];
		}
		auto minCoords = anim.SubTexture.get()->GetMin();
		auto maxCoords = anim.SubTexture.get()->GetMax();
		SubTexture2D* imageSubTexture = anim.SubTexture.get();
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 300) * 0.5f);
		ImGui::SetCursorPosY(100);

		if (m_Anim->GetFrameCount() > 0) {
			ImGui::Image((ImTextureID)imageSubTexture->GetTexture()->GetRendererID(), ImVec2(300, 300), ImVec2(minCoords.x, maxCoords.y), ImVec2(maxCoords.x, minCoords.y));
		}
	}

	void AnimationEditorPanel::DrawAnimationPreview() {
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 300) * 0.5f);
		ImGui::SetCursorPosY(100);
		int i = 0;
		if (m_Anim->GetFrameCount() > 0) {
			ImGui::Image(
				(void*)m_Anim->GetFrame().SubTexture->GetTexture()->GetRendererID(),
				ImVec2(300, 300),
				ImVec2(
					m_Anim->GetFrame().SubTexture->GetMin().x,
					m_Anim->GetFrame().SubTexture->GetMax().y),
				ImVec2(
					m_Anim->GetFrame().SubTexture->GetMax().x,
					m_Anim->GetFrame().SubTexture->GetMin().y)
			);
			
		}
	}

	void AnimationEditorPanel::SetFrames() {
		m_Anim->ClearFrames();

		for (auto& anim : m_FramesData) {
			m_Anim->AddFrame(anim);
		}

		m_Anim->SetFrame(0);
	}
}