#pragma once
#include "Engine/Core/Application.h"
#include "../EditorLayer.h"
#include "Imgui/imgui.h"
#include "Engine/Project/Project.h"
#include "egpch.h"
#include <vector>
#include "ConsolePanel.h"
#include "Engine/Core/UUID.h"
#include "Engine.h"
#include "Engine/Scene/Hud.h"

namespace eg {

	class LayoutManagementPanel {
	public:
		LayoutManagementPanel() = default;
		void OnImGuiRender();
		void SetContext(const Ref<Scene>& scene);
		static std::vector<Hud*> Huds;
		void CreateHUD();
	private:
		Ref<Scene> m_Context;
	};


}