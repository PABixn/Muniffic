#pragma once

#include "Engine/Events/Event.h"
#include "Engine/Core/Core.h"
#include "Engine/Core/MouseButtonCodes.h"
#include "Engine/Core/KeyCodes.h"
namespace eg
{
	class Input {
		private:
			static void key_callback(auto window, int key, int scancode, int action, int mods);
		public:
			static void Init();
			static bool IsKeyPressed(KeyCode keycode);

			static bool IsMouseButtonPressed(MouseCode button);
			static float GetMouseX();
			static float GetMouseY();
			static std::pair<float, float> GetMousePositon();
	};

}
