#pragma once
#include "Engine.h"

namespace eg {
	class NoFramesPanel {
	public:
		NoFramesPanel();
		bool OpenNoFramesPanel();
		void OnImGuiRender();
		bool ButtonCenteredOnLine(const char* label, int width = 0, int height = 0, bool isDown = false, float alignment = 0.5f);
		void TextCenteredOnLine(const char* label, float alignment = 0.5f);
	private:
		void ShowNoFramesPanel(bool show) { m_ShowNoFramesPanel = show; };
	private:
		bool m_ShowNoFramesPanel = false;
	};
}