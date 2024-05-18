#include "AnimationEditorPanel.h"
#include "Imgui/imgui.h"

namespace eg {
	bool AnimationEditorPanel::OpenAnimationEditorPanel(std::vector<std::pair<int,int>>& frames) {
		ShowAnimationEditorPanel(true);
		m_frames = std::move(frames);
		return true;
	}

	void AnimationEditorPanel::OnImGuiRender() {
		if (!m_ShowAnimationEditorPanel)
			return;
		ImGui::Begin("Animation Editor Panel");
		ImGui::SetWindowSize(ImVec2(500,300));

		ImGui::End();
		
	}

}