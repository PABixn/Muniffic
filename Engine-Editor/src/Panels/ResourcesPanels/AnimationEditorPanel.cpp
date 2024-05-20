#include "AnimationEditorPanel.h"
#include "Imgui/imgui.h"

namespace eg {
	bool AnimationEditorPanel::OpenAnimationEditorPanel() {
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