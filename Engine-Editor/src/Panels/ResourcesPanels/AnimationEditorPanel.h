#pragma once
#include "Engine.h"
#include "Imgui/imgui.h"
namespace eg {
	class AnimationEditorPanel {
	public:
		AnimationEditorPanel() = default;
		AnimationEditorPanel(UUID asset);
		~AnimationEditorPanel() = default;
		bool OpenAnimationEditorPanel(UUID asset);
		void OnImGuiRender();
		void DrawAnimationPreview();
		void Update(float dt);
		bool IsAnimationEditorOpen() { return m_ShowAnimationEditorPanel; };
		void SetFrames();
	private:
		void ShowAnimationEditorPanel(bool show) { m_ShowAnimationEditorPanel = show; };
		void ResetData();
		void UpdateSelectedFrames();
		bool IsFrameSelected(int frame);
		void ResetSelectedFrames();
		void UpdateDisplayedFrame();

		void DrawPlayButton();
		void DrawFunctionCallPopup();
		void DrawAnimationOptions();
		void DrawFrameTrack();
		void DrawFunctionInfoPopup();
		void DrawFrameRate();
		void HandleFrameHover(int frameIndex, const ImVec2& cursorPos, const ImVec2& cursorPos2);
		void HandleFrameLeftClick(int clickedFrameIndex);
		void HandleFrameRightClick(int clickedFrameIndex);
		void HandleFrameDrag(int clickedFrameIndex);
		void HandleFrameLeftClickRelease(int clickedFrameIndex);
		void HandleResize();
		void HandleMove(int moveTo);
		void DrawActionButtons(const ImVec2& buttonSize, float gap);
		void DrawExitButtons();
		uint32_t GetFrameColor(int frame);
	private:
		std::pair<int, int> m_SelectedFrames = { -1, -1 };
		std::vector<UUID> m_DeletedFrames;
		Ref<Animation> m_Anim;
		bool m_ShowAnimationEditorPanel = false;
		bool m_PlayAnimation = false;
		bool m_ShowLongerFrame = false;
		bool m_ShowInitialFrame = true;
		bool m_Resize = false;
		bool m_Move = false;
		bool m_IsDisplayed = false;
		bool m_IsDragging = false;
		bool m_OpenFunctionCallPopup = false;
		bool m_OpenOptionsPopup = false;
		bool m_FrameReleased = false;
		int m_DraggingIndex = -1;
		int m_ClickedFrame;
		int m_DisplayedFrameIndex = 0;
		int m_HoveredFrame = -1;
		int m_FrameStartDuration = 1;
		float m_ResizeOffset = 0.0f;
	};
}