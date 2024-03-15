#pragma once

#include "Engine/Core/Application.h"
#include "Imgui/imgui.h"

namespace eg {
	class InvalidDataPanel
	{
	public:
		InvalidDataPanel();
		void OnImGuiRender();
		void ShowWindow();
		bool IsShown() { return m_Show; }

	protected:
		bool m_Show;
	};
}