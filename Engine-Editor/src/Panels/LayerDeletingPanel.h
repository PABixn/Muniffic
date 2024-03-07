#pragma once

#include "Engine/Core/Application.h"
#include "../EditorLayer.h"
#include "Imgui/imgui.h"
#include "Engine/Project/Project.h"
#include "egpch.h"
//#include "LayersPanel.h"

namespace eg {



	class LayerDeletingPanel
	{
	public:
		LayerDeletingPanel() = default;
		void OnImGuiRender();
		bool IsShown() { return m_Show; }
		void Show() { m_Show = true; }
	protected:
		bool m_Show = false;
	};
}