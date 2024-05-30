#pragma once
#include "Engine.h"

namespace eg {
	class ContentBrowserRightClickPanel {
	public:
		bool OpenContentBrowserRightClickPanel(UUID key,float posX, float posY);
		void OnImGuiRender();
	private:
		void ShowRightClickPanel(bool show) { m_showRightClickPanel = show; }
	private:
		bool m_showRightClickPanel = false;
		UUID m_key;
		float m_posY;
		float m_posX;
	};
}