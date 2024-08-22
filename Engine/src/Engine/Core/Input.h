#pragma once

#include "Engine/Events/Event.h"
#include "Engine/Core/Core.h"
#include "Engine/Core/MouseButtonCodes.h"
#include "Engine/Core/KeyCodes.h"
namespace eg
{
	class Input {
		private:
			static GLFWwindow* GetWindow();
		public:
			static void KeyCallback(int key, int action);
			static void MouseCallback(int button, int action);
			static void ScrollCallback(double xoffset, double yoffset);

			static char* GetClipboardContent();
			static void SetClipboardContent(char* content);

			static char* GetKeyName(KeyCode keycode);
			static bool IsKeyPressed(KeyCode keycode);
			static bool IsKeyReleased(KeyCode keycode);

			static bool IsMouseButtonPressed(MouseCode button);
			static bool IsMouseButtonReleased(MouseCode button);

			static bool IsCursorOnWindow();
			static std::pair<float, float> GetCursorPositon();
			static float GetCursorPositonX();
			static float GetCursorPositonY();
			static void SetCursorMode(int mode);

			static void SetStickyKeysEnabled(bool enable);
			static void SetStickyMouseButtonsEnabled(bool enable);
	};

}
