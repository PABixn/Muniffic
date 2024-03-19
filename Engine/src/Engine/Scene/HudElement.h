#pragma once
#include "Imgui/imgui.h"
#include "Engine/Project/Project.h"
#include "egpch.h"
#include <vector>
#include "Engine/Core/UUID.h"
#include "Engine.h"
#include "Engine/Core/Application.h"

namespace eg {

	class HudElement {
	public:
		HudElement() {
			this->name = "New HUD Element";
			this->x = 0;
			this->y = 0;
		}
		std::string name;
		void SetX(float number) { this->x = number; };
		void SetY(float number) { this->y = number; };
		float GetX() { return this->x; };
		float GetY() { return this->y; };
	private:
		float x;
		float y;
	};

}