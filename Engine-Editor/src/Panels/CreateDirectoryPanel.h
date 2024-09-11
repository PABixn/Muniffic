#pragma once

#include <filesystem>
#include "Engine/Core/Application.h"
#include "Imgui/imgui.h"
#include "../Commands/Commands.h"

namespace eg
{
	class CreateDirectoryPanel
	{
	public:
		CreateDirectoryPanel();
		void OnImGuiRender();
		void ShowWindow(UUID parentDirectoryUUID);
		bool IsShown() { return m_Show; }

	protected:
		bool m_Show;
		UUID m_DirectoryUUID;
		ImFont* m_PoppinsRegularFont;
	};
}