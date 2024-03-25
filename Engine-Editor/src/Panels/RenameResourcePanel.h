#pragma once

#include "Engine/Core/Application.h"
#include "Imgui/imgui.h"

namespace eg
{
	class RenameResourcePanel
	{
	public:
		RenameResourcePanel();
		void OnImGuiRender();
		void ShowWindow(UUID uuid);
		bool IsShown() { return m_Show; }

	protected:
		bool m_Show;
		UUID m_UUID;
	};
}