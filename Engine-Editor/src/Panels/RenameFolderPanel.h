#include "Engine/Core/Application.h"
#include "Imgui/imgui.h"

#pragma once

namespace eg
{
	class RenameFolderPanel
	{
	public:
		RenameFolderPanel();
		void OnImGuiRender();
		void ShowWindow(std::filesystem::path path);
		bool IsShown() { return m_Show; }

	protected:
		bool m_Show;
		std::filesystem::path m_Path;
	};
}