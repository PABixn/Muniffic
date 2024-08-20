#pragma once

#include "Engine/Events/Event.h"
#include "Engine/Core/Core.h"
#include "Engine/Core/MouseButtonCodes.h"
#include "Engine/Core/KeyCodes.h"
namespace eg
{
	class Input {
		public:
			static void KeyCallback(int key, int action);
			static void MouseCallback(int button, int action);
			static void ScrollCallback(double xoffset, double yoffset);
			static bool IsKeyPressed(KeyCode keycode);
			static bool IsMouseButtonPressed(MouseCode button);
			static float GetMouseX();
			static float GetMouseY();
			static std::pair<float, float> GetMousePositon();
			static char* GetClipboardContent();
			static void SetClipboardContent(char* content);
	};

}
