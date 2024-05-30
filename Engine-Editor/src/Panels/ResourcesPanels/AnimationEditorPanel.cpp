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
		ImGui::SetWindowSize(ImVec2(1000,600));

		float animationTime = 0.0f;
		for (auto anim : m_Anim->GetFrames()) {
			animationTime += anim.FrameDuration;
		}
		
		float cursorX = 0.0f;
		for (auto& anim : m_Anim->GetFrames()) {
			ImGui::SameLine();
			auto cordsTopLeft = anim.SubTexture.get()->GetCoords(0);
			auto cordsBottomRight = anim.SubTexture.get()->GetCoords(2);
			SubTexture2D* imageSubTexture = anim.SubTexture.get();
			ImGui::Image((ImTextureID)imageSubTexture->GetTexture()->GetRendererID(), ImVec2(50, 50), ImVec2(cordsTopLeft.x, cordsBottomRight.y), ImVec2(cordsBottomRight.x, cordsTopLeft.y));
		}

		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 50);
		if (ImGui::Button("Close")) {
			ShowAnimationEditorPanel(false);
		}
		ImGui::End();
			
	}

}