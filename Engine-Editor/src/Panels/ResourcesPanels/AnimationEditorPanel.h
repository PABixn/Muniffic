#pragma once
#include "Engine.h"

namespace eg {
	class AnimationEditorPanel {
	public:
		bool OpenAnimationEditorPanel(UUID asset);
		void OnImGuiRender();
	private:
		void ShowAnimationEditorPanel(bool show) { m_ShowAnimationEditorPanel = show; };
	private:
		bool m_ShowAnimationEditorPanel = false;
		Ref<Animation> m_Anim;
		Resource* m_LoadedResource;
	};
}