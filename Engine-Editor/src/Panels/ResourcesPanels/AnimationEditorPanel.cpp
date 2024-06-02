#include "AnimationEditorPanel.h"
#include "Imgui/imgui.h"

namespace eg {
	bool AnimationEditorPanel::OpenAnimationEditorPanel(UUID asset) {
		ShowAnimationEditorPanel(true);
		m_Anim = Animation::Create(asset);
		return m_Anim != nullptr;
	}

	void AnimationEditorPanel::OnImGuiRender() {
		if (!m_ShowAnimationEditorPanel)
			return;

		ImGui::Begin("Animation Editor Panel", nullptr, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::SetWindowSize(ImVec2(1000, 600));

		/*float animationTime = 0.0f;
		std::vector<Animation::FrameData> keyFrames;
		for (auto anim : m_Anim->GetFrames()) {
			keyFrames.push_back(anim);
			animationTime += anim.FrameDuration;
		}*/

		for (auto anim : m_Anim->GetFrames()) {
			auto cordsTopLeft = anim.SubTexture.get()->GetCoords(0);
			auto cordsBottomRight = anim.SubTexture.get()->GetCoords(2);
			SubTexture2D* imageSubTexture = anim.SubTexture.get();
			float width = anim.FrameDuration * 100;

			ImVec2 cursorPos = ImGui::GetCursorScreenPos();

			ImGui::Image((ImTextureID)imageSubTexture->GetTexture()->GetRendererID(), ImVec2(50, 50), ImVec2(cordsTopLeft.x, cordsBottomRight.y), ImVec2(cordsBottomRight.x, cordsTopLeft.y));
			
			ImGui::SameLine(0.0f, 0.0f);

			if (width > 50) {
				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(cursorPos.x + 50, cursorPos.y), ImVec2(cursorPos.x + width, cursorPos.y + 50), IM_COL32(207, 159, 255, 255));
			}

			/*ImGui::GetWindowDrawList()->AddLine(ImVec2(cursorPos.x + width, cursorPos.y), ImVec2(cursorPos.x + width, cursorPos.y + 50), IM_COL32(255, 255, 255, 255));*/

			ImGui::SetCursorScreenPos(ImVec2(cursorPos.x + width + 10, cursorPos.y));

			if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
				ImGui::OpenPopup("KeyframesOptions");

			if (ImGui::BeginPopupContextItem("KeyframesOptions")) {
				if(!anim.isKeyFrame){
					if (ImGui::MenuItem("Add keyframe")){
						anim.isKeyFrame = true;
					}
				}
				else {
					if (ImGui::MenuItem("Remove keyframe")){
						anim.isKeyFrame = false;
					}
				}
				ImGui::EndPopup();
			}
		}

		ImGui::SetCursorPos(ImVec2(15, ImGui::GetWindowHeight() - 50));
		if (ImGui::Button("Close")) {
			ShowAnimationEditorPanel(false);
		}
		ImGui::End();
			
	}

}