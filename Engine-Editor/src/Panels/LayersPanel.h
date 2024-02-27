#pragma once

#include "Engine/Core/Application.h"
#include "../EditorLayer.h"
#include "Imgui/imgui.h"
#include "Engine/Project/Project.h"
#include "egpch.h"
#include <vector>

namespace eg {

	class LayersPanel {
	public:
		void OnImGuiRender();
	};

}