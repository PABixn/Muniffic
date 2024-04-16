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
		}
		std::string name;
		void SetX(float number) { this->position.x = number; };
		void SetY(float number) { this->position.y = number; };
		float GetX() { return this->position.x; };
		float GetY() { return this->position.y; };
		void SetWidth(float number) { this->size.x = number; };
		void SetHeight(float number) { this->size.y = number; };
		float GetWidth() { return this->size.x; };
		float GetHeight() { return this->size.y; };
		void SetColor(float r, float g, float b, float a) { this->color = { r, g, b, a }; };
		glm::vec4 GetColor() { return this->color; };
	private:
		glm::vec2 position = { 0.0f, 0.0f };
		glm::vec2 size = { 0.0f, 0.0f };
		glm::vec4 color = {0.0f, 0.0f, 0.0f, 0.0f};
		
	};

}