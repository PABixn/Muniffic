#include "Engine/Core/Application.h"
#include "Imgui/imgui.h"

#pragma once

namespace eg
{
	class NameNewProjectPanel
	{
	public:
		NameNewProjectPanel();
		void OnImGuiRender();
		void ShowWindow(UUID directoryUUID);
		bool IsShown() { return m_Show; }
		bool isNameGiven() { return m_GiveProjectName; }
		void setNameGiven(bool val) { m_GiveProjectName = val; }
		void setUUID(UUID uuid) { m_ProjectUUID = uuid; }
		std::string projectName;

	protected:
		bool m_GiveProjectName;
		bool m_Show;
		UUID m_ProjectUUID;
	};
}