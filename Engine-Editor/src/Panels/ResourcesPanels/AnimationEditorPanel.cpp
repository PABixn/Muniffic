#include "AnimationEditorPanel.h"
#include "Imgui/imgui.h"

namespace eg {
	static bool isDisplayed = false;
	static Animation::FrameData displayedAnim;
	static int clickedFrame = -1;
	bool AnimationEditorPanel::OpenAnimationEditorPanel(UUID asset) {
		ShowAnimationEditorPanel(true);
		m_Anim = Animation::Create(asset);
		m_PlayIcon = Texture2D::Create("resources/icons/PlayButton.png");
		m_StopIcon = Texture2D::Create("resources/icons/StopButton.png");
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
			}
			else {
				m_Anim->Play();
			}
		}

		ImGui::PopStyleColor(3);

		DrawAnimationPreview();

		int i = 0;
		for (auto anim : m_Anim->GetFrames()) {
			auto cordsTopLeft = anim.SubTexture.get()->GetCoords(0);
			auto cordsBottomRight = anim.SubTexture.get()->GetCoords(2);
			SubTexture2D* imageSubTexture = anim.SubTexture.get();
			float width = anim.FrameDuration * 100;

			ImVec2 cursorPos = ImGui::GetCursorScreenPos();

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursorPos.x, cursorPos.y), ImVec2(cursorPos.x + 20, cursorPos.y + 40), IM_COL32(207, 159, 255, 255));
			ImGui::SetCursorScreenPos(ImVec2(cursorPos.x, cursorPos.y));
			ImGui::InvisibleButton(("rectBtn" + std::to_string(i)).c_str(), ImVec2(20, 40));

			/*ImGui::GetWindowDrawList()->AddLine(ImVec2(cursorPos.x + width, cursorPos.y), ImVec2(cursorPos.x + width, cursorPos.y + 50), IM_COL32(255, 255, 255, 255));*/

			if (ImGui::IsItemClicked(ImGuiMouseButton_Left)){
				if(clickedFrame < 0)
					clickedFrame = i;
				else if(clickedFrame == i)
					isDisplayed = !isDisplayed;

				displayedAnim = anim;
				clickedFrame = i;
			}

			if (isDisplayed)
				DrawAnimationPreview(displayedAnim);

			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
				Menu(anim);

			ImGui::SetCursorScreenPos(ImVec2(cursorPos.x + 22, cursorPos.y));
			i++;
		}

		ImGui::SetCursorPos(ImVec2(15, ImGui::GetWindowHeight() - 50));
		if (ImGui::Button("Close")) {
			ShowAnimationEditorPanel(false);
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

	void AnimationEditorPanel::DrawAnimationPreview(Animation::FrameData anim) {
		if (anim.SubTexture == nullptr) {
			anim = m_Anim->GetFrames()[0];
		}
		auto cordsTopLeft = anim.SubTexture.get()->GetCoords(0);
		auto cordsBottomRight = anim.SubTexture.get()->GetCoords(2);
		SubTexture2D* imageSubTexture = anim.SubTexture.get();
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 300) * 0.5f);
		ImGui::SetCursorPosY(100);


		if (m_Anim->GetFrameCount() > 0) {
			ImGui::Image((ImTextureID)imageSubTexture->GetTexture()->GetRendererID(), ImVec2(300, 300), ImVec2(cordsTopLeft.x, cordsBottomRight.y), ImVec2(cordsBottomRight.x, cordsTopLeft.y));
		}
	}
}