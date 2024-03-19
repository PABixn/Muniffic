#pragma once
#include "Imgui/imgui.h"
#include "Engine/Project/Project.h"
#include "egpch.h"
#include <vector>
#include "Engine/Core/UUID.h"
#include "Engine.h"
#include "Engine/Core/Application.h"
#include "HudElement.h"

namespace eg {

	class Hud {
	public:
		Hud() {
			this->name = "New HUD";
		}
		std::string name;
		std::vector<HudElement*> elements;
		void CreateElement() {
			elements.push_back(new HudElement());
		}
	};

}