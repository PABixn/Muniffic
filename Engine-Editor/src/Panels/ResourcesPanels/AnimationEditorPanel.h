#pragma once
#include "Engine.h"

namespace eg {
	class AnimationEditorPanel {
	public:
		bool OpenAnimationEditorPanel(std::vector<std::pair<int, int>>& frames);
		void OnImGuiRender();
	private:
		void ShowAnimationEditorPanel(bool show) { m_ShowAnimationEditorPanel = show; };
	private:
		std::vector<std::pair<int, int>> m_frames;
		bool m_ShowAnimationEditorPanel = false;
	};
}