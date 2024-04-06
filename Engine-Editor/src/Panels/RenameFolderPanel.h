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
		void ShowWindow(UUID directoryUUID);
		bool IsShown() { return m_Show; }

	protected:
		bool m_Show;
		UUID m_DirectoryUUID;
	};
}