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
		void ShowWindow(std::filesystem::path directoryPath);
		bool IsShown() { return m_Show; }

	protected:
		bool m_Show;
		std::filesystem::path m_DirectoryPath;
	};
}