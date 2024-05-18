#pragma once
#include "Engine.h"

namespace eg {
	class NoFramesPanel {
	public:
		NoFramesPanel();
		void OpenNoFramesPanel();
		void OnImGuiRender();
		bool ButtonCenteredOnLine(const char* label, int width = 0, int height = 0, bool isDown = false, float alignment = 0.5f);
	private:
		bool m_ShowNoFramesPanel = false;
	};
}