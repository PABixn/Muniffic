#include "egpch.h"
#include "Engine/Core/Input.h"
#include <GLFW/glfw3.h>
#include "Engine/Core/Application.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Platform/Windows/WindowsWindow.h"

namespace eg {
	auto Input::GetWindow()
	{
		return static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	}

	void Input::KeyCallback(int key, int action)
	{
		if (!ScriptEngine::IsInitialized())
			return;

		switch (action)
		{
			case GLFW_PRESS:
				for (auto& scriptInstance : ScriptEngine::GetAllScriptInstances())
					scriptInstance->InvokeOnKeyPress(key);
				break;
			case GLFW_RELEASE:
				for (auto& scriptInstance : ScriptEngine::GetAllScriptInstances())
					scriptInstance->InvokeOnKeyRelease(key);
				break;
		}
	}

	void Input::MouseCallback(int button, int action)
	{
		if (!ScriptEngine::IsInitialized())
			return;

		switch (action)
		{
			case GLFW_PRESS:
				for (auto& scriptInstance : ScriptEngine::GetAllScriptInstances())
					scriptInstance->InvokeOnMouseButtonPress(button);
				break;
			case GLFW_RELEASE:
				for (auto& scriptInstance : ScriptEngine::GetAllScriptInstances())
					scriptInstance->InvokeOnMouseButtonRelease(button);
				break;
		}
	}

	void Input::ScrollCallback(double xOffset, double yOffset)
	{
		if (!ScriptEngine::IsInitialized())
			return;

		for (auto& scriptInstance : ScriptEngine::GetAllScriptInstances())
			scriptInstance->InvokeOnScroll(xOffset, yOffset);
	}

	char* Input::GetClipboardContent()
	{
		return (char*)glfwGetClipboardString(GetWindow());
	}

	void Input::SetClipboardContent(char* content)
	{
		glfwSetClipboardString(GetWindow(), content);
	}

	char* Input::GetKeyName(KeyCode keycode)
	{
		return (char*)glfwGetKeyName((int)keycode, 0);
	}

	bool Input::IsKeyPressed(KeyCode keycode)
	{
		return GLFW_PRESS == glfwGetKey(GetWindow(), (int)keycode);
	}

	bool Input::IsKeyReleased(KeyCode keycode)
	{
		return GLFW_RELEASE == glfwGetKey(GetWindow(), (int)keycode);
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		return GLFW_PRESS == glfwGetMouseButton(GetWindow(), (int)button);
	}

	bool Input::IsMouseButtonReleased(MouseCode button)
	{
		return GLFW_RELEASE == glfwGetMouseButton(GetWindow(), (int)button);
	}

	bool Input::IsCursorOnWindow()
	{
		return glfwGetWindowAttrib(GetWindow(), GLFW_HOVERED);
	}

	std::pair<float, float> Input::GetCursorPositon()
	{
		double xPos, yPos;
		glfwGetCursorPos(GetWindow(), &xPos, &yPos);
		return { (float)xPos, (float)yPos };
	}

	float Input::GetCursorPositonX()
	{
		auto [x, y] = GetCursorPositon();
		return (float)x;
	}

	float Input::GetCursorPositonY()
	{
		auto[x,y] = GetCursorPositon();
		return (float)y;
	}

	/* modes:
		0 -> GLFW_CURSOR_NORMAL (makes the cursor visible and behaving normally)
		1 -> GLFW_CURSOR_HIDDEN (makes the cursor invisible when it is over the content area of the window but does not restrict the cursor from leaving)
		2 -> GLFW_CURSOR_DISABLED (hides and grabs the cursor, providing virtual and unlimited cursor movement. This is useful for implementing for example 3D camera controls)
	*/
	void Input::SetCursorMode(int mode)
	{
		auto window = GetWindow();
		switch (mode)
		{
			case 0:
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				break;
			case 1:
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
				break;
			case 2:
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				break;
		}
	}

	void Input::SetStickyKeysEnabled(bool enable)
	{
		glfwSetInputMode(GetWindow(), GLFW_STICKY_KEYS, enable ? GLFW_TRUE : GLFW_FALSE);
	}

	void Input::SetStickyMouseButtonsEnabled(bool enable)
	{
		glfwSetInputMode(GetWindow(), GLFW_STICKY_MOUSE_BUTTONS, enable ? GLFW_TRUE : GLFW_FALSE);
	}
}
