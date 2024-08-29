#pragma once
#include "Engine.h"

namespace eg {
	class AnimationEditorPanel {
	public:
		bool OpenAnimationEditorPanel(UUID asset);
		void OnImGuiRender();
		void DrawFramePreview(Animation::FrameData anim = {}, bool addSpace = false);
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
		bool m_ShowInitialFrame = true;
		bool m_Selection = true;
		std::map<int, Animation::FrameData> m_FramesToSwap;
		Ref<Animation> m_Anim;
		Ref<Texture2D> m_PlayIcon;
		Ref<Texture2D> m_StopIcon;
		Ref<Texture2D> m_MoveIcon;
		Ref<Texture2D> m_MoveSelectedIcon;
		Ref<Texture2D> m_LenghtIcon;
		Ref<Texture2D> m_LenghtSelectedIcon;
		std::vector<Animation::FrameData> m_FramesData;
		int m_ClickedFrame;
	};
}