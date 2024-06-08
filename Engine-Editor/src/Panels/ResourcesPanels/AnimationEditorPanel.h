#pragma once
#include "Engine.h"

namespace eg {
	class AnimationEditorPanel {
	public:
		bool OpenAnimationEditorPanel(UUID asset);
		void OnImGuiRender();
		void Menu(Animation::FrameData anim);
		void DrawFramePreview(Animation::FrameData anim = {});
		void DrawAnimationPreview();
		void Update(float dt) { m_Anim->Update(dt, 1.0f); };
		bool IsAnimationEditorOpen() { return m_ShowAnimationEditorPanel; };
		void SetFrames();
	private:
		void ShowAnimationEditorPanel(bool show) { m_ShowAnimationEditorPanel = show; };
	private:
		bool m_ShowAnimationEditorPanel = false;
		bool m_PlayAnimation;
		bool m_ShowLongerFrame = false;
		Ref<Animation> m_Anim;
		Ref<Texture2D> m_PlayIcon;
		Ref<Texture2D> m_StopIcon;
		Resource* m_LoadedResource;
		std::vector<Animation::FrameData> m_FramesData;
	};
}