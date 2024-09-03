#pragma once

#include "Engine/Core/Application.h"
#include "../EditorLayer.h"
#include "Imgui/imgui.h"

namespace eg {

	class UnsavedChangesPanel
	{
	public:
		UnsavedChangesPanel();
		~UnsavedChangesPanel() = default;
		void SetUnsavedChangesPanelRender(bool val);
		bool GetUnsavedChangesPanelRender();
		void OnImGuiRender();
	};
}