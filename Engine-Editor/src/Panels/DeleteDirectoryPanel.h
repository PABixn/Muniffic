#pragma once

#include <filesystem>
#include "Engine/Core/Application.h"
#include "Imgui/imgui.h"
#include "../Commands/Commands.h"

namespace eg
{
	class DeleteDirectoryPanel
	{
	public:
		DeleteDirectoryPanel();
		void OnImGuiRender();
		void ShowWindow(UUID directoryUUID);
		bool IsShown() { return m_Show; }

	protected:
		bool m_Show;
		UUID m_DirectoryUUID;
	};
}